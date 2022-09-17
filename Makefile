CC	 	= g++
FLAGS	= -g -Wall

SRC_DIR     = ./src
BUILD_DIR   = ./build
		
SRC		= $(wildcard $(SRC_DIR)/*.cpp)
OBJS	= $(BUILD_DIR)/$(notdir $(SRC:.cpp=.o))
LIBS 	= -lusb-1.0 -lpulse-simple -lpulse
EXEC	= AndroidMicLinux


all: $(EXEC)

%.o : %.c
	$(CC) -o $(OBJS) -c $(SRC)

$(EXEC) : $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LIBS) $(FLAGS)

clean:
	rm -f $(OBJS) $(EXEC)