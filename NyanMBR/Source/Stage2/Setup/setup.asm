; Set video mode
mov ax, 0x0003
int 10h

; Disable screen blinking (which requires EGA)
mov ax, 0x1003
mov bl, 0
int 10h

; Setup the main timer
%include "Setup/setupTimer.asm"

; Setup the interrupts
%include "Setup/setupInterrupts.asm"

; Setup the PC speaker
%include "Setup/setupSpeaker.asm"

; Setup the screen and Nyan Counter
call initDrawing
