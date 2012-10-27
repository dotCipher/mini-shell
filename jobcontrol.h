void showJobs(){
	t_job* jobs = jobList;
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

t_job* getJob(int val, int par){
	t_job* jobs = jobList;
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

t_job* delJob(t_job* checkJob){
	t_job* curJob;
	t_job* prevJob;
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
	t_job *checkJob = jobList;
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

t_job* addJob(pid_t jPID, pid_t jPGID, char* jNAME, char* jDES, int jSTATUS){
	t_job *jobToAdd = malloc(sizeof(t_job));
	jobToAdd->pid = jPID;
	jobToAdd->pgid = jPGID;
	jobToAdd->name = (char*)malloc(sizeof(name));
	jobToAdd->name = strcpy(jobToAdd->name,name);
	jobToAdd->des = (char*)malloc(sizeof(des));
	jobToAdd->des = strcpy(jobToAdd->des,des);
	jobToAdd->next = NULL;
	if(jobList == NULL){
		activeJobs++;
		jobToAdd->id = activeJobs;
		return jobToAdd;
	} else {
		t_job *nextJob = jobList;
		while(nextJob->next != NULL){
			nextJob = nextJob->next;
		}
		jobToAdd->id = nextJob->id + 1;
		nextJob->nest = jobToAdd;
		activeJobs++;
		return jobList;
	}
}






