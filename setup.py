try:
    from skbuild import setup
except ImportError:
    import sys
    print(
        "Please update pip, you need pip 10 or greater,\n"
        " or you need to install the PEP 518 requirements in pyproject.toml yourself",
        file=sys.stderr,
    )
    raise

from setuptools import find_packages

setup(
    name='basegraph',
    version='0.1.0',
    author='Simon Lizotte',
    description='A light-weight, efficient and expressive graph library.',
    license="GPLv3",
    zip_safe=False,
    packages=["basegraph"],
    cmake_args=["-DBUILD_BINDINGS=ON"],
    include_package_data=True,
    exclude_package_data={'': ["__pycache__"]},
    extras_require={"test": ["pytest"]},
    python_requires=">=3.6",
)
