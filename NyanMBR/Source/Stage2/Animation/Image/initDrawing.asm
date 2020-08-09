initDrawing:

	; Set the extra segment to video memory
	push es
	push 0xb800
	pop es
	
	mov di, 0
	
	mov ax, 0x00DC
	mov cx, nyanTimeVideoStart/2
	rep stosw
	
	mov cx, nyanTimeStringLen
	mov si, nyanTimeString
	.initNyanTime:
		lodsb
		stosw
	loop .initNyanTime
	
	mov al, 0xDC
	mov cx, frameSize - nyanTimeVideoStart/2 - nyanTimeStringLen
	rep stosw
	
	pop es
		
	ret
