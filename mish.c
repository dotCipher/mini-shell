// External includes
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

// Local includes
#include "defs.h"
#include "jobcontrol.h"
#include "utils.h"

void handleCmds(){
	if(builtInCmds() == 1){
		if(DEBUG == 1){
			printf("Builtin Command found:\n");
		}
		startJob(cmdArgs, "STANDARD", 0, 'F');
	} else {
		if(DEBUG == 1){
			printf("Non-Builtin Command found.\n");
		}
	}
}

void startShell(){
	// Init vars
	mTerm = STDIN_FILENO;
	mCheck = isatty(mTerm);
	mPID = getpid();
	activeJobs = 0;
	jobList = NULL;
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
		setpgid(mPID, mPID);
		mPGID=getpgrp();
		if(mPID != mPGID){
			printf("Error: Mish is not process group leader\n");
			exit(EXIT_FAILURE);
		}
		if(tcsetpgrp(mTerm, mPGID) == -1){
			tcgetattr(mTerm, &mTermios);
		}
		curDir = (char*) calloc(1024, sizeof(char));
	} else {
		printf("Error: Mish could not be made interactive.");
		exit(EXIT_FAILURE);
	}
}

/* [-------- Main Method --------]
 */
int main(int argc, char *argv[], char **envp){
	// Start up everything
	startShell();
	writePrompt();
	
	// Start Loop for reading input
	while(1){
		usrInp = getchar();
		switch(usrInp){
			case '\n':
				writePrompt();
				break;
			default:
				readInput();
				handleCmds();
				writePrompt();
				break;
		}
	}
	printf("\n");
	exit(0);
}









