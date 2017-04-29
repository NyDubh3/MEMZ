import midi, sys

pattern = midi.read_midifile(sys.argv[1])

def pitchconv(pitch):
	return int(round(1193180.0 / (2**((pitch-69)/12.0)*440), 0))

with open(sys.argv[2], "wb") as out:
	pitches = [pitchconv(event.pitch) for event in pattern[1] if isinstance(event, midi.NoteOnEvent)]
    
	b = 0
	d = 0
	
	t = 0
	o = -1
	for event in pattern[1]:
		if isinstance(event, midi.NoteOnEvent):
			if event.velocity == 0:
				d += int(round(event.tick/48.0, 0))
				p = pitchconv(event.pitch)
				out.write(chr(p & 0xff) + chr((d - 1) << 5 | p >> 8))
				b = 0
			else:
				d = int(round(event.tick/48.0, 0))
