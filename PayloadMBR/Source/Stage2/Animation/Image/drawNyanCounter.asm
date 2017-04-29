drawNyanCounter:
	mov di, 80*48
	mov si, nyanTimeStart
	mov cx, nyanTimeLenFull
	mov bx, 0

	.loop:
		lodsb
		mov ah, 0x1f
		stosw
		
		loop .loop
	
	.end: ret