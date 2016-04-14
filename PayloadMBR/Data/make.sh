cd Image
py png2bin.py frames/*.png image.bin
cd ../Song
py midi2bin.py nyan.mid song.bin

cd ..

cat Image/image.bin Song/song.bin Other/message.txt > data.bin
Compressor/compress.exe data.bin compressed.bin