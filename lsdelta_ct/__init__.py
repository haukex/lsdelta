"""Python Ctypes wrapper for ``lsdelta`` C library.

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
#import os
#import ctypes
from typing import Union

#_lsdelta = ctypes.CDLL(os.environ.get('LSDELTA_LIB', "liblsdelta.so"), use_errno=True).lsdelta
#TODO: the return value is now mpz_t instead of long long!
#_lsdelta.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_longlong)]
#_lsdelta.restype = ctypes.c_char

def lsdelta(a :Union[str,bytes], b :Union[str,bytes]) -> int:
    raise NotImplementedError()  # pragma: no cover
    #delta = ctypes.c_longlong()
    #rv :bytes = _lsdelta(
    #    a.encode('UTF-8') if isinstance(a, str) else a,
    #    b.encode('UTF-8') if isinstance(b, str) else b,
    #    delta )
    #if ord(rv):
    #    raise RuntimeError(os.strerror(ctypes.get_errno()))
    #return delta.value
