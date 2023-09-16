@echo off
set /a count=0
for %%x in (*) do set /a count+=1
set /a count=count-3
for /l %%i in (0 2 %count%) do del f%%i.dat
