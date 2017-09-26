#created by ljk<ljk@xdja.com>
CC = cc
CFLAGS := -g -O0 -Wall -fPIC
CFLAGS += -D_DEBUG
CFLAGS += -I./
LDFLAGS += -L./
LIBS += -lc

.PHONY : default all clean

SRCS += event_kqueue.c test.c

OBJS = $(SRCS:.c=.o)

TARGET = test

default : all

all : ${TARGET}

${TARGET} : ${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS} ${LIBS}
	@echo "$@"

%.o : %.c %.h
	${CC} ${CFLAGS} -o $@ -c $<

clean :
	rm -rf ${OBJS} ${TARGET}
