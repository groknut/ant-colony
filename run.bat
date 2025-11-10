

@echo off

@REM запуск под windows

echo run project...

.\aco.exe config.ic

if "%~1" == "" (
	echo No args provided
	uv run ./main.py ./output/output.csv
) else (
	echo Argument provided: %~1
	uv run ./main.py "%~1" ./output/output.csv
)
