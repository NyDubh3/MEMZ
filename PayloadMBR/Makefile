
IMAGES = $(wildcard Data/Image/frames/*.png)
MUSIC  = Data/Song/nyan.mid

all: disk.img

image.bin: $(IMAGES)
	python2 Data/Image/png2bin.py `echo $(IMAGES) | sort -h` $@

song.bin: $(MUSIC)
	python2 Data/Song/midi2bin.py $< $@

data.o: song.bin image.bin
	cat image.bin song.bin Data/Other/message.txt > data.o

Data/compressed.bin: data.o Data/Compressor/compress.c
	cc Data/Compressor/compress.c -o Data/Compressor/compress
	Data/Compressor/compress data.o Data/compressed.bin


#comp.o: decompress.asm
#	nasm decompress.asm -o comp.o -f aout

kernel.o: kernel.asm decompress.asm Data/compressed.bin
	nasm kernel.asm -o kernel.o -f aout
	
disk.img: kernel.o linker.ld Data/compressed.bin
	ld -o disk.img -T linker.ld kernel.o -m elf_i386
	
test: disk.img
	qemu-system-i386 -s -soundhw pcspk -fda disk.img
	
.PHONY: test all
