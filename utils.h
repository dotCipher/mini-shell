void setNullString(char *string){
	int newlineInt = (int)'\n';
	if(DEBUG == 1){
		printf("\nNull Terminating the string:\n");
	}
	if((int)string[strlen(string)-1] == (newlineInt)){
			if(DEBUG == 1){
				printf("...already null terminated!\n");
			}
	}
	else {
		if(DEBUG == 1){
			// FIX THIS
			printf("string[%d] = %s]\n", (int)strlen(string)-1, string[i]);
		}
		string[strlen(string)-1] = newlineInt;
	}
}

/*  ------------ Write Prompt ------------ 
 *	Writes the main prompt to the terminal
 *	Returns:
 *		void
 */
void writePrompt(){
	printf("mish:%s> ",getcwd(curDir, 1024));
}

/*  ------------ Parse Commands ------------ 
 *	Parses all commands in the command array
 *	Returns:
 *		void
 */
void parseCmds(){
	char *p;
	p = strtok(buf, " ");
	memset(&cmdArgs,0,sizeof(cmdArgs));
	
	while(p != NULL){
		cmdArgs[cmdCount] = p;
		p = strtok(NULL, " ");
		cmdCount++;
	}
}

/*  ------------ Reset Input ------------ 
 *	Resets the comman array inputs
 *	Returns:
 *		void
 */
void resetInput(){
	while (cmdCount != 0) {
		cmdArgs[cmdCount] = NULL;
		cmdCount--;
	}
	bufChars = 0;
	memset(&buf,0,sizeof(buf));
	setNullString(buf);
}

/*  ------------ Read Input ------------ 
 *	Reads the input character by character
 *		from the buffer array.
 *	Returns:
 *		void
 */
void readInput(){
	resetInput();
	while ((usrInp != '\n') && (bufChars < BUF_MAX)) {
		buf[bufChars++] = usrInp;
		usrInp = getchar();
	}
	buf[bufChars] = 0x00;
	parseCmds();
}

/*  ------------ Change Directory ------------ 
 *	Changes the current working directory in
 *		the terminal.
 *	Returns:
 *		void
 */
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

/*  ------------ Pipeline ------------ 
 *	Handles the piping of commands from
 *		one input or output to another.
 *		Uses the integer "f" as a flag 
 *		for parsing.
 *	Returns:
 *		void
 */
void pipeline(int f){
	int i;
  char pl[MAX_PIPE];
  FILE *fp_in, *fp_out;
  char cmd1[BUF_MAX],cmd2[BUF_MAX];
  if(f==2){
  	// strcpy ?
	  strcat(cmd1,cmdArgs[0]);
	  strcat(cmd2,cmdArgs[2]);
	} else {
		for(i=0; i<f-1; i++){
			strcat(cmd1,cmdArgs[i]);
		  strcat(cmd1," ");
	  }
	  strcpy(cmd2,cmdArgs[cmdCount-1]);
	}
  puts(cmdArgs[0]);
  puts(cmd1);
  puts(cmd2);
  
  if ((fp_in = popen(cmd1, "r")) == NULL){
  	printf("popen error with arg: %s\n", cmd1);
  }
  if ((fp_out = popen(cmd2, "w")) == NULL){
  	printf("popen error with arg: %s\n", cmd2);
  }
  while(fgets(pl, MAX_PIPE, fp_in) != NULL){
  	if(fputs(pl, fp_out) == EOF){
  		printf("fputs error with pipe\n");
  	}
  }
  if(ferror(fp_in)){
  	printf("fgets error with file pipe input\n");
  }
  if(pclose(fp_out) == -1){
  	printf("pclose error with fpout\n");
  }     
}


/*  ------- Check Built In Commands ------- 
 *	Checks the command array for any built
 *		in commands and handles them.
 *	Returns:
 *		1 = is built in
 *		0 = is not built in
 */
int builtInCmds(){
	if(strcmp("exit", cmdArgs[0]) == 0){
		exit(EXIT_SUCCESS);
	}
	if(strcmp("cd", cmdArgs[0]) == 0){
		chDir();
		return 1;
	}
	if(strcmp("bg", cmdArgs[0]) == 0){
		if(cmdArgs[1] == NULL){
			return 0;
		}
		//strcmp in and out at cmdArgs[1] ?
		else {
			startJob(cmdArgs + 1, "STANDARD", 0, 'B');
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
			if(job->stat == 'S' || job->stat == 'W'){
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






