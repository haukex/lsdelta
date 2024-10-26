"""Tests for Python implementation(s) of ``lsdelta``.

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
import unittest
from pathlib import Path
import lsdelta
import lsdelta_pp

class LSDeltaTestCase(unittest.TestCase):

    def test_lsdelta(self):
        for uut in (lsdelta_pp, lsdelta):
            with (Path(__file__).resolve().parent.parent/'lsdelta_tests.json').open(encoding='ASCII') as ifh:
                tests = [ t for t in json.load(ifh) if not isinstance(t, str) ]
            for test in tests:
                if test[2] is None:
                    with self.assertRaises(ValueError):
                        uut.lsdelta(test[0], test[1])
                else:
                    t0, t1 = test[0:2]
                    t2 = int(test[2])
                    self.assertEqual( uut.lsdelta(t0, t1), t2, f"{uut.__name__}.lsdelta({t0!r}, {t1!r}) == {t2!r}" )
                    t0 = test[0].encode('ASCII')
                    t1 = test[1].encode('ASCII')
                    self.assertEqual( uut.lsdelta(t0, t1), t2, f"{uut.__name__}.lsdelta({t0!r}, {t1!r}) == {t2!r}" )
