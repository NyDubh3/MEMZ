import midi, sys

pattern = midi.read_midifile(sys.argv[1])
print pattern

def pitchconv(pitch):
	out = int(round(1193180.0 / (2**((pitch-69)/12.0)*440), 0))
	#print out
	return out

with open(sys.argv[2], "wb") as out:
	pitches = [pitchconv(event.pitch) for event in pattern[1] if isinstance(event, midi.NoteOnEvent)]
	minp = min(pitches)
	maxp = max(pitches)
	
	#assert maxp - minp < 32
	
	print minp, maxp
	
	b = 0
	d = 0
	
	t = 0
	o = -1
	for event in pattern[1]:
		if isinstance(event, midi.NoteOnEvent):
			if event.velocity == 0:
				#b = b | (int(round(event.tick/48.0, 0)-1) << 7)
				d += int(round(event.tick/48.0, 0))
				print d
				p = pitchconv(event.pitch)
				out.write(chr(p & 0xff) + chr(d << 5 | p >> 8))
				b = 0
			else:
				d = int(round(event.tick/48.0, 0))
				#print d
				#b = (d << 5) | (event.pitch-minp)