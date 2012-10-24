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
		
	}
}








