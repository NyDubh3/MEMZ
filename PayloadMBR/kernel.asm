use16
org 0x7c00

%include "decompress.asm" ; Include decompressor part

%macro sleep 2
	; Use BIOS interrupt to sleep
	push dx
	mov ah, 86h
	mov cx, %1
	mov dx, %2
	int 15h
	pop dx
%endmacro

%macro beepfreq 0
	out 42h, al
	mov al, ah
	out 42h, al
%endmacro

%macro beepon 0
	in al, 61h
	or al, 00000011b
	out 61h, al
%endmacro

%macro beepoff 0
	in al, 61h
	and al, 11111100b
	out 61h, al
%endmacro

startanimation:
	; Init PC speaker
	mov al, 182
	out 43h, al
	
	; Remove blinking
	mov ax, 1003h
	mov bl, 0
	int 10h

	mov di, 0

	mov dx, image+24000
	
	mov cx, 0xb800 ; Set base address for video memory
	mov es, cx
	
	; Clear screen
	mov ax, 0
	mov cx, 2000
	rep stosw
	
	mov si, image+24000+476
	mov di, 0
	
	beepon
	mov bl, 1
	
	startmsg:
		sleep 0x0, 0x6000
		
		cmp si, image+24000+476+msglen
		jge note
		
		lodsb
		mov ah, 0xf0
		stosw
		
		note:
			dec bl
			cmp bl, 0
			jne startmsg
			
			push si
			mov si, dx
			
			lodsw
			mov cx, ax
			and ah, 0b00011111
			
			beepfreq
			
			shr ch, 5
			shl ch, 2
			mov bl, ch
			
			mov dx, si
			
			pop si
			
			cmp dx, image+24000+26*2
			jne startmsg
		
	; Set image address
	mov si, image
	mov di, 0
	
	mov ax, daddr
	mov ds, ax
	
	mov ax, 0xb800
	mov es, ax
	
	dec bl
	jmp transition
	
	wrimg:
		; Write character
		mov al, 220
		stosb
		
		; Write attributes
		lodsb
		stosb
		
		; Check if animation is done
		cmp si, image+24000
		je repeat
	
		; Check if the next frame is reached
		cmp di, 4000
		je nextframe
	
		; Repeat the loop
		jmp wrimg
	
	nextframe:
		sleep 0x1, 0x6000 ; Sleep some time
		
		transition:
		mov di, 0         ; Reset video memory address
		
		cmp dx, image+24000+476
		jne nextnote
		
		mov dx, image+24000+26*2 ; Loop song
		
		nextnote:
			dec bl
			cmp bl, 0
			jne wrimg
			
			push si
			mov si, dx
			
			lodsw
			mov cx, ax
			and ah, 0b00011111
			
			beepfreq
			
			shr ch, 5
			mov bl, ch
			
			mov dx, si
			
			pop si
			jmp wrimg         ; Go back
	
	repeat:
		mov si, image
		jmp nextframe
	
daddr: equ 0x07e0
compressed: equ 0x0000
image: equ 0x4000
msglen: equ 76

times 510 - ($ - $$) db 0
dw 0xAA55 ; Boot sector signature

comp: incbin "Data/compressed.bin"
compsize: equ $-comp

times 4*1024 - ($ - $$) db 0