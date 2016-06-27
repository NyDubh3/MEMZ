# Very poorly coded exe (or other file) to batch converter.
#
# If you want to use it, don't forget to append -zip to the command line,
# because this is the best method for packing the file.

import sys, os, base64, zipfile

use_zip = "-zip" in sys.argv
for_xp = "-xp" in sys.argv
use_script = "-zip" in sys.argv or "-xp" in sys.argv

ZIP_NAME = "z.zip"
EXE_NAME = sys.argv[3]
BASE64_NAME = "x"
JS_NAME = "x.js"

def writeScript(script, path):
	out = ""
	i = 0
	
	for line in script.splitlines():
		if i == 0:
			out += 'echo %s>%s\r\n' % (batchescape(line), path)
		else:
			out += 'echo %s>>%s\r\n' % (batchescape(line), path)
		
		i+=1
		
	return out
	
def batchescape(s):
	chrs = '<>|"&'
	
	for c in chrs:
		s = s.replace(c, "^"+c)
		
	return s#.replace("%", "%%")

out = "@echo off\r\n\r\n"

fn = sys.argv[1]

if use_zip:
	with zipfile.ZipFile("temp.zip", "w") as z:
		z.write(sys.argv[1], EXE_NAME, zipfile.ZIP_DEFLATED)
	
	fn = "temp.zip"

ifile = open(fn, "rb")
inp = ifile.read()
ifile.close()

b64 = base64.encodestring(inp)
out += writeScript(b64, BASE64_NAME)

if use_zip:
	os.remove("temp.zip")
	
if use_script:
	out += "\r\n"
	
if use_zip:
	js = """f=new ActiveXObject("Scripting.FileSystemObject");i=f.getFile("%s").openAsTextStream();
x=new ActiveXObject("MSXml2.DOMDocument").createElement("Base64Data");x.dataType="bin.base64";
x.text=i.readAll();o=new ActiveXObject("ADODB.Stream");o.type=1;o.open();o.write(x.nodeTypedValue);
z=f.getAbsolutePathName("%s");o.saveToFile(z);s=new ActiveXObject("Shell.Application");
s.namespace(26).copyHere(s.namespace(z).items());o.close();i.close();""" % (BASE64_NAME, ZIP_NAME)

	out += writeScript(js, JS_NAME)
elif for_xp:
	js = """i=WScript.createObject("Scripting.FileSystemObject").getFile("%s").openAsTextStream();
x=WScript.createObject("MSXml2.DOMDocument").createElement("Base64Data");x.dataType="bin.base64";
x.text=i.readAll();o=WScript.createObject("ADODB.Stream");o.type=1;o.open();o.write(x.nodeTypedValue);
o.saveToFile("%s");o.close();i.close();""" % (BASE64_NAME, EXE_NAME)

	out += writeScript(js, JS_NAME)

out += "\r\n"

out += 'set v="%%appdata%%\\%s"\r\n' % EXE_NAME
out += "del %v% >NUL 2>NUL\r\n"

if use_script:
	out += "cscript %s >NUL 2>NUL\r\n" % JS_NAME
	out += "del %s >NUL 2>NUL\r\n" % JS_NAME
else:
	out += "certutil -decode %s %%v%% >NUL 2>NUL\r\n" % BASE64_NAME

if for_xp:
	out += "move %s %%v%% >NUL 2>NUL\r\n" % EXE_NAME
	
if use_zip:
	out += "del %s >NUL 2>NUL\r\n" % ZIP_NAME
	
out += "del %s >NUL 2>NUL\r\n" % BASE64_NAME
out += 'start "" %v%'

ofile = open(sys.argv[2], "wb")
ofile.write(out)
ofile.close()

print len(out), "characters."
