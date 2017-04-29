%macro startInterrupt 0
	pusha ; Save all registers
%endmacro

%macro finishInterrupt 0
	; Acknowledge Interrupt
	mov al, 0x20
	out 0x20, al
	
	popa ; Restore all registers
	iret ; Return from the interrupt
%endmacro

%macro setupInterrupt 2
	; Set the right segments
	mov bx, 0x0000
	mov es, bx
	
	; Register the handler
	mov word [es:(%1+8)*4], %2  ; Interrupt Handler
	mov word [es:(%1+8)*4+2], 0 ; Segment 0x0000
%endmacro

%macro setVideoMemory 0
	; Set the extra segment to video memory
	mov cx, 0xb800
	mov es, cx
%endmacro
