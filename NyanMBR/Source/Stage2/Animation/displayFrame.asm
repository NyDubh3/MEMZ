frameIndex dw 0

frameSize:  equ (80*50) / 2 ; Raw binary size of a frame
lastFrame:  equ special

displayFrame:
	; Set the extra segment to video memory
	push es
	push 0xb800
	pop es
	
	mov di, 0

	mov si, [frameIndex]

	cmp word [soundIndex], lastIntroNote
	ja .normalFrame
	jne .introFrame

	; Reset the frame index when the intro is done
	mov si, frames
	
	; Revert the message characters
	mov di, 0
	mov cx, messageLength
	mov ax, 0x00DC
	rep stosw
	
	jmp .normalFrame

	; Intro Frame
	.introFrame:
		call drawIntroFrame
		jmp .end

	; Normal Animation Frame
	.normalFrame:
		call drawNormalFrame
	
	; Reset frame index when the last frame has been reached
	cmp word [frameIndex], lastFrame
	jb .end
	mov word [frameIndex], frames

	.end:
	    pop es
	    ret

%include "Animation/Image/initDrawing.asm"
%include "Animation/Image/drawIntroFrame.asm"
%include "Animation/Image/drawNormalFrame.asm"
