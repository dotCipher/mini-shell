#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

// BOOLEAN OPERATORS
#define TRUE 1
#define FALSE 0

// ERROR RETURN VALUES
#define SUCCESS 0
#define ERROR_INTER 2
#define ERROR_PGID 3

#define BUF_MAX 64
static char* curDir;
static char usrInp = '\0';
static char buf[BUF_MAX];
static int bufChars = 0;

static char *cmdArgs[5];
static int cmdCount = 0;

static pid_t mPID;
static pid_t mPGID;
static struct termios mTermios;
static int mTerm;
static int mCheck;

