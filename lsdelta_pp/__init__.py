"""Pure-Python implementation of ``lsdelta``.

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
import re
from typing import Union

_decimal_re = re.compile(r'\A[+-]?([0-9]*)(?:\.([0-9]*))?\Z')

def lsdelta(a :Union[str,bytes], b :Union[str,bytes]) -> int:
    if isinstance(a, bytes):
        a = a.decode('UTF-8')
    if isinstance(b, bytes):
        b = b.decode('UTF-8')
    if not (a_m := _decimal_re.fullmatch(a)):
        raise ValueError(f"Invalid decimal {a=}")
    if not (a_m[1] or a_m[2]):
        raise ValueError(f"Invalid decimal {a=}")
    if not (b_m := _decimal_re.fullmatch(b)):
        raise ValueError(f"Invalid decimal {b=}")
    if not (b_m[1] or b_m[2]):
        raise ValueError(f"Invalid decimal {b=}")
    # get count of digits after decimal
    a_cnt = len(a_m[2]) if a_m[2] else 0
    b_cnt = len(b_m[2]) if b_m[2] else 0
    # pad the strings
    if b_cnt>a_cnt:
        a += "0" * (b_cnt-a_cnt)
    if a_cnt>b_cnt:
        b += "0" * (a_cnt-b_cnt)
    # remove decimal
    a = a.replace(".","")
    b = b.replace(".","")
    # convert to int and return delta
    return int(a) - int(b)
