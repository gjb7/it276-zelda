CC = gcc
CC_FLAGS = -g -Wall -pedantic
UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)
	CC_FLAGS += -I`brew --prefix glib`/include -F lib/osx/
endif

EXEC = zelda
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

all: dirs bin/$(EXEC)

dirs:
	@mkdir -p bin/ obj/

bin/$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(L_FLAGS)
  
obj/%.o: src/%.c
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f bin/$(EXEC) obj/*.o