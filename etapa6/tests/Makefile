CC=./lang161.sh

SRC_FILES=$(wildcard ./*.txt)
ASM_FILES=$(patsubst ./%.txt, ./%.s, $(SRC_FILES))
TARG_FILES=$(basename $(SRC_FILES))

all: $(TARG_FILES)

./%: ./%.txt ../etapa6 ../lang161.sh
	@ cd .. ; \
	$(CC) tests/$<

../etapa6:
	cd .. ; make

.PHONY: clean
clean:
	rm -f $(TARG_FILES) $(ASM_FILES)

rebuild: clean all
