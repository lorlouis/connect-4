OBJS	= main.o client.o serv.o con4.o
SOURCE	= main.c client.c serv.c con4.c
HEADER	=
OUT	= a.out
CC	= gcc
FLAGS	= -g -c -Wall
LFLAGS	=

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

%.o: %.c
	$(CC) $(FLAGS) $^

clean:
	rm -f $(OBJS) $(OUT)
