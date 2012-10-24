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
	
}


