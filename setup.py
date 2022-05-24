import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext, ParallelCompile


__version__ = '1.0.0'


ext_modules = [
    Pybind11Extension(
        'basegraph',
        include_dirs=["include"],
        sources= glob.glob("pybind_wrapper/**/*.cpp", recursive=True)
                    + glob.glob("src/**/*.cpp", recursive=True),
        language='c++'
    ),
]

setup(
    name='basegraph',
    version=__version__,
    author='Simon Lizotte',
    description='A light-weight, efficient and expressive graph library.',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.3'],
    setup_requires=['pybind11>=2.3'],
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
)

# Optional multithreaded build
ParallelCompile("N_JOBS", default=0, max=4).install()
