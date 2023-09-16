@echo off
for /l %%i in (0 2 %1) do fsutil file createnew f%%i.dat 512
for /l %%i in (1 2 %1) do fsutil file createnew f%%i.dat 1024
for /l %%i in (0 1 %1) do echo %%i > f%%i.dat
for /l %%i in (0 1 100) do ( for /l %%i in (0 1 %1) do echo %%i >> f%%i.dat )
