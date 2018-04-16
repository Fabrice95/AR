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

HC=include/mpi_server.h

.SUFFIXES:
.PHONY: all clean test test-exo1_chang_roberts test-exo3
all: $(BIN)exo2 $(BIN)exo3 $(BIN)exo4 $(BIN)exo5

test: test-exo2 test-exo3 test-exo4 test-exo5

test-exo1_chang_roberts: $(BIN)exo1
	-mpirun -np 4 bin/exo1

test-exo3: $(BIN)exo3
	-mpirun -np 4 bin/exo3

test-exo4: $(BIN)exo4
	-mpirun -np 4 bin/exo4

test-exo5: $(BIN)exo5
	-mpirun -np 4 bin/exo5


# compilation specifique avec un header et une deuxième source
$(BIN)exo5: $(SRC)exo5.c $(SRC)mpi_server.c
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS) -I $(INCLUDE)



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
