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
	push ds
	push 0x0000
	pop ds
	
	; Register the handler
	mov word [(%1+8)*4], %2  ; Interrupt Handler
	mov word [(%1+8)*4+2], 0x2000 ; Segment 0x2000
	
	pop ds
%endmacro
