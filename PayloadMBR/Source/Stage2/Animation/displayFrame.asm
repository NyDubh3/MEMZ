frameIndex dw 0

displayFrame:

frameSize:  equ (80*50) / 2 ; Raw binary size of a frame
lastFrame:  equ special

; Set base address for video memory
mov cx, 0xb800
mov es, cx

; Set Data section
mov cx, 0
mov ds, cx

mov si, [cs:frameIndex]

cmp word [cs:soundIndex], lastIntroNote
ja .normalFrame
jne .introFrame

; Reset the frame index
mov si, frames
jmp .normalFrame

; Intro Frame
.introFrame:
	; Increase the frame tick counter to make the intro run faster
	mov byte [cs:frameTickCounter], 5
	
	; Check if message is already fully displayed
	cmp si, messageLength
	jae .end
	
	mov di, si
	imul di, 2
	
	mov byte al, [cs:si+message]
	
	mov byte [es:di], al
	mov byte [es:di+1], 0xf0
	
	inc si
	mov [cs:frameIndex], si
	
	jmp .end

; Normal Animation Frame
.normalFrame:
	mov ax, [cs:nyanTimeBin]
	mov dx, 0
	mov bx, 10
	div bx
	
	cmp ax, 420
	jne .actualNormalFrame
	
	mov si, special
	
	.actualNormalFrame:
	; Display the frame
	mov ah, 220 ; Save the character used
	mov di, 1   ; Offset one byte
	
	mov cx, frameSize
	.draw:
		lodsb
		stosw
	loop .draw
	
	mov [cs:frameIndex], si
	
	mov di, 80*48
	mov si, nyanTimeStart
	mov cx, nyanTimeLenFull
	mov bx, 0
	
	.loop:
		lodsb
		mov ah, 0x1f
		stosw
		
		loop .loop

	cmp word [cs:frameIndex], lastFrame
	jb .end

	mov word [cs:frameIndex], frames

.end: ret