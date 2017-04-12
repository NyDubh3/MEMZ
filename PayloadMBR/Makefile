
IMAGES = $(wildcard Data/Image/frames/*.png)
MUSIC  = Data/Song/nyan.mid

all: disk.img

image.bin: $(IMAGES)
	python2 Data/Image/png2bin.py $< $@

song.bin: $(MUSIC)
	python2 Data/Song/midi2bin.py $< $@

data.o: song.bin image.bin
	cat image.bin song.bin Data/Other/message.txt > data.o

Data/compressed.bin: data.o
	cc Data/Compressor/compress.c -o Data/Compressor/compress
	Data/Compressor/compress data.o Data/compressed.bin

disk.o: kernel.asm decompress.asm Data/compressed.bin
	nasm -o disk.o kernel.asm
	
disk.img: disk.o linker.ld data.o
	ld -o disk.img -T linker.ld disk.o data.o
	
test: disk.img
	qemu-system-i386 -s -soundhw pcspk -fda disk.img
	
.PHONY: test
