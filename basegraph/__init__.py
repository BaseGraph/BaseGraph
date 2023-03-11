import basegraph._core as core

import os
package_directory = os.path.dirname(os.path.realpath(__file__))
for filename in os.listdir(package_directory):
    if os.path.isdir(os.path.join(package_directory, filename)) \
            and os.path.isfile(os.path.join(package_directory, filename, "__init__.py")):
        exec(f"import basegraph.{filename} as {filename}")
    del filename
del os, package_directory
