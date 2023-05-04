import re
import os
import subprocess
import pathlib

from matplotlib import pyplot as plt
from matplotlib import patches

benchmark_name_regex = re.compile("Benchmark - (.+)")
benchmark_result_regex = re.compile("([^0-9\t]+)\t*:\t([\\d\\.]+)±([\\d\\.]+)")

file_directory = pathlib.Path(__file__).parent.resolve()
benchmark_output = {
    "python":
    subprocess.run(["python", "benchmark.py"],
                   stdout=subprocess.PIPE,
                   text=True),
    "cpp":
    subprocess.run([
        os.path.join(file_directory, "build", "benchmark") +
        (".exe" if os.name == "nt" else "")
    ],
                   stdout=subprocess.PIPE,
                   text=True)
}

color_map = {
    "BaseGraph": "#613F75",
    "igraph": "#F7A9A8",
    "Boost Graph": "#7D82B8",
    "graph-tool": "#E5C3D1",
    "NetworkX": "#EF798A",
}

for language, output in benchmark_output.items():
    benchmark_results = {}
    benchmark_name = None
    lib_names = set()
    for line in output.stdout.splitlines():
        regex_match = benchmark_name_regex.match(line)
        if regex_match:
            benchmark_name = regex_match[1]
            benchmark_results[benchmark_name] = {}
            continue

        regex_match = benchmark_result_regex.match(line)
        if regex_match:
            benchmark_results[benchmark_name][regex_match[1]] = (
                regex_match[2], regex_match[3])
            lib_names.add(regex_match[1])

    lib_number = max(map(len, benchmark_results.values()))

    fig, ax = plt.subplots()
    spacing = 2
    for i, (name, results) in enumerate(benchmark_results.items()):
        lib_averages = list(map(lambda x: float(x[0]), results.values()))
        lib_errors = list(map(lambda x: float(x[1]), results.values()))
        width = 1 / lib_number

        bin_start = [i * spacing + width * j for j in range(len(lib_averages))]
        ax.bar(
            bin_start,
            lib_averages,
            yerr=lib_errors,
            width=width,
            color=[color_map[lib_name] for lib_name in results],
            align="edge",
        )
    ax.set_xticks([i * spacing + .5 for i in range(len(benchmark_results))],
                  benchmark_results.keys())
    ax.set_ylabel("Time [ms]")
    ax.set_yscale("log")
    ax.legend([
        patches.Patch(facecolor=color_map[lib_name]) for lib_name in lib_names
    ], lib_names)
    fig.tight_layout()
plt.show()
