void showJobs(){
	mishJob* jobs = jobList;
	// jobs called for display
	// pointing to jobsList
	if(jobs == NULL){
		printf("No active jobs.\n");
	} else {
		// [jID]	pID		des		name	->	status
		printf(" [%4s] %8s %16s %32s -> %6s",
			"JID", "PID", "DESCRIPTOR", "JOB_NAME", "STATUS");
		while(jobs != NULL){
			printf(" [%4d] %8d %16s %32s -> %6c", 
				jobs->id, jobs->pid, jobs->des, 
				jobs->name, job->stat);
			jobs = jobs->next;
		}
	}
}

// par = 1 == Get by jID
// par = 2 == Get by pID
// par = 3 == Get by status
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
			if(jobs->status == val){
				return jobs;
			} else {
				jobs = jobs->next;
			}
		}
	} else {
		// Nothing set
		return NULL;
		break;
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
				checkJob->status = jSTATUS;
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
	jobToAdd->name = (char*)malloc(sizeof(name));
	jobToAdd->name = strcpy(jobToAdd->name,name);
	jobToAdd->des = (char*)malloc(sizeof(des));
	jobToAdd->des = strcpy(jobToAdd->des,des);suspendJob(job);
		tcsetpgrp(mTerm,mPGID);
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
		nextJob->nest = jobToAdd;
		activeJobs++;
		return jobList;
	}
}

void killJob(int jID){
	mishJob *jobCheck = getJob(jID, 1);
	kill(jobCheck->pid, SIGKILL);
}

void foregroundJob(mishJob* job, int jobBool){
	job->status = FG;
	tcsetpgrp(mTerm, job->pgid);
	if(jobBool==1){
		errno=0;
		if(kill(job->pgid,SIGCONT) < 0)){
			perror(" kill SIGCONT");
		}
	}
	suspendJob(job);
	tcsetpgrp(mTerm,mPGID);
}
// Make job go into background


void startJob(char *cmds[], char *fd. int newfd, int execMode){
	pid_t pid;
	pid = fork();
	if(pid==-1){
	
	} else if(pid==0){
	
	} else {
		setpgid(pid,pid);
		jobList = addJob(pid,pid,*cmds,fd,execMode);	
		// Check Job by pID
		mishJob* jobCheck = getJob(pid,2);
		// execMode = 1 == Foreground
		// execMode = 2 == Background
		if(execMode == 1){
			foregroundJob(jobCheck, 0);
		}
		if(execMode == 1){
			
		}
	}
}




