CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
HDRS =
OBJS = $(OBJ)/main.o
CFLAGS = -Wall -g -c -I$(INC) -std=c++11
EXE = $(BIN)/simulador

all: run

run: $(EXE)
	$(EXE) 4096 1024 2 src/laura.txt

$(EXE): $(OBJS)
	$(CC) -pg -o $(EXE) $(OBJS) $(LIBS)

$(OBJ)/%.o: $(SRC)/%.cpp $(HDRS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXE) $(OBJS) gmon.out
