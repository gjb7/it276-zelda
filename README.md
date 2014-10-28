# Zelda

A Legend of Zelda: A Link to the Past clone for IT 276 at NJIT.

[![Build Status](https://travis-ci.org/gjb7/it276-zelda.svg?branch=develop)](https://travis-ci.org/gjb7/it276-zelda)

## Dependencies

1. glib
2. check (for unit testing)
3. SDL 2.0

## Installation

Running `./install-dependencies.sh` will install the dependencies necessary for it276-zelda. Once the dependencies are installed, you can run `make` to compile the game.

## Tests

Unit tests can be run by executing `make test` and then `bin/tests`. Unit tests are handled by [check](http://check.sourceforge.net/), which is installed as part of `./install-dependencies.sh`.
