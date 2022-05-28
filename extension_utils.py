import os
import shutil

import yaml
import git


install_path = os.path.join("include", "BaseGraph", "extensions")
with open("extensions.yml", "r") as file_stream:
    _extensions_info = yaml.safe_load(file_stream)

_valid_extensions = {extension_info["name"]
                     for extension_info in _extensions_info}


class ExtensionNotFoundError(ModuleNotFoundError):
    pass

class ExtensionDuplicateError(ExtensionNotFoundError):
    pass

class RequirementError(Exception):
    pass


def is_valid_extension(extension_name):
    global _extensions_info, _valid_extensions
    return extension_name in _valid_extensions


def find_extension_info(extension_name):
    global _extensions_info
    extensions_with_name = list(filter(
            lambda x: x["name"]==extension_name, _extensions_info
        ))

    if len(extensions_with_name) == 0:
        raise ExtensionNotFoundError(f"Could not find \"{extension_name}\" in "
                                      "extensions.yml.")
    elif len(extensions_with_name) > 1:
        raise ExtensionDuplicateError(f"Could not resolve \"{extension_name}\" "
                        f"in extensions.yml: {len(extensions_with_name)} "
                         "extensions share the same name.")
    return extensions_with_name[0]


def verify_requirements(extension):
    pass


def install_extension(extension):
    global install_path
    extension_path = os.path.join(install_path, extension["name"])
    if os.path.isdir(extension_path):
        repository = None
        print(f"Extension \"{extension['name']}\" is already installed. "
               "Use \"update\" command to update it.")
        return

    repository = git.Repo.clone_from(extension["url"], extension_path)

    python_extension_path = os.path.join(extension_path, extension["name"])
    if os.path.isdir(python_extension_path):
        shutil.move(python_extension_path, "basegraph")
    elif os.path.isfile(python_extension_path + ".py"):
        shutil.move(python_extension_path+".py", "basegraph")


def get_extension_info(extension_name):
    repository = get_extension_repository(extension_name)
    if repository is None:
        return f"{extension_name:<17}Unknown version(no git)"

    tag = repository.tags if repository.tags else 'No tag'
    return f"{extension_name:<17}commit {str(repository.head.commit)[:8]}"\
           f"({tag})"


def update_extension(extension_name):
    repository = get_extension_repository(extension_name)
    if repository is None:
        print(f"Cannot update extension \"{extension_name}\": is not a git "
               "repository.")
        return

    if "origin" not in repository.remotes:
        print(f"Cannot update extension \"{extension_name}\": remote "
               "\"origin\" not found.")
        return

    previous_commit = repository.head.commit
    pull_info = repository.remotes["origin"].pull()
    if repository.head.commit == previous_commit:
        print(f"Extension \"{extension_name}\" is up-to-date.")
        return
    return pull_info


def get_extension_repository(extension_name):
    extension_install_path = os.path.join(install_path, extension_name, ".git")
    if not os.path.isdir(extension_install_path):
        return
    return git.Repo(extension_install_path)
