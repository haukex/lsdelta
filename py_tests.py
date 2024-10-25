#!/usr/bin/env python
"""Runs tests on the Python version of lsdelta.

Author, Copyright, and License
------------------------------

Copyright (c) 2024 Hauke Daempfling (haukex@zero-g.net)
at the Leibniz Institute of Freshwater Ecology and Inland Fisheries (IGB),
Berlin, Germany, https://www.igb-berlin.de/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see https://www.gnu.org/licenses/
"""
import json
from pathlib import Path
from lsdelta import lsdelta

#TODO: This should be turned into normal unit tests.
# https://stackoverflow.com/a/65710653 ??

def main() -> None:
    with (Path(__file__).resolve().parent/'lsdelta_tests.json').open(encoding='ASCII') as ifh:
        tests = [ t for t in json.load(ifh) if not isinstance(t, str) ]
    for test in tests:
        if isinstance(test, str):
            continue
        assert ( isinstance(test, list) and len(test)==3 and isinstance(test[0], str) and isinstance(test[1], str)
            and (test[2] is None or isinstance(test[2],int)) ), test
        for x in test[0:2]:
            assert '"' not in x and '\\' not in x, test
        if test[2] is None:
            try:
                lsdelta(test[0], test[1])
            except ValueError:
                print(f"({test[0]!r},{test[1]!r}) == error => PASS")
            else:
                raise RuntimeError(f"Failed test ({test[0]!r},{test[1]!r}) == error")
        else:
            if lsdelta(test[0], test[1]) == test[2]:
                print(f"({test[0]!r},{test[1]!r}) == {test[2]!r} => PASS")
            else:
                raise RuntimeError(f"Failed test ({test[0]!r},{test[1]!r}) == {test[2]!r}")

if __name__=='__main__':
    main()
