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

soundIndex dw frameCount*frameSize
soundWait  db 0

align 0x10 ; Align Data so it can fit into a section
xdata: equ 0x800+(($-$$)>>4) ; Workaround, might be changed later

; TODO Don't pack it together and hardcode
data: incbin "../../Build/data.bin" ; Include the actual data
songLength: equ 476
message: db "Your computer has been trashed by the MEMZ trojan. Now enjoy the Nyan Cat..."
msglen: equ $-message