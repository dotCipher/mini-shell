/*  ------------ Show Jobs ------------ 
 *		Shows all mishJobs.
 *  Returns:
 *		void
 */
void showJobs(){
	mishJob* jobs = jobList;
	// jobs called for display
	// pointing to jobsList
	if(jobs == NULL){
		printf("No active jobs->\n");
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

/*  ------------ Get Job ------------ 
 *		Gets the mishJob with a value
 *		of "val" of the parameter of
 *		"par". The return is the mishJob
 *		that was found.
 *		par = 1 == Get by jID
 *		par = 2 == Get by pID
 *		par = 3 == Get by stat
 *  Returns:
 *		mishJob* = SUCCESS
 *		NULL = FAILURE
 */
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
	return NULL;
}

/*  ------------ Delete Job ------------ 
 *		Deletes mishJob of "checkJob"
 *		within global jobList. The return
 *		of a mishJob* is the current 
 *		mishJob after the deleted mishJob
 *  Returns:
 *		mishJob* = SUCCESS
 *		NULL = FAILURE
 */
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

/*  ------------ Update Job Status ------------ 
 *		Updates mishJob's status of pid "jPID"
 *		to status of "jSTATUS"
 *  Returns:
 *		1 = SUCCESS
 *		0 = FAILURE
 */
int updateJobStatus(int jPID, int jSTATUS){
	mishJob *checkJob = jobList;
	if(checkJob == NULL){
		return 0;
	} else {
		while(checkJob != NULL){
			if(checkJob->pid == jPID){
				checkJob->stat = jSTATUS;
				return 1;
			}
			checkJob = checkJob->next;
		}
		return 0;
	}
}

/*  ------------ Add Job ------------ 
 *	Adds a new job to the current list
 *		of jobs with pid of "jPID",
 *		pgid of "jPGID", name of "jNAME",
 *		descriptor of "jDES", status
 *		of "jSTATUS" and then returns
 *		the pointer to the new list of
 *		jobs
 *  Returns:
 *		mishJob* = Pointer to new job list
 */
mishJob* addJob(pid_t jPID, pid_t jPGID, char* jNAME, char* jDES, int jSTATUS){
	if(DEBUG == 1){
		printf("\nJob To Add:\n");
		printf("PID = %d\n", jPID);
		printf("PGID = %d\n", jPGID);
		printf("NAME = %s\n", jNAME);
		printf("DESCRIPTOR = %s\n", jDES);
		printf("STATUS = %d\n", jSTATUS);
	}
	mishJob *jobToAdd = malloc(sizeof(mishJob));
	jobToAdd->pid = jPID;
	jobToAdd->pgid = jPGID;
	jobToAdd->name = (char*)malloc(sizeof(jNAME));
	memset(&jobToAdd->name,0,sizeof(jNAME));
	strcpy(jobToAdd->name,jNAME);
	jobToAdd->des = (char*)malloc(sizeof(jDES));
	memset(&jobToAdd->des,0,sizeof(jDES));
	strcpy(jobToAdd->des,jDES);
	jobToAdd->stat = jSTATUS;
	jobToAdd->next = NULL;
	if(jobList == NULL){
		activeJobs++;
		jobToAdd->id = activeJobs;
		return jobToAdd;
	} else {
		mishJob *prevJob = jobList;
		while(prevJob->next != NULL){
			prevJob = prevJob->next;
		}
		jobToAdd->id = prevJob->id + 1;
		prevJob->next = jobToAdd;
		activeJobs++;
		return jobList;
	}
}

/*  ------------ Kill Job ------------ 
 *	Kills the job of "jID" using syscall
 *		kill()
 *	Returns:
 *		void
 */
void killJob(int jID){
//	getJob(x,1) == Get by jID
	mishJob *jobCheck = getJob(jID, 1);
	kill(jobCheck->pid, SIGKILL);
}

/*  ------------ Suspend Job ------------ 
 *	Sets the job of mishJob "job" to wait
 *		status
 *	Returns:
 *		void
 */
void suspendJob(mishJob* job){
	int tStat;
	while(waitpid(job->pid, &tStat, WNOHANG) == 0){
		if(job->stat == 'S'){
			return;
		}
	}
	jobList = delJob(job);
}

/*  ------------ Foreground Job ------------ 
 *	Sets the job of mishJob "job" to 
 *		foreground status using int "jobBool"
 *	Returns:
 *		void
 */
void foregroundJob(mishJob* job, int jobBool){
	job->stat = 'F';
	tcsetpgrp(mTerm, job->pgid);
	if(jobBool==1){
		errno=0;
		if(kill(-job->pgid,SIGCONT) < 0){
			perror(" kill SIGCONT");
		}
	}
	suspendJob(job);
	tcsetpgrp(mTerm,mPGID);
}

/*  ------------ Background Job ------------ 
 *	Sets the job of mishJob "job" to 
 *		foreground status using int "jobBool"
 *	Returns:
 *		void
 */
void backgroundJob(mishJob* job, int jobBool){
	if(job==NULL){
		return;
	}
	if(jobBool==1 && job->stat != 'W'){
		job->stat = 'W';
	}
	if(jobBool==1){
		errno=0;
		if(kill(-job->pgid,SIGCONT) < 0){
			perror(" kill SIGCONT");
		}
	}
	tcsetpgrp(mTerm,mPGID);
}

/*  ------------ Run Command ------------ 
 *	Runs the command array of "cmds" on 
 *		file descriptor of "newfd" using
 *		file descriptor of "fd".  During the
 *		execution, the integer execMode is
 *		used.
 *	Returns:
 *		void
 */
void runCmd(char *cmds[], char *fd, int newfd, int execMode){
	int cmdDes;
	if(newfd == MISHSTDIN){
		cmdDes = open(fd, O_RDONLY, 0600);
		dup2(cmdDes, STDIN_FILENO);
		close(cmdDes);
	}
	if(newfd == MISHSTDOUT){
		cmdDes = open(fd, O_CREAT | O_TRUNC | O_WRONLY, 0600);
		dup2(cmdDes, STDOUT_FILENO);
		close(cmdDes);
	}
	if(execvp(*cmds, cmds) == -1){
		perror("Error executing command(s) ");
	}
}

/*  ------- Mish Child Process Handler ------- 
 *	Handles the execution of any child process
 *		ran by the MISH terminal.
 *	Returns:
 *		void
 */
void mChildHandler(int sig){
	int tStat;
	pid_t jPID;
	jPID = waitpid(-1, &tStat, WUNTRACED | WNOHANG);
	if(jPID > 0){
		mishJob* jobCheck = getJob(jPID, 2);
		if(jobCheck == NULL){
			return;
		} else {
			if(WIFEXITED(tStat)){
				if(jobCheck->stat == 'B'){
					printf("\n[%4d] %32s -> Done\n",
						jobCheck->id, jobCheck->name);
					jobList = delJob(jobCheck);
				}
			} else if(WIFSIGNALED(tStat)){
				printf("\n[%4d] %32s -> Killed\n",
					jobCheck->id, jobCheck->name);
				jobList = delJob(jobCheck);
			} else if(WIFSTOPPED(tStat)){
				if(jobCheck->stat == 'B'){
					tcsetpgrp(mTerm, mPGID);
					updateJobStatus(jPID, 'W');
					printf("\n[%4d] %32s -> Suspended\n",
						jobCheck->id, jobCheck->name);
				} else {
					// Job is in foreground
					tcsetpgrp(mTerm, jobCheck->pgid);
					updateJobStatus(jPID, 'S');
					printf("\n[%4d] %32s -> Stopped\n",
						jobCheck->id, jobCheck->name);
				}
				return;
			} else {
				if(jobCheck->stat == 'B'){
					jobList = delJob(jobCheck);
				}
			}
			tcsetpgrp(mTerm, mPGID);
		}
	}
}

/*  ------------ Start Job ------------ 
 *	Starts a job using the command array 
 *		of "cmds" on file descriptor of 
 *		"newfd" using file descriptor of "fd".
 *		During the execution, the integer 
 *		execMode is used.
 *	Returns:
 *		void
 *		exit on failure
 *		exit on success
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
			printf(" [%4d] %8d \n", ++activeJobs, (int)getpid());
			runCmd(cmds,fd,newfd,execMode);
			exit(0);
		}
	} else {
		setpgid(pid,pid);
		if(DEBUG == 1){
			printf("\nAdding Job:\n");
			printf("pid = %d\n", pid);
			for(i=0; i<sizeof(cmds); i++){
				printf("cmds[%d] = %s\n", i, cmds[i]);
			}
			printf("fd = %s\n", fd);
			printf("execMode = %c\n", execMode);
		}
		// Add to job list
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




