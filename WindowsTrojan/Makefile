BUILDDIR = Build

CC = i686-w64-mingw32-gcc

CCFLAGS = -nostdlib -nodefaultlibs -std=c99 -Wl,-e_start -v -s
LIBS    = -luser32 -lkernel32 -lgdi32 -lwinmm -lcomctl32 -ladvapi32 -lshell32

CFILES = $(shell find Source -name *.c -type f)

all: $(BUILDDIR)/MEMZ.exe

dir: $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/MEMZ.exe: $(CFILES) dir
	$(CC) $(CCFLAGS) $(CFILES) $(LIBS) -o $(BUILDDIR)/MEMZ.exe
	
clean:
	rm -r $(BUILDDIR)

.PHONY: clean all dir