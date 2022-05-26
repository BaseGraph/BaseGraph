import os, sys
from argparse import ArgumentParser
import yaml


parser = ArgumentParser()
parser.add_argument("modules", nargs="?", help="modules to install")
args = parser.parse_args()

with open("extensions.yaml", "r") as file_stream:
    modules_info = yaml.safe_load(file_stream)


modules_install_path = "modules"


def verify_requirements(module):
    pass


def install_module(module):
    os.system(f"git clone {module['url']}")
    os.system(f"pip install --target='../basegraph' .")


if __name__ == "__main__":
    if os.path.isdir(modules_install_path):
        os.mkdir(modules_install_path)
    os.chdir(modules_install_path)

    for module in ["metrics"]:
        verify_requirements(module)
        install_module(module)
