import sys

with open(sys.argv[2], "w") as of:
	with open(sys.argv[1], "r") as inf:
		of.write("#include \"../Source/data.h\"\n")
		of.write("#ifndef CLEAN\n")
		
		of.write("const char STAGE1_MBR[] = {")
		b = inf.read(510).rstrip(chr(0))
		x = ", ".join([str(ord(c)) for c in b])
		of.write(x + "};\n")
		
		of.write("const char STAGE2_MBR[] = {")
		b = inf.read().rstrip(chr(0))
		x = ", ".join([str(ord(c)) for c in b])
		of.write(x + "};\n")
		
		of.write("const size_t STAGE1_MBR_LEN = sizeof(STAGE1_MBR);\n");
		of.write("const size_t STAGE2_MBR_LEN = sizeof(STAGE2_MBR);\n");
		
		of.write("#endif")