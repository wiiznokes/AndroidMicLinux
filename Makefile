CC	 	= g++
FLAGS	= -g -Wall

SRC	= $(wildcard *.c)
HEADER	= $(SRC:.c=.h)
OBJS	= $(SRC:.c=.o)
EXEC	= AndroidMicLinux

OBJS_SUB_DIR = build

all: $(EXEC)

%.o : %.c
	$(CC) -o $(OBJS) -c $(SRC)

$(EXEC) : $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(FLAGS)

clean:
	rm -f $(OBJS) $(EXEC)