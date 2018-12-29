CC := g++
CFLAGS := -g
TARGET := turing
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %cpp, %o, $(SRC))

all:$(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	-rm -rf $(TARGET) *.o */console.txt */result.txt