from typing import Union

# could use str|collections.abc.Buffer on Python 3.12+
def lsdelta(  # pragma: no cover
        a :Union[str, bytes, bytearray, memoryview],          # pylint: disable=unused-argument
        b :Union[str, bytes, bytearray, memoryview]) -> int:  # pylint: disable=unused-argument
    ...
