@echo off
make.bat
set PATH=%PATH%;C:\Program Files\qemu
qemu-system-i386 -s -soundhw pcspk -fda disk.img
pause
