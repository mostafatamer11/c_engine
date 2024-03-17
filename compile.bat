@echo off
gcc *.c res\*.c res\*.dll -Iinclude -lopengl32 -o res\Game.exe
res\Game.exe
