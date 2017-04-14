; This file contains the actual animation
use16
org 0x8000

%include "setup.asm"

timerHandler:
	pusha
	
	inc word [cs:frameTickCounter]
	
	checkNextFrame:
		cmp word [cs:frameTickCounter], 10
		jne checkNextNote

		mov word [cs:frameTickCounter], 0
		
		%include "displayframe.asm"
	
	checkNextNote:
		; Do the same for sound later
		
	
	; Acknowledge Interrupt
	mov al, 0x20
	out 0x20, al
	
	popa
	iret

frameTickCounter dw 0

frameIndex dw 0

align 0x10
xdata: equ 0x800+(($-$$)>>4)
data: incbin "../../Build/data.bin"