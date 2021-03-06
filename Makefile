PATH := /usr/local/bin/:$(PATH)
CC = gcc
CC_FLAGS = -std=gnu99 -pedantic -g -Wall -Werror `pkg-config glib-2.0 --cflags` `pkg-config yaml-0.1 --cflags` -Isrc/
L_FLAGS = `pkg-config glib-2.0 --libs` `pkg-config yaml-0.1 --libs`
UNAME = $(shell uname)
LIB_PATH = $(addsuffix /lib/osx/, $(shell pwd))

ifeq ($(UNAME), Darwin)
	CC_FLAGS += -F lib/osx/ -F /Library/Frameworks/ -I/usr/local/opt/libyaml/include/ -Wno-gnu-zero-variadic-macro-arguments
	L_FLAGS += -F lib/osx/ -F /Library/Frameworks/ -framework SDL2 -framework SDL2_image -Wl,-rpath,$(LIB_PATH),-rpath,/Library/Frameworks/
else ifeq ($(UNAME), Linux)
	CC_FLAGS += -lm `pkg-config sdl2 --cflags` `pkg-config SDL2_image --cflags`
	L_FLAGS += -lm `pkg-config sdl2 --libs` `pkg-config SDL2_image --libs`
endif

EXEC = zelda
SOURCES = $(wildcard src/*.c src/enemies/*.c src/tests/*.c)
SOURCES_MINUS_TESTS = $(filter-out src/tests/%.c, $(SOURCES))
SOURCES_MINUS_MAIN = $(filter-out src/main.c, $(SOURCES_MINUS_TESTS))
OBJECTS = $(SOURCES:src/%.c=obj/%.o)
OBJECTS_MINUS_TESTS = $(SOURCES_MINUS_TESTS:src/%.c=obj/%.o)

all: dirs clean bin/$(EXEC) package-resources

lint: CC_FLAGS += -fsyntax-only -ansi -DNDEBUG
lint: dirs clean $(OBJECTS)

# ----

test: CC_FLAGS += -DTESTS `pkg-config --cflags check`
test: L_FLAGS += `pkg-config --libs check`
test: clean-tests test-dirs $(OBJECTS) package-resources
	$(CC) $(OBJECTS) $(L_FLAGS) -o bin/tests

clean-tests: clean
	rm -f obj/tests/*.o

test-dirs: dirs
	@mkdir -p obj/tests/

# ----

dirs:
	@mkdir -p bin/ obj/ obj/enemies

bin/$(EXEC): $(OBJECTS_MINUS_TESTS)
	$(CC) $(OBJECTS_MINUS_TESTS) $(L_FLAGS) -o $@

obj/%.o: src/%.c
	$(CC) -c $< $(CC_FLAGS) -o $@

clean:
	rm -f bin/$(EXEC) obj/*.o
	rm -rf bin/res

package-resources:
	cp -r res/ bin/res/

# ----

tools: clean map-checker

tools-dirs: dirs
	@mkdir -p bin/tools obj/tools

# ----

MAP_CHECKER_SOURCES = $(wildcard src/tools/map-checker/*.c)
MAP_CHECKER_SOURCES += $(SOURCES_MINUS_MAIN)
MAP_CHECKER_OBJECTS = $(MAP_CHECKER_SOURCES:src/%.c=obj/%.o)

map-checker: clean map-checker-dirs bin/tools/map-checker

map-checker-dirs: tools-dirs
	@mkdir -p obj/tools/map-checker

bin/tools/map-checker: $(MAP_CHECKER_OBJECTS)
	$(CC) $(MAP_CHECKER_OBJECTS) $(L_FLAGS) -o $@