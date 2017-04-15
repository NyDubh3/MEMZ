@echo off
set PATH=%PATH%;C:\Program Files\qemu
qemu-system-i386 -s -soundhw pcspk disk.img