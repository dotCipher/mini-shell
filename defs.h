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

// PROGRAM RETURN VALUES
#define SUCCESS 0
#define ERROR_INTER 1
#define ERROR_PID 2
#define ERROR_PGID 3

// JOB STATUSES
#define FG 1
#define BG 2
#define SP 3
#define WI 4

// Runtime vars
#define BUF_MAX 64
#define MAX_PIPE 1024
static char* curDir;
static char usrInp = '\0';
static char buf[BUF_MAX];
static int bufChars = 0;

static char *cmdArgs[64];
static int cmdCount = 0;

// Mish process vars
static pid_t mPID;
static pid_t mPGID;
static struct termios mTermios;
static int mTerm;
static int mCheck;

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
static int activeJobs = 0;
static mishJob* jobList = NULL;
