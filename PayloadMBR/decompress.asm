mov bx, daddr
mov es, bx
mov ds, bx

; Read from disk
mov ax, 0x0204
mov cx, 0x0002
mov dh, 0
mov bx, compressed
int 13h

xor ax, ax
mov bx, ax
mov cx, ax
mov dx, ax

mov si, compressed
mov di, image

readcommand:
	lodsb
	
	cmp si, compressed+compsize
	jae startanimation
	
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
	
	mov dx, si
	mov si, bx
	add si, image
	mov cl, al
	
	oldnextbyte:
		lodsb
		stosb
		
		dec cl
		cmp cl, 0
		jne oldnextbyte
		
		mov si, dx
		jmp readcommand
