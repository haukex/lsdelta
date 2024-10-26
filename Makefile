
.PHONY: all test install clean

# Debug build: make clean test CFLAGS="-DLSDELTA_DEBUG"

CC = gcc
local_CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Werror
DEST_LIB ?= /usr/local/lib
DEST_INC ?= /usr/local/include
# sudo apt install libgmp-dev
LINK_LIBS = -Wl,-rpath=$(shell pwd) -L. -llsdelta -lgmp

all: liblsdelta.so

liblsdelta.so: lsdelta.c lsdelta.h
	$(CC) $(local_CFLAGS) $(CFLAGS) -shared -o "$@" -fPIC lsdelta.c

test_lsdelta_tests.inc.c: gen_lsdelta_tests.py lsdelta_tests.json
	python gen_lsdelta_tests.py

test_lsdelta: liblsdelta.so test_lsdelta.c test_lsdelta_tests.inc.c
	$(CC) $(local_CFLAGS) $(CFLAGS) -o test_lsdelta test_lsdelta.c $(LINK_LIBS)

test: test_lsdelta
	./test_lsdelta

install: liblsdelta.so lsdelta.h
	install -m 644 liblsdelta.so $(DEST_LIB)
	install -m 644 lsdelta.h $(DEST_INC)
	ldconfig

clean:
	rm -f liblsdelta.so test_lsdelta


.PHONY: py_installdeps py_build py_clean py_test

py_installdeps:
	pip install --upgrade --upgrade-strategy=eager pip wheel setuptools build

py_build:
	python -m build --sdist

py_test: py_build
	python -m venv .venv_test
	.venv_test/bin/python -m pip install dist/*.tar.gz
	.venv_test/bin/python py_tests.py
	rm -r .venv_test

py_clean:
	rm -rf dist *.egg-info
