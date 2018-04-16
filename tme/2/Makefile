#			Programmation Repartie (4I400)
#		Makefile pour TD 7. Introduction aux sémaphores


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =mpicc
LDFLAGS =
CFLAGS =

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
INCLUDE=$(DIR)/include/
LIB=$(DIR)/lib/
SRC=$(DIR)/src/

# HC=include/mpi_server.h

.SUFFIXES:
.PHONY: all clean test test-philosophe #test-exo5
all: $(BIN)philosophe #$(BIN)exo5

test: test-philosophe #test-exo5

test-philosophe: $(BIN)philosophe
	-mpirun -np 4 bin/philosophe

# test-exo5: $(BIN)exo5
# 	-mpirun -np 4 bin/exo5


# compilation specifique avec un header et une deuxième source
# $(BIN)exo5: $(SRC)exo5.c $(SRC)mpi_server.c
# 	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
# 	$(CC) -o $@ $^ $(LDFLAGS) -I $(INCLUDE)



$(BIN)%: $(SRC)%.c
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ)%.o: $(LIB)%.c
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

clean: 
	rm $(OBJ)*.o $(BIN)*
