
.PHONY: installdeps build clean test

installdeps:
	pip install --upgrade --upgrade-strategy=eager pip wheel setuptools build

build:
	python -m build --sdist

test: build ../lsdelta_tests.json
	python -m venv .venv_test
	.venv_test/bin/python -m pip install dist/*.tar.gz
	.venv_test/bin/python py_tests.py
	rm -r .venv_test

clean:
	rm -rf dist *.egg-info
