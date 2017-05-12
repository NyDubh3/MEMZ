start:
	; Dump compressed data to segment 1000:0000 instead
	mov bx, 0x1000
	mov es, bx
	
	; Read from disk
	mov ax, 0x0208
	mov cx, 0x0002
	mov dh, 0
	xor bx, bx      ; (ip: address 0)
	int 13h
	
	; Source segment DS (1000:0000)
	push es
	pop ds
	; Target 2000:0000 segment for uncompressed data
	mov ax, 0x2000
	mov es, ax
	
	xor ax, ax
	mov bx, ax
	mov cx, ax
	mov dx, ax
	mov di, ax	
	mov si, ax

readcommand:
	lodsb
	
	cmp si, compsize
	jae exit
	
	cmp al, 128
	jae newdata
	jmp olddata
	
newdata:
	and al, 127
	mov cl, al
	
	newnextbyte:
		lodsb
		stosb
		
		dec cl
		cmp cl, -1
		jne newnextbyte
		
		jmp readcommand
		
olddata:
	mov ah, al
	lodsb
	
	mov bx, ax
	lodsb
	
	push ds
	
    push 0x2000
    pop ds
	
	mov dx, si
	mov si, bx
	mov cl, al	
	
	oldnextbyte:
		lodsb
		stosb
		
		loop oldnextbyte
		
		mov si, dx
		pop ds
		jmp readcommand

exit:
