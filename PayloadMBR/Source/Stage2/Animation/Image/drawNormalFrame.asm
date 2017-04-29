drawNormalFrame:
	mov ax, [cs:nyanTimeBin]
	mov dx, 0
	mov bx, 10
	div bx
	
	cmp ax, 420
	jne .displayFrame
	
	; At 420 secs, show a special image
	mov si, special
	
	; Display the frame
	.displayFrame:
		mov ah, 220 ; Set the block character to use
		mov di, 1   ; Offset one byte
		
		mov cx, frameSize
		.draw:
			lodsb
			stosw
		loop .draw
		
		mov [cs:frameIndex], si
		
		call drawNyanCounter
		
	.end: ret