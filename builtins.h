void chDir(){
	if(cmdArgs[1]==NULL){
		if(chdir(getenv("HOME")) == -1){
			printf("%s: Enviroment variable HOME not found\n",
			cmdArgs[0]);
		}
	} else {
		if(chdir(cmdArgs[1]) == -1){
			printf("%s: %s: Not a directory\n",
				cmdArgs[0],cmdArgs[1]);
		}
	}
}

/*
 * 1 = Exit shell
 *
 */
int builtInCmds(){
	if(strcmp("exit", cmdArgs[0]) == 0){
		return 1;
	}
	if(strcmp("cd", cmdArgs[0]) == 0){
		chDir();
		return 2;
	}
	if(strcmp("bg", cmdArgs[0]) == 0){
		if(cmdArgs[1] == NULL){
			//return 0;
		} else {
			if(strcmp("in", cmdArgs[1]) == 0){
				//launchJob(cmdArgs + 3, *(cmdArgs+2), STDIN, BACKGROUND);
			}
			else if(strcmp("out", cmdArgs[1]) == 0){
				//launchJob(cmdArgs + 3, *(cmdArgs+2), STDOUT, BACKGROUND);
			} else{
				//launchJob(cmdArgs+1, "STANDARD", 0, BACKGROUND);
			}
		}
		//return 1;
	}
	if(strcmp("fg", cmdArgs[0]) == 0){
		if(cmdArgs[1] == NULL){
			//return 0;
		} else {
			//int jID = (int) atoi(amdArgs[1]);
			//mishJob* job = getJob(jID, BY_JOB_ID);command
			if(job == NULL){
				//return 0;
			}
			else if(job->status == SUSPENDED || job->status == WAITING_INPUT){
				//putJobForegound(job, TRUE);
			} else {
				//putJobForeground(job, FALSE);
			}
		}
		//return 1;
	}
	if(strcmp("jobs", cmdArgs[0]) == 0){
		printJobs();
		return 1;
	}
	if(strcmp("kill", cmdArgs[0]) == 0){
	
	}
	if(strcommandcmp("|", cmdArgs[0]) == 0){
	
	}
}


