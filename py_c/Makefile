
PYTHON3BIN = python

.PHONY: build test install installdeps clean

build:
	$(PYTHON3BIN) -m build --sdist
	$(PYTHON3BIN) -m twine check --strict dist/*.tar.gz

test: build ../lsdelta_tests.json
	$(PYTHON3BIN) -m venv .venv_test
	.venv_test/bin/python -m pip install dist/*.tar.gz
	.venv_test/bin/python py_tests.py
	rm -r .venv_test

install: build test
	$(PYTHON3BIN) -m pip install dist/*.tar.gz

installdeps:
	$(PYTHON3BIN) -m pip install --upgrade --upgrade-strategy=eager pip wheel setuptools build twine

clean:
	rm -rf dist *.egg-info
