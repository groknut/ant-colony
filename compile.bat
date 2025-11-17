

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

uv run ./main.py ./output/output.csv
