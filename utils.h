void writePrompt(){
	printf("mish:%s> ",getcwd(curDir, 1024));
}

void resetInput(){
	while (cmdCount != 0) {
		cmdArgs[cmdCount] = NULL;
    cmdCount--;
  }
  bufChars = 0;
}

void parseCmds(){
	char *p;
	p = strtok(buf, "");
	while(p != NULL){
		cmdArgs[cmdCount] = p;
		p = strtok(NULL, "");
		cmdCount++;
	}
}

void readInput(){
	resetInput();
	while ((usrInp != '\n') && (bufChars < BUF_MAX)) {
		buf[bufChars++] = usrInp;
		usrInp = getchar();
	}
  buf[bufChars] = 0x00;
  parseCmds();
}

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

void pipeline(int f){
	int i;
  char pl[MAX_PIPE];
  FILE *fp_in, *fp_out;
  char cmd1[BUF_MAX],cmd2[BUF_MAX];
  if(f==2){
  	// strcpy ?
	  strcat(cmd1,commandArgv[0]);
	  strcat(cmd2,commandArgv[2]);
	} /* else {
		for(i=0; i<f-1; i++){
			strcat(cmd1,cmdArgs[i]);
		  strcat(cmd1," ");
	  }
	  strcpy(cmd2,cmdArgs[cmdCount-1]);
	}
  puts(cmdArgs[0]);
  puts(cmd1);
  puts(cmd2);
  
  if ((fpin = popen(arg1, "r")) == NULL)
  	printf("can't open %s", arg1);

    if ((fpout = popen(arg2, "w")) == NULL)
        printf("popen error");


    while (fgets(line, MAXLINE, fpin) != NULL) {
        if (fputs(line, fpout) == EOF)
            printf("fputs error to pipe");
    }
    if (ferror(fpin))
        printf("fgets error");
    if (pclose(fpout) == -1)
        printf("pclose error\n");
        */
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
			return 0;
		}
		//strcmp in and out at cmdArgs[1] ?
		else {
			startJob(cmdArgs + 1, "STANDARD", 0, BG);
		}
		return 1;
	}
	if(strcmp("fg", cmdArgs[0]) == 0){
		if(cmdArgs[1] == NULL){
			return 0;
		} else {
			int jID = (int) atoi(cmdArgs[1]);
			mishJob* job = getJob(jID, 1);
			if(job == NULL){
				return 0;
			}
			if(job->stat == SP || job->stat == WI){
				foregroundJob(job, 1);
			} else {
				foregroundJob(job, 0);
			}
			return 1;
		}
	}
	if(strcmp("jobs", cmdArgs[0]) == 0){
		showJobs();
		return 1;
	}
	if(strcmp("kill", cmdArgs[0]) == 0){
		if(cmdArgs[1] == NULL){
			return 0;
		} else {
			killJob(atoi(cmdArgs[1]));
			return 1;
		}
	}
	// Is a pipe used?
	if(cmdCount == 3){
		if(strcmp("|", cmdArgs[1]) == 0){
			pipeline(2);
			return 1;
		}
	} else if(cmdCount > 3){
		if(strcmp("|", cmdArgs[cmdCount-2]) == 0){
			pipeline(cmdCount-1);
			return 1;
		}
	}
	return 0;
}






