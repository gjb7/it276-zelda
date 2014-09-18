CC = gcc
CC_FLAGS = -g -Wall -pedantic
UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)
	CC_FLAGS += -I`brew --prefix glib`/include -F lib/osx/
endif

EXEC = zelda
SOURCES = $(wildcard src/*.c src/tests/*.c)
SOURCES_MINUS_TESTS = $(filter-out src/tests/%.c, $(SOURCES))
OBJECTS = $(SOURCES:src/%.c=obj/%.o)
OBJECTS_MINUS_TESTS = $(SOURCES_MINUS_TESTS:src/%.c=obj/%.o)

all: dirs bin/$(EXEC)

test: CC_FLAGS += -DTESTS `pkg-config --cflags check`
test: L_FLAGS += `pkg-config --libs check`
test: clean test-dirs $(OBJECTS)
	$(CC) $(OBJECTS) -o bin/tests $(L_FLAGS)
	@bin/tests

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