CXX	 		= g++
CXXFLAGS	= -g -Wall

SRC_DIR     = ./src
BUILD_DIR   = ./build
		
SRC			= $(wildcard $(SRC_DIR)/*.cpp)
OBJS    	= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
LIBS 		= -lusb-1.0 -lpulse-simple -lpulse
EXEC		= AndroidMicLinux


all: $(EXEC)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -o $@ -c $<

$(EXEC) : $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(EXEC)