CC=gcc
LEX=lex
YACC=yacc
FLAGS=-g -Wall#-O3
BIN=bin
SRC=.
TARGET=etapa3

SRC_FILES=$(SRC)/y.tab.c $(SRC)/lex.yy.c $(wildcard $(SRC)/*.c)
OBJ_FILES=$(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRC_FILES))
RM=$(SRC)/lex.yy.h $(SRC)/lex.yy.c $(SRC)/y.tab.h $(SRC)/y.tab.c

$(TARGET): $(OBJ_FILES)
	$(CC) $(FLAGS) -o $@ $^

$(BIN)/%.o: $(SRC)/%.c | $(BIN)
	$(CC) $(FLAGS) -c -o $@ $<

$(SRC)/lex.yy.c: $(SRC)/scanner.l
	$(LEX) --header-file=$(SRC)/lex.yy.h -o $@ $<

$(SRC)/y.tab.c: $(SRC)/parser.y $(SRC)/lex.yy.c
	$(YACC) -d -o $@ $<

$(BIN):
	mkdir -p $(BIN)

.PHONY: clean
clean:
	rm -f $(BIN)/*.o *~ *.dot $(TARGET) $(RM)

rebuild: clean $(TARGET)
