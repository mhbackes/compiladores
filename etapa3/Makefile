CC=gcc
LEX=lex
YACC=yacc
CXXFLAGS=-g -Wall#-O3
BIN=bin
SRC=.
OBJ=$(BIN)/y.tab.o $(BIN)/lex.yy.o $(BIN)/hashtable.o $(BIN)/main.o
TARGET=etapa2

$(TARGET): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(SRC)/lex.yy.c: $(SRC)/scanner.l
	$(LEX) --header-file=$(SRC)/lex.yy.h -o $@ $<

$(SRC)/y.tab.c: $(SRC)/parser.y $(SRC)/lex.yy.c
	$(YACC) -d -o $@ $<

clean:
	rm -f $(BIN)/*.o *~ $(TARGET) $(SRC)/lex.yy.h $(SRC)/lex.yy.c $(SRC)/y.tab.h $(SRC)/y.tab.c

rebuild: clean $(TARGET)
