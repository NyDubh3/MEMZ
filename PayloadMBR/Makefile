BUILDDIR = build

CC       = cc
PY       = python2
NASM     = nasm
QEMU     = qemu-system-i386

CCFLAGS  = 
ASFLAGS  = 

IMAGES   = $(sort $(wildcard Data/Image/frames/*.png))
MUSIC    = Data/Song/nyan.mid

all: disk.img

dir: $(BUILDDIR)
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/image.bin: $(IMAGES)
	$(PY) Data/Image/png2bin.py $(IMAGES) $@

$(BUILDDIR)/song.bin: $(MUSIC)
	$(PY) Data/Song/midi2bin.py $(MUSIC) $@

$(BUILDDIR)/data.bin: $(BUILDDIR)/song.bin $(BUILDDIR)/image.bin
	cat $(BUILDDIR)/image.bin $(BUILDDIR)/song.bin Data/Other/message.txt > $(BUILDDIR)/data.bin

$(BUILDDIR)/compress: Data/Compressor/compress.c
	$(CC) Data/Compressor/compress.c -o $@ $(CCFLAGS)
	
$(BUILDDIR)/compressed.bin: $(BUILDDIR)/data.bin $(BUILDDIR)/compress
	$(BUILDDIR)/compress $(BUILDDIR)/data.bin $(BUILDDIR)/compressed.bin

disk.img: dir kernel.asm decompress.asm $(BUILDDIR)/compressed.bin
	$(NASM) kernel.asm -o $@ $(ASFLAGS)
	
test: disk.img
	$(QEMU) -s -soundhw pcspk -fda disk.img

clean:
	rm -r $(BUILDDIR)
	rm disk.img

.PHONY: test clean all dir