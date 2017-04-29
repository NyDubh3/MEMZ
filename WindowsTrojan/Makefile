BUILDDIR = Build

CC = i686-w64-mingw32-gcc
PYTHON = python2

CCFLAGS = -nostdlib -nodefaultlibs -std=c99 -Wl,-e_start -s -mwindows -m32 -municode
LIBS    = -luser32 -lkernel32 -lgdi32 -lwinmm -lcomctl32 -ladvapi32 -lshell32 -lpsapi

CFILES = $(shell find Source -name *.c -type f)

all: $(BUILDDIR)/MEMZ.exe

dir: $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/MBR.c: ../NyanMBR/disk.img Source/genData.py
	$(PYTHON) Source/genData.py ../NyanMBR/disk.img $(BUILDDIR)/MBR.c
	
$(BUILDDIR)/MEMZ.exe: $(CFILES) dir $(BUILDDIR)/MBR.c
	$(CC) $(CCFLAGS) $(CFILES) $(BUILDDIR)/MBR.c $(LIBS) -o $@
	
clean:
	rm -r $(BUILDDIR)

.PHONY: clean all dir