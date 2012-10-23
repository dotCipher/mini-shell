void writePrompt(){
	printf("mish:%s> ",getcwd(currentDirectory, 1024));
}

void resetInput(){
	while (commandCount != 0) {
		cmdArgs[commandCount] = NULL;
    commandCount--;
  }
  bufChars = 0;
}

void readInput(){
	resetInput();
	while ((userInput != '\n') && (bufferChars < BUFFER_MAX_LENGTH)) {
		buf[bufferChars++] = userInput;
		userInput = getchar();
	}
  buffer[bufferChars] = 0x00;
  parseCommands();
}

void parseCommands(){
	char *p;
	p = strtok(buf, "");
	while(p != NULL){
		cmdArgs[commandCount] = p;
		p = strtok(NULL, "");
		commandCount++;
	}
}
