OBJS	= client.o serv.o con4.o tui.o
SOURCE	= client.c serv.c con4.c tui.c
TEST_OBJS = tests.o
TEST_SOURCE = tests.c
EXEC_OBJS = main.o
EXEC_SOURCE = main.c
HEADER	=
OUT	= a.out
CC	= gcc
FLAGS	= -g -c -Wall
LFLAGS	=

all: $(OBJS) $(EXEC_OBJS)
	$(CC) -g $(OBJS) $(EXEC_OBJS) -o $(OUT) $(LFLAGS)

tests: $(OBJS) $(TEST_OBJS)
	$(CC) -g $(OBJS) $(TEST_OBJS) -o tests $(LFLAGS)

%.o: %.c
	$(CC) $(FLAGS) $^

clean:
	rm -f $(OBJS) $(OUT) $(TEST_OBJS) $(EXEC_OBJS) tests
