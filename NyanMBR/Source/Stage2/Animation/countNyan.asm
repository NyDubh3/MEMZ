nyanTimeString: db "You've Nyaned for 00000.0 seconds!"
nyanTimeStringLen: equ $-nyanTimeString
nyanTimeVideoStart: equ 3840

nyanTimeBin dw 0

countNyan:
	; Set the extra segment to video memory
	push es
	push 0xb800
	pop es
	
	mov di, 0

	inc word [nyanTimeBin]
	
	mov bx, nyanTimeStringLen*2
	
	.loop:
		sub bx, 2
		
		mov al, [es:nyanTimeVideoStart+bx]
		cmp al, '0'
		jb .next
		
		cmp al, '9'
		ja .next
		
		inc al
		mov [es:nyanTimeVideoStart+bx], al
		
		cmp al, '9'+1
		jne .end
		
		mov al, '0'
		mov [es:nyanTimeVideoStart+bx], al
		
		.next:
		cmp bx, 0
		jne .loop
	
	.end:
	    pop es
	    ret
