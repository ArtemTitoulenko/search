CC ?= gcc
# CFLAGS ?= -std=c99
CFLAGS ?= -g
CFDEBUG = -pedantic -Wall -g

EXEC = search
SRCS = search.c parser.c cache.c util.c hash.c sorted-list.c repl.c
OBJS = ${SRCS:.c=.o}

all: ${EXEC}

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

${EXEC}: ${OBJS}
	${CC} ${LDFLAGS} -g -o ${EXEC} ${OBJS}

debug: ${EXEC}
debug: CC += ${CFDEBUG}

test: debug
	./${EXEC} test_index

clean:
	rm -rf ./*.o
	rm -rf ./${EXEC}

.PHONY: all debug clean
