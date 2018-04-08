@echo off
title Compiling...
cls
make
pause
cls
title Executing 3DSX...
output\3dJS.3dsx
copy output\3dJS.3dsx D:\3ds\3dJS.3dsx
copy output\3dJS.cia D:\_\3dJS.cia
pause
exit