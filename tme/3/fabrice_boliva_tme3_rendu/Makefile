#			Algorothmique Repartie (4I403)
#		Makefile pour TME 3. Algorithme de l'arbre et l'echo


CC = mpicc
LDFLAGS =
CFLAGS =

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
INCLUDE=$(DIR)/include/
LIB=$(DIR)/lib/
SRC=$(DIR)/src/

HC=

.SUFFIXES:
.PHONY: all clean test-arbre test-echo arbre echo
all: $(BIN)arbre $(BIN)echo

test: test-arbre test-echo

test-arbre: $(BIN)arbre
	-mpirun --hostfile $$master/ar/tme/host -np 7 $$PWD/bin/arbre

test-echo: $(BIN)echo
	-mpirun --hostfile $$master/ar/tme/host -np 7 $$PWD/bin/echo

$(BIN)%: $(OBJ)%.o
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)%.o: $(LIB)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

clean: 
	rm -Rf $(OBJ)

clean_all:
	rm -Rf $(OBJ); rm -Rf $(BIN)