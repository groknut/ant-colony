

@echo off

@REM запуск под windows

echo run project...

echo "1.txt" file
type .\input\1.txt

.\aco.exe config.ic

uv run ./antpath.py ./output/output.csv
uv run ./main.py ./output/output.csv

