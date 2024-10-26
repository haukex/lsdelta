
PYTHON3BIN = python

.PHONY: build install installdeps clean

build:
	$(PYTHON3BIN) -m build --sdist
	$(PYTHON3BIN) -m twine check --strict dist/*.tar.gz

install: build
	$(PYTHON3BIN) -m pip install dist/*.tar.gz

installdeps:
	$(PYTHON3BIN) -m pip install --upgrade --upgrade-strategy=eager pip wheel setuptools build twine

clean:
	rm -rf dist *.egg-info
