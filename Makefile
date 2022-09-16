CC	 	= g++
FLAGS	= -g -Wall

SRC		= $(wildcard *.c)
HEADER	= $(SRC:.c=.h)
OBJS	= $(SRC:.c=.o)
LIBS 	= -lusb-1.0 -lpulse-simple -lpulse
EXEC	= AndroidMicLinux

OBJS_SUB_DIR = build

all: $(EXEC)

%.o : %.c
	$(CC) -o $(OBJS) -c $(SRC)

$(EXEC) : $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LIBS) $(FLAGS)

clean:
	rm -f $(OBJS) $(EXEC)