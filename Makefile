
.PHONY: all test install clean

# Debug build: make clean test CFLAGS="-DLSDELTA_DEBUG"

CC = gcc
local_CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Werror
DEST_LIB ?= /usr/local/lib
DEST_INC ?= /usr/local/include

all: liblsdelta.so

liblsdelta.so: lsdelta.c lsdelta.h
	$(CC) $(local_CFLAGS) $(CFLAGS) -shared -o "$@" -fPIC lsdelta.c

test_lsdelta_tests.inc.c: gen_lsdelta_tests.py lsdelta_tests.json
	python gen_lsdelta_tests.py

test: liblsdelta.so test_lsdelta.c test_lsdelta_tests.inc.c
	$(CC) $(local_CFLAGS) $(CFLAGS) -o test_lsdelta test_lsdelta.c -Wl,-rpath=$(shell pwd) -L. -llsdelta && ./test_lsdelta

install: liblsdelta.so lsdelta.h
	install -m 644 liblsdelta.so $(DEST_LIB)
	install -m 644 lsdelta.h $(DEST_INC)
	ldconfig

clean:
	rm -f liblsdelta.so test_lsdelta


.PHONY: py_installdeps py_build py_clean

py_installdeps:
	pip install --upgrade --upgrade-strategy=eager pip wheel setuptools build

py_build:
	python -m build --sdist

py_clean:
	rm -rf dist *.egg-info
