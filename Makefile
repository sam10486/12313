SRC 	:= ./src
OBJ 	:= ./obj
INC 	:= ./include
UTIL	:= $(SRC)/util
BIN		:= ./bin

SOURCE 		:= $(wildcard $(SRC)/*.cpp)
OBJS 		:= $(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(SOURCE)))
MAIN_SOURCE	:= $(wildcard $(UTIL)/*.cpp)
MAIN_OBJS	:= $(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(MAIN_SOURCE)))

CC = g++
CFLAGS = -g -Wall

LIBS	:= -lntl -lgmp -lm
INCLUDE	:= -I $(INC)
TARGET	:= $(BIN)/main

all: main #$(TARGET)

$(MAIN_OBJS):$(OBJ)/%.o: $(UTIL)/%.cpp
	$(CC) -c $< -o $@ $(INCLUDE)

$(OBJS):$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(INCLUDE)


$(TARGET): $(MAIN_OBJS) $(OBJS) 
	$(CC) -o $@ $(MAIN_OBJS) $(OBJS) $(LIBS)

main: $(TARGET)
	$(BIN)/main

barrett: $(OBJS) $(MAIN_OBJS) 
	$(CC) -o $(BIN)/barrett.exe $(OBJS) $(MAIN_OBJS) $(LIBS)
	$(BIN)/barrett.exe

AE: $(OBJS) $(MAIN_OBJS) 
	$(CC) -o $(BIN)/AE.exe $(OBJS) $(MAIN_OBJS) $(LIBS)
	$(BIN)/AE.exe

.PHONY: clean


clean:
	rm -rf $(TARGET)
	rm -rf $(OBJ)/*.o
	rm -rf $(BIN)/*.exe
	rm -rf $(BIN)/main