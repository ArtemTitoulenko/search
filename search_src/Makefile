CC ?= gcc
# CFLAGS ?= -std=c99
CFDEBUG = -pedantic -Wall -g

EXEC = search
SRCS = search.c util.c hash.c sorted-list.c repl.c
OBJS = ${SRCS:.c=.o}
BIN_DIR = ../

all: ${EXEC}

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} -g -o ${EXEC} ${OBJS}
	mv ${EXEC} ${BIN_DIR}

debug: ${EXEC}
debug: CC += ${CFDEBUG}

test: debug
	./${EXEC} test_index

clean:
	rm -rf ./*.o
	rm -rf ./${EXEC}

.PHONY: all debug clean
