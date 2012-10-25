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
		// Find by pID
	} else if(par==2){
		// Find by jID
	} else if(par==3){
		// Find by stat
	} else {
		// Nothing set
		return NULL;
		break;
	}
}
