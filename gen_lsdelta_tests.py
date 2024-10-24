#!/usr/bin/env python
"""Generates C test cases for this project from JSON.

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

def main() -> None:
    with (Path(__file__).resolve().parent/'lsdelta_tests.json').open(encoding='ASCII') as ifh:
        tests = [ t for t in json.load(ifh) if not isinstance(t, str) ]
    with (Path(__file__).resolve().parent/'test_lsdelta_tests.inc.c').open('w', encoding='ASCII') as ofh:
        print(f"// ***** DO NOT EDIT ***** this file is generated by {Path(__file__).name}\n"
            f"#define NUM_TESTS ({len(tests)})\n"
            'struct _struct_test {\n'
            '\tchar *a;\n' '\tchar *b;\n'
            '\tchar rv;\n' '\tlong long o;\n'
            '};\n' 'struct _struct_test TEST[NUM_TESTS] = {', file=ofh)
        for test in tests:
            if isinstance(test, str):
                continue
            assert ( isinstance(test, list) and len(test)==3 and isinstance(test[0], str) and isinstance(test[1], str)
                and (test[2] is None or isinstance(test[2],int)) ), test
            for x in test[0:2]:
                assert '"' not in x and '\\' not in x, test
            print(f"\t{{ .a = \"{test[0]}\", .b = \"{test[1]}\","
                  + ( " .rv=-1, .o=0 " if test[2] is None else f" .rv=0, .o={test[2]} " )
                  +"},", file=ofh)
        print('};', file=ofh)

if __name__=='__main__':
    main()
