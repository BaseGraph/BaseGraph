import os
import re
import importlib

regex = re.compile(f"(.+)\.cpython")
__all__ = [regex.match(f)[1]
           for f in os.listdir(os.path.dirname(__file__)) if regex.match(f)]
for module in __all__:
    importlib.import_module("."+module, package="basegraph")

del regex, module
del os, re, importlib
