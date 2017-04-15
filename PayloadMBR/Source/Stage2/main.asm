; This file contains the actual animation
use16
org 0x8000

%include "setup.asm"
%include "macros.asm"

timerHandler:
	pusha
	
	checkTimer [cs:frameTickCounter], 10, "displayframe.asm"
	checkTimer [cs:noteTickCounter],  10, "playnote.asm"
	
	; Acknowledge Interrupt
	mov al, 0x20
	out 0x20, al
	
	popa
	iret

frameTickCounter db 0
noteTickCounter  db 0

frameIndex dw 0

soundIndex dw song
soundWait  db 0

; Data

image:       incbin "../../Build/image.bin"
imageLength: equ $-image

song:        incbin "../../Build/song.bin"
songLength:  equ $-song

message:       db "Your computer has been trashed by the MEMZ trojan. Now enjoy the Nyan Cat..."
messageLength: equ $-message