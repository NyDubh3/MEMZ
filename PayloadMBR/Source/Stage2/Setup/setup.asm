; Make sure all registers are clean
xor ax, ax
xor bx, bx
xor cx, cx
xor dx, dx
xor si, si
xor di, di
; =================================

; Set video mode
mov ax, 0x0003
int 10h
; ==============

; Put the stack somewhere safe
mov sp, 0x2000

; Setup the main timer
%include "Setup/setupTimer.asm"

; Setup the interrupts
%include "Setup/setupInterrupts.asm"

; Setup the PC speaker
%include "Setup/setupSpeaker.asm"