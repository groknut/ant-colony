

@echo off

@REM запуск под windows

echo run project...

echo "1.txt" file
type .\input\1.txt

.\aco.exe config.ic

uv run ./main.py ./output/output.csv

