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
		mov di, 1 ; Offset one byte
		
		mov cx, frameSize
		.draw:
			lodsb
			stosb
			inc di
		loop .draw
		
		mov [cs:frameIndex], si
		
	.end: ret