CC = gcc
CC_FLAGS = -g -Wall -pedantic -Wno-gnu-zero-variadic-macro-arguments `pkg-config glib-2.0 --cflags`
L_FLAGS = `pkg-config glib-2.0 --libs`
UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)
	CC_FLAGS += -F lib/osx/
endif

EXEC = zelda
SOURCES = $(wildcard src/*.c src/tests/*.c)
SOURCES_MINUS_TESTS = $(filter-out src/tests/%.c, $(SOURCES))
OBJECTS = $(SOURCES:src/%.c=obj/%.o)
OBJECTS_MINUS_TESTS = $(SOURCES_MINUS_TESTS:src/%.c=obj/%.o)

all: dirs clean bin/$(EXEC)

test: CC_FLAGS += -DTESTS `pkg-config --cflags check`
test: L_FLAGS += `pkg-config --libs check`
test: clean-tests test-dirs $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/tests $(L_FLAGS)
	@bin/tests

clean-tests: clean
	rm -f obj/tests/*.o

test-dirs: dirs
	@mkdir -p obj/tests/

dirs:
	@mkdir -p bin/ obj/

bin/$(EXEC): $(OBJECTS_MINUS_TESTS)
	$(CC) $(OBJECTS_MINUS_TESTS) -o $@ $(L_FLAGS)

obj/%.o: src/%.c
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f bin/$(EXEC) obj/*.o