displayFrame:

frameCount: equ 12          ; Don't hardcode that later
frameSize:  equ (80*50) / 2 ; Raw binary size of a frame

; Set base address for video memory
mov cx, 0xb800
mov es, cx

setDataSection

mov si, [cs:frameIndex]

cmp word [cs:soundIndex], lastIntroNote
ja .normalFrame
jne .introFrame

; Reset the frame index
mov si, 0
jmp .normalFrame

; Intro Frame
.introFrame:
	; Increase the frame tick counter to make the intro run faster
	mov byte [cs:frameTickCounter], 7
	
	; Check if message is already fully displayed
	cmp si, msglen
	jae .end
	
	mov di, si
	imul di, 2
	
	mov byte al, [cs:si+message]
	
	mov byte [es:di], al
	mov byte [es:di+1], 0xf0
	
	inc si
	
	jmp .end

; Normal Animation Frame
.normalFrame:
	; Display the frame
	mov ah, 220 ; Save the character used
	mov di, 1   ; Offset one byte

	mov cx, frameSize
	.draw:
		lodsb
		stosw
	loop .draw

	cmp si, frameCount*frameSize
	jne .end

	mov si, 0

.end:
mov [cs:frameIndex], si