

@echo off

@REM компиляция и запуск под windows

echo compile project...

g++ ./head/*.h ./src/*.cpp main.cpp -o aco

if %errorlevel% neq 0 (
    echo compile error!
    pause
    exit /b 1
)

echo "1.txt" file
type .\input\1.txt


.\aco.exe config.ic

uv run ./antpath.py ./output/output.csv
uv run ./main.py ./output/output.csv
