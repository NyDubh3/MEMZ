nyanTimeStart: db "You Nyaned for "
nyanTime: db "000000.0"
nyanTimeLen equ $-nyanTime
nyanTimeEnd: db " seconds!"
nyanTimeLenFull equ $-nyanTimeStart

nyanTimeBin dw 0

countNyan:
	inc word [cs:nyanTimeBin]
	
	mov bx, nyanTimeLen
	
	.loop:
		dec bx
		
		mov al, [cs:nyanTime+bx]
		cmp al, '0'
		jb .next
		
		cmp al, '9'
		ja .next
		
		inc al
		mov [cs:nyanTime+bx], al
		
		cmp al, '9'+1
		jne .end
		
		mov al, '0'
		mov [cs:nyanTime+bx], al
		
		.next:
		cmp bx, 0
		jne .loop
	
	.end: ret