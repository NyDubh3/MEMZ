drawIntroFrame:
    push es
    push 0xb800
    pop es
    
	; Increase the frame tick counter to make the intro run faster
	mov byte [frameTickCounter], 5

	; Check if message is already fully displayed
	cmp si, messageLength
	jae .end

	mov di, si
	imul di, 2

	mov byte al, [si+message]

	mov byte [es:di], al
	mov byte [es:di+1], 0xf0

	inc si
	mov [frameIndex], si
	
	.end:
	    pop es
	    ret
