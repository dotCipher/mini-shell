void chDir(){
	if(commandArray[1]==NULL){
		chdir(getenv("HOME"));
	} else {
		if(chdir(cmdArgs[1]) == -1){
			printf("%s: %s: Not a directory\n",
				cmdArgs[0],cmdArgs[1]);
		}
	}
}
