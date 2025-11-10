

@echo off

@REM компиляция и запуск под windows

echo compile project...

g++ ./head/*.h ./src/*.cpp main.cpp -o aco

if %errorlevel% neq 0 (
    echo compile error!
    pause
    exit /b 1
)

.\aco.exe config.ic

if "%~1" == "" (
	echo No args provided
	uv run ./main.py ./output/output.csv
) else (
	echo Argument provided: %~1
	uv run ./main.py "%~1" ./output/output.csv
)
