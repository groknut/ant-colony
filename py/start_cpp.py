

import subprocess as sp
import os
from pathlib import Path
import sys
from config import timeit

@timeit
def compile_cpp():

    exe_path = "main.exe"  if os.name == "nt" else "main"

    if not os.path.exists(exe_path):
        sp.run(f"g++ -Ilibs/cfig  main.cpp src/*.cpp libs/cfig/cfig.cpp -o main", shell=True)

@timeit
def start_cpp():
    exe_path = "main.exe"  if os.name == "nt" else "main"
    sp.run(
        f"{exe_path}", shell=True
    )

def main():
    compile_cpp()
    start_cpp()

if __name__ == "__main__":
    main()
