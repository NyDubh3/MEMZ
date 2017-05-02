CC = i686-w64-mingw32-gcc
PYTHON = python2

CCFLAGS = -nostdlib -nodefaultlibs -std=c99 -Wl,-e_start -s -mwindows -m32 -municode -Os -w -march=i486
LIBS    = -luser32 -lkernel32 -lgdi32 -lwinmm -lcomctl32 -ladvapi32 -lshell32 -lpsapi

# Don't change the build directory, it is hardcoded on other places
BUILDDIR = Build

CFILES = $(shell find Source -name '*.c' -type f)
HFILES = $(shell find Source -name '*.h' -type f)

DATA = $(wildcard Data/*)


all: dir $(BUILDDIR)/MEMZ-Clean.bat $(BUILDDIR)/MEMZ-Destructive.bat

dir: $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	
$(BUILDDIR)/Data.c $(BUILDDIR)/Data.h: ../NyanMBR/disk.img $(DATA)
	$(PYTHON) Data/genData.py $(BUILDDIR)/Data.c $(BUILDDIR)/Data.h
	
$(BUILDDIR)/MEMZ-Clean.exe: $(CFILES) $(HFILES) $(BUILDDIR)/Data.c $(BUILDDIR)/Data.h
	echo "#define CLEAN" > $(BUILDDIR)/Mode.h
	$(CC) $(CCFLAGS) $(CFILES) $(BUILDDIR)/Data.c $(LIBS) -o $@
	
$(BUILDDIR)/MEMZ-Clean.bat: $(BUILDDIR)/MEMZ-Clean.exe Source/exe2bat.py
	$(PYTHON) Source/exe2bat.py $(BUILDDIR)/MEMZ-Clean.exe $@ MEMZ-Clean.exe -zip

$(BUILDDIR)/MEMZ-Destructive.exe: $(CFILES) $(HFILES) $(BUILDDIR)/Data.c $(BUILDDIR)/Data.h
	echo "#define DESTRUCTIVE" > $(BUILDDIR)/Mode.h
	$(CC) $(CCFLAGS) $(CFILES) $(BUILDDIR)/Data.c $(LIBS) -o $@
	
$(BUILDDIR)/MEMZ-Destructive.bat: $(BUILDDIR)/MEMZ-Destructive.exe Source/exe2bat.py
	$(PYTHON) Source/exe2bat.py $(BUILDDIR)/MEMZ-Destructive.exe $@ MEMZ-Destructive.exe -zip
	
clean:
	rm -r $(BUILDDIR)

clean-exe: dir $(BUILDDIR)/MEMZ-Clean.exe
clean-bat: dir $(BUILDDIR)/MEMZ-Clean.bat
destructive-exe: dir $(BUILDDIR)/MEMZ-Destructive.exe
destructive-bat: dir $(BUILDDIR)/MEMZ-Destructive.bat

.PHONY: clean all dir clean-exe clean-bat destructive-exe destructive-bat