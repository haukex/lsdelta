
Notes made after not touching this repository for a while:
- I don't remember why I didn't set up `make test` to work in this repo,
  so for now I set up a default `make lint` that leaves out the tests.
  One *can* run tests on the built module locally (Linux) as follows:
  - `python -m build`
  - `dev/isolated-dist-test.sh dist/lsdelta-*.tar.gz`
  - Clean: `git clean -dxf -e .venv*`
- Instructions for making a release via `cibuildwheel` are in
  `.github/workflows/build_wheels.yml`.
