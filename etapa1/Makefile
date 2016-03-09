CC=gcc
CXXFLAGS=-g -Wall #-O3
BIN=bin
SRC=.
LEX=flex
OBJ=$(BIN)/hashtable.o $(BIN)/lex.yy.o
TARGET=etapa1

$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(SRC)/lex.yy.c: $(SRC)/scanner.l
	$(LEX) --header-file=$(SRC)/lex.yy.h -o $@ $^

clean:
	rm -f $(BIN)/*.o *~ $(TARGET) $(SRC)/lex.yy.h $(SRC)/lex.yy.c
