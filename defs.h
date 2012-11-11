// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

// DEBUG VAR
int DEBUG = 0;

// Runtime vars
#define BUF_MAX 50
#define MAX_PIPE 4096
#define MISHSTDIN 1
#define MISHSTDOUT 2
char* curDir;
char usrInp = '\0';
char buf[BUF_MAX];
int bufChars = 0;

char *cmdArgs[BUF_MAX];
int cmdCount = 0;

// Mish process vars
pid_t mPID;
pid_t mPGID;
struct termios mTermios;
int mTerm;
int mCheck;

// Job control vars
typedef struct mishJob {
	char *name;
	int id;
	int stat;
	char *des;
	pid_t pid;
	pid_t pgid;
	struct mishJob *next;
} mishJob;
int activeJobs = 0;
mishJob* jobList = NULL;

// Index marker
int i;

