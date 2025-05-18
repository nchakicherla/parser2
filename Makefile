CC = gcc
CFLAGS = -std=c11 -Wall -Wpedantic -Werror -o3
OS := $(shell uname)

mkBinDir := $(shell mkdir -p bin)
mkObjDir := $(shell mkdir -p obj)

MAIN = ./obj/main.o
MAIN_BIN = ./bin/main.run

OBJS = 	./obj/rkp_error.o \
		./obj/rkp_arena.o \

TESTS = ./obj/tests.o
TESTS_BIN = ./bin/tests.run

tests: $(TESTS_BIN)
ifeq ($(OS),Darwin) 
	$(TESTS_BIN)
else 
	valgrind --track-origins=yes --leak-check=full $(TESTS_BIN)  
endif

main: $(MAIN_BIN)
ifeq ($(OS),Darwin) 
	$(MAIN_BIN)
else 
	valgrind --track-origins=yes --leak-check=full $(MAIN_BIN)  
endif

reset: clear $(MAIN_BIN)

run: reset $(MAIN_BIN)
	$(MAIN_BIN)

$(MAIN_BIN): $(OBJS) $(MAIN)
	$(CC) $(CFLAGS) $(OBJS) $(MAIN) -o $(MAIN_BIN)

$(TESTS_BIN): $(OBJS) $(TESTS)
	$(CC) $(CFLAGS) $(OBJS) $(TESTS) -o $(TESTS_BIN)

./obj/%.o: ./src/%.c ./src/%.h
	$(CC) $(CFLAGS) -c $< -o $@

./obj/main.o: ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o ./obj/main.o
	
./obj/tests.o: ./src/tests.c
	$(CC) $(CFLAGS) -c ./src/tests.c -o ./obj/tests.o

clear: clear-bin clear-obj

clear-bin:
	-rm ./bin/*

clear-obj:
	-rm ./obj/*
