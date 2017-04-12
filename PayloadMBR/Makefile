
IMAGES = $(wildcard Data/Image/frames/*.png)
MUSIC  = Data/Song/nyan.mid

all: disk.img

build/image.bin: $(IMAGES)
	python2 Data/Image/png2bin.py `echo $(IMAGES) | sort -h` $@

build/song.bin: $(MUSIC)
	python2 Data/Song/midi2bin.py $< $@

build/data.o: build/song.bin build/image.bin
	cat build/image.bin build/song.bin Data/Other/message.txt > build/data.o

Data/compressed.bin: build/data.o Data/Compressor/compress.c
	cc Data/Compressor/compress.c -o Data/Compressor/compress
	Data/Compressor/compress build/data.o Data/compressed.bin


#comp.o: decompress.asm
#	nasm decompress.asm -o comp.o -f aout

build/kernel.o: kernel.asm decompress.asm Data/compressed.bin
	nasm kernel.asm -o build/kernel.o -f aout
	
disk.img: build/kernel.o linker.ld Data/compressed.bin
	ld -o disk.img -T linker.ld build/kernel.o -m elf_i386
	
test: disk.img
	qemu-system-i386 -s -soundhw pcspk -fda disk.img

clean:
	rm build/* Data/Compressor/compress Data/compressed.bin	

.PHONY: test all

