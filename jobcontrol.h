void showJobs(){
	mishJob* jobs = jobList;
	// jobs called for display
	// pointing to jobsList
	if(jobs == NULL){
		printf("No active jobs.\n");
	} else {
		// [jID]	pID		des		name	->	stat
		printf(" [%4s] %8s %16s %32s -> %6s\n",
			"JID", "PID", "DESCRIPTOR", "JOB_NAME", "STATUS");
		while(jobs != NULL){
			printf(" [%4d] %8d %16s %32s -> %6c\n", 
				jobs->id, jobs->pid, jobs->des, 
				jobs->name, jobs->stat);
			jobs = jobs->next;
		}
	}
}

// par = 1 == Get by jID
// par = 2 == Get by pID
// par = 3 == Get by stat
mishJob* getJob(int val, int par){
	mishJob* jobs = jobList;
	if(par==1){
		// Find by jID
		while(jobs!=NULL){
			if(jobs->id == val){
				return jobs;
			} else {
				jobs = jobs->next;
			}
		}
	} else if(par==2){
		// Find by pID
		while(jobs!=NULL){
			if(jobs->pid == val){
				return jobs;
			} else {
				return jobs = jobs->next;
			}
		}
	} else if(par==3){
		// Find by stat
		while(jobs!=NULL){
			if(jobs->stat == val){
				return jobs;
			} else {
				jobs = jobs->next;
			}
		}
	} else {
		// Nothing set
		return NULL;
	}
}

mishJob* delJob(mishJob* checkJob){
	mishJob* curJob;
	mishJob* prevJob;
	if(jobList == NULL){
		return NULL;
	}
	curJob = jobList->next;
	prevJob = jobList;
	if(prevJob->pid == checkJob->pid){
		prevJob = prevJob->next;
		activeJobs--;
		return curJob;
	}
	while(curJob != NULL){
		if(curJob->pid == checkJob->pid){
			activeJobs--;
			prevJob->next = curJob->next;
		}
		prevJob = curJob;
		curJob = curJob->next;
	}
	return jobList;
}

int updateJobStatus(int jPID, int jSTATUS){
	int i;
	mishJob *checkJob = jobList;
	if(checkJob == NULL){
		return 0;
	} else {
		i=0;
		while(checkJob != NULL){
			if(checkJob->pid == jPID){
				checkJob->stat = jSTATUS;
				return 1;
			}
			i++;
			checkJob = checkJob->next;
		}
		return 0;
	}
}

mishJob* addJob(pid_t jPID, pid_t jPGID, char* jNAME, char* jDES, int jSTATUS){
	mishJob *jobToAdd = malloc(sizeof(mishJob));
	jobToAdd->pid = jPID;
	jobToAdd->pgid = jPGID;
	jobToAdd->name = (char*)malloc(sizeof(jNAME));
	jobToAdd->name = strcpy(jobToAdd->name,jNAME);
	jobToAdd->des = (char*)malloc(sizeof(jDES));
	jobToAdd->des = strcpy(jobToAdd->des,jDES);
	jobToAdd->stat = jSTATUS;
	jobToAdd->next = NULL;
	if(jobList == NULL){
		activeJobs++;
		jobToAdd->id = activeJobs;
		return jobToAdd;
	} else {
		mishJob *nextJob = jobList;
		while(nextJob->next != NULL){
			nextJob = nextJob->next;
		}
		jobToAdd->id = nextJob->id + 1;
		nextJob->next = jobToAdd;
		activeJobs++;
		return jobList;
	}
}

void killJob(int jID){
	mishJob *jobCheck = getJob(jID, 1);
	kill(jobCheck->pid, SIGKILL);
}

void suspendJob(mishJob* job){
	int tStat;
	while(waitpid(job->pid, &tStat, WNOHANG) == 0){
		if(job->stat == SP){
			return;
		}
	}
	jobList = delJob(job);
}

void foregroundJob(mishJob* job, int jobBool){
	if(job==NULL){
		return;
	}
	job->stat = FG;
	tcsetpgrp(mTerm, job->pgid);
	if(jobBool==1){
		errno=0;
		if(kill(job->pgid,SIGCONT) < 0){
			perror(" kill SIGCONT");
		}
	}
	suspendJob(job);
	tcsetpgrp(mTerm,mPGID);
}

void backgroundJob(mishJob* job, int jobBool){
	if(job==NULL){
		return;
	}
	if(jobBool==1 && job->stat != WI){
		job->stat = WI;
	}
	if(jobBool==1){
		errno=0;
		if(kill(job->pgid,SIGCONT) < 0){
			perror(" kill SIGCONT");
		}
	}
	tcsetpgrp(mTerm,mPGID);
}

/*
 * Method call to run parsed cmds
 */
void runCmd(char *cmds[], char *fd, int newfd, int execMode){
	int cmdDes;
	if(newfd == STDIN_FILENO){
		cmdDes = open(fd, 
			O_RDONLY, 0600);
		dup2(cmdDes, STDIN_FILENO);
		close(cmdDes);
	}
	if(newfd == STDOUT_FILENO){
		cmdDes = open(fd, 
			O_CREAT | O_TRUNC | O_WRONLY, 0600);
		dup2(cmdDes, STDOUT_FILENO);
		close(cmdDes);
	}
	if(execvp(*cmds, cmds) == -1){
		perror("Error executing command(s) ");
	}
}

void mChildHandler(int sig){
	// signalHandler_child
	int tStat;
	pid_t jPID;
	jPID = waitpid(-1, &tStat, WUNTRACED | WNOHANG);
	if(jPID > 0){
		mishJob* jobCheck = getJob(jPID, 2);
		if(jobCheck == NULL){
			return;
		} else {
			if(WIFEXITED(tStat)){
				if(jobCheck->stat == BG){
					printf("\n[%4d] %32s -> Done\n",
						jobCheck->id, jobCheck->name);
					jobList = delJob(jobCheck);
				}
			} else if(WIFSIGNALED(tStat)){
				printf("\n[%4d] %32s -> Killed\n",
					jobCheck->id, jobCheck->name);
				jobList = delJob(jobCheck);
			} else if(WIFSTOPPED(tStat)){
				if(jobCheck->stat == BG){
					tcsetpgrp(mTerm, mPGID);
					updateJobStatus(jPID, WI);
					printf("\n[%4d] %32s -> Suspended\n",
						jobCheck->id, jobCheck->name);
				} else {
					// Job is in foreground
					tcsetpgrp(mTerm, jobCheck->pgid);
					updateJobStatus(jPID, SP);
					printf("\n[%4d] %32s -> Stopped\n",
						jobCheck->id, jobCheck->name);
				}
				return;
			} else {
				if(jobCheck->stat == BG){
					jobList = delJob(jobCheck);
				}
			}
			tcsetpgrp(mTerm, mPGID);
		}
	}
}

/*
 * Will have exit states
 */
void startJob(char *cmds[], char *fd, int newfd, int execMode){
	pid_t pid;
	pid = fork();
	if(pid==-1){
		perror("Error starting job ");
		exit(2);
	} else if(pid==0){
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGCHLD, &mChildHandler);
		setpgrp();
		if(execMode == 1){
			// Foreground
			tcsetpgrp(mTerm,getpid());
		}
		if(execMode == 2){
			// Background
			printf(" [%4d] %8d \n",
			++activeJobs, (int)getpid());
			runCmd(cmds,fd,newfd,execMode);
			exit(0);
		}
	} else {
		setpgid(pid,pid);
		jobList = addJob(pid,pid,*(cmds),fd,(int)execMode);	
		// Check Job by pID
		mishJob* jobCheck = getJob(pid,2);
		// execMode = 1 == Foreground
		// execMode = 2 == Background
		if(execMode == 1){
			foregroundJob(jobCheck, 0);
		}
		if(execMode == 2){
			backgroundJob(jobCheck, 0);
		}
	}
}




