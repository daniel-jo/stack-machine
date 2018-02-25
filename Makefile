# Directories
SRC:=src
INCLUDE:=-I include
OBJ:=obj
BIN:=bin

# Compiler
CC:=gcc
CFLAGS:=-std=c99 -Wall -O2

# Project
OUTPUT:=vsm

all: $(OBJ)/*.o
	@echo "-- [Linking] ---------------------------------------------"
	$(CC) -o $(BIN)/$(OUTPUT) $^

$(OBJ)/%.o: $(SRC)/%.c
	@echo "-- [Compiling] -------------------------------------------"
	$(CC) $(CFLAGS) -c $? $(INCLUDE)
	mv ./*.o $(OBJ)/

dirs:
	@mkdir src include obj bin

.PHONY: clean
clean:
	rm bin/$(OUTPUT) obj/*.o