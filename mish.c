// External includes
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
// Local includes
#include "utils.h"
#include "utils.h"
#include "builtins.h"

void mChildHandler(int sig){
	
}

void startShell(){
	// Init vars
	mTerm = STDIN_FILENO;
	mCheck = isatty(mTerm);
	mPID = getpid();
	// Is the terminal interactive now?
	if(mCheck==1){
		while(tcgetpgrp(mTerm) != (mPGID=getpgrp())){
			kill(mPID,SIGTTIN);
		}
		// Ignore all irrelevant signals
		signal(SIGINT, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		// Any child jobs need to be handled
		signal(SIGCHLD, &mChildHandler);
		// 
		//setpgid(mPID, mPID);
		mPGID=getpgrp();
		if(mPID != mPGID){
			printf("Error: Mish is not process group leader\n");
			exit(ERROR_PGID);
		}
		if(tcsetgrp(mTerm, mPGID) == -1){
			tcgetattr(mTerm, &mTermios);
		}
		curDir = (char*) malloc(sizeof(char)*1024);
	} else {
		printf("Error: Mish could not be made interactive.");
		exit(ERROR_INTER);
	}
}

/* [-------- Main Method --------]
 */
int main(int argc, char *argv[]){
	/*
	// Start while loop 1
	while(1){
		int childPid;
		char *cmdLine;
		char *cmd;
		printPrompt();
		cmdLine = readCommandLine();
		cmd = parseCommandLine();
		// Maybe record command in mish history?
		if(isBuiltInCommand(cmd)){
			executeBuiltInCommand(cmd);
		} else {
			childPid = fork();
			if(childPid == 0){
				// call execvp
				executeCommand(cmd);
			} else {
				if(isBackgroundJob(cmd)){
					// record in list of background jobs
				} else {
					waitpid(childPid);
				}
			}
		}
	}
	// End while loop 1
	*/
	
	// Start up everything
	startShell();
	writePrompt();
	
	// Start Loop for reading input
	while(TRUE){
		usrInp = getchar();
		switch(userInp){
			case '\n':
				writePrompt();
				break;
			default:
				readInput();
				handleCommands();
				writePrompt();
				break;
		}
	}
	printf("\n");
	return 0;
}









