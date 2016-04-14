@echo off
nasm -o disk.img kernel.asm
set PATH=%PATH%;C:\Program Files\qemu
qemu-system-i386 -s -soundhw pcspk -fda disk.img
pause