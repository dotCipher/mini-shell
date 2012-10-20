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

SRC=

OBJECTS=

DBGOBJECTS=

EXEC_FILE=mish

$(EXEC_FILE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

debug: cipher_dbg

mish_dbg: $(DBGOBJECTS)
	$(CC) $(CDBGFLGS) -o $@ $(DBGOBJECTS)

%_dbg.o: %.c
	$(CC) $(CDBGFLGS) -c -o $@ $<
	
##### CLEAN RULE #####
clean:
	rm mish_dbg; rm *.o $(EXEC_FILE);
