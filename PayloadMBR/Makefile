
CC      = cc
PY      = python2
NASM    = nasm
LD      = ld
QEMU    = qemu-system-i386

CCFLAGS = 
ASFLAGS = -f aout
LDFLAGS = -T linker.ld -m elf_i386

IMAGES  = $(sort $(wildcard Data/Image/frames/*.png))
MUSIC   = Data/Song/nyan.mid

all: disk.img dir

dir:
	mkdir -p build 

build/image.bin: $(IMAGES)
	$(PY) Data/Image/png2bin.py $(IMAGES) $@

build/song.bin: $(MUSIC)
	$(PY) Data/Song/midi2bin.py $< $@

build/data.o: build/song.bin build/image.bin
	cat build/image.bin build/song.bin Data/Other/message.txt > build/data.o

Data/compressed.bin: build/data.o Data/Compressor/compress.c
	$(CC) Data/Compressor/compress.c -o Data/Compressor/compress $(CCFLAGS)
	Data/Compressor/compress build/data.o Data/compressed.bin

build/kernel.o: kernel.asm decompress.asm Data/compressed.bin
	$(NASM) kernel.asm -o build/kernel.o $(ASFLAGS)
	
disk.img: build/kernel.o linker.ld Data/compressed.bin
	$(LD) -o disk.img build/kernel.o $(LDFLAGS)
	
test: disk.img
	$(QEMU) -s -soundhw pcspk -fda disk.img

clean:
	rm build/* Data/Compressor/compress Data/compressed.bin	

.PHONY: test all

