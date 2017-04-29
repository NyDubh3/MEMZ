drawIntroFrame:
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
	
	.end: ret