

@echo off

@REM запуск под windows

echo run project...

.\aco.exe config.ic

uv run ./main.py ./output/output.csv
