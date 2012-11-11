# Makefile for HW2
# by: Cody Moore
# CSE 306

# C-Compiler (default: GNU C-Compiler)
CC=gcc

# Compile flags (default:
# -O2 : Optimize (more)
# -g  : Debug info for OS
# -Wall -Werror : all warnings = errors
CFLAGS=-g -O2 -Wall -Werror
CDBGFLGS=-g -O0 -Wall -Werror

SRC=mish.c

MISH_HEADERS=defs.h \
	utils.h \
	jobcontrol.h

OBJECTS=mish.o

DBGOBJECTS=mish_dbg.o

EXEC_FILE=mish

##### MAIN RULE ######
$(EXEC_FILE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS); \
	rm *.o;

##### OBJECT RULES #####
mish.o: mish.c $(MISH_HEADERS)
	$(CC) $(CFLAGS) -c $<

##### DEBUG RULES #####
debug: mish_dbg

mish_dbg: $(DBGOBJECTS)
	$(CC) $(CDBGFLGS) -o $@ $(DBGOBJECTS)

mish_dbg.o: mish.c $(MISH_HEADERS)
	$(CC) $(CDBGFLGS) -c -o $@ $<

##### CLEAN RULE #####
clean:
	rm mish_dbg; rm *.o $(EXEC_FILE);
