# MEMZ
A trojan made for Danooct1's User Made Malware Series.

I am not responsible for any damage made using this trojan.

The build scripts and converters for the MBR payload are very noobish. You will need some things to run these:

 - Python 2.7 for Windows and [this](https://github.com/vishnubob/python-midi) library.
 - git for Windows or some other linux tools for windows like MinGW
 - NASM in your PATH (I just copyed the executable into C:\Windows\ because I was too lazy to add it to the PATH manually)
 - The [Nyan Cat midi file](http://morganatmacsd.tumblr.com/post/7275362575/midi-file-for-nyan-cat-song)
 - Something to convert binary files into C arrays, to embed the MBR payload into the trojan (I used the export function of the HxD hex editor, you will only need that if you changed and recompiled the MBR payload)
 - qemu for Windows (additional, if you want to test the payload)

Even if you have all of this I don't think you get this to compile because my scripts suck, as I said earlier.
 
If you want to use the exe to batch converter, don't forget to append -zip to it's command line, because this gives way smaller file sizes and also works on Windows XP.

You'll also need Visual Studio 2015 to build the actual Trojan. Make sure you selected the Release configuration, because Debug is not configured properly.

The ASM code may look very noobish, but this is my first big thing written in assembly.

If you want to know what the trojan does or download the binaries, look at [my thread on danooct1's forum](http://danooct1.com/forum/viewtopic.php?f=12&t=470) (registration required)
