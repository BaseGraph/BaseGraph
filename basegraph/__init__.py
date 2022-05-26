for module in ["_core", "_metrics", "_random"]:
    try:
        exec(f"from . import {module} as {module[1:]}")
    except ImportError:
        pass
