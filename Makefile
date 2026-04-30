CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude

PREFIX=/usr/local

SRC=src/debugger.c
OBJ=$(SRC:.c=.o)

TEST_SRC=tests/test_runner.c 
TEST_BIN=test_runner

all: cdbg

cdbg: $(OBJ) tools/main.c
	$(CC) $(CFLAGS) tools/main.c $(OBJ) -o cdbg

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(OBJ)
	$(CC) $(CFLAGS) $(TEST_SRC) $(OBJ) -o $(TEST_BIN)
	./$(TEST_BIN)

install:
	install -d $(PREFIX)/bin
	install cdbg $(PREFIX)/bin/cdbg

install_headers:
	install -d $(PREFIX)/include/cdbg
	install include/cdbg/*.h $(PREFIX)/include/cdbg/

clean:
	rm -f cdbg $(OBJ) $(TEST_BIN)

.PHONY: all tests clean install install_headers
