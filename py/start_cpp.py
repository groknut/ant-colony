import subprocess as sp
import os
from pathlib import Path
import sys
from config import timeit

@timeit
def compile_cpp(output_file):

    exe_path = str(Path(output_file).with_suffix('.exe'))  if os.name == "nt" else str(Path(output_file))

    if not os.path.exists(exe_path):
        sp.run(f"g++ -Ilibs/cfig  main.cpp src/*.cpp libs/cfig/cfig.cpp -o {output_file}", shell=True)

@timeit
def start_cpp(output_file):
    exe_path = str(Path(output_file).with_suffix('.exe'))  if os.name == "nt" else str(Path(output_file))
    sp.run(
        f"{output_file}", shell=True
    )

def main():
    if len(sys.argv) != 2:
        print(f"""
Usage: python {sys.argv[0]} <output_file>
Example: python {sys.argv[0]} main
        """)
        sys.exit(0)
    else:
        compile_cpp(sys.argv[1])
        start_cpp(sys.argv[1])
        sys.exit(0)

if __name__ == "__main__":
    main()
