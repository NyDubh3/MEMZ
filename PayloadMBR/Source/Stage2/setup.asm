; Make sure all registers are clean
xor ax, ax
xor bx, bx
xor cx, cx
xor dx, dx
xor si, si
xor di, di

; Set video mode
mov ax, 0x0003
int 10h

; Set base address for video memory
mov cx, 0xb800
mov es, cx

; Set base address for input data
mov cx, xdata
mov ds, cx

; Put the stack somewhere safe
mov sp, 0x2000

cli ; Disable Interrupts

; Setup the main timer
mov al, 00110100b
out 0x43, al
; Set the frequency
mov ax, 11932 ; ~100 Hz
out 0x40, al
mov al, ah
out 0x40, al

; Setup the interrupt handler
mov bx, 0x0000
mov es, bx
mov word [es:0x8*4], timerHandler  ; Timer Handler
mov word [es:0x8*4+2], 0           ; Segment 0x0000

sti ; Enable interrupts again

; Setup the PC speaker timer
mov al, 10110110b
out 0x43, al
; Set the frequency
mov ax, 1193 ; ~1000 Hz
out 0x42, al
mov al, ah
out 0x42, al

; Enable the PC speaker
in al, 61h
or al, 00000011b
out 61h, al

; Keep running only on interrupts
rip:
	hlt
jmp rip