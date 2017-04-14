frameCount: equ 12          ; Don't hardcode that later
frameSize:  equ (80*50) / 2 ; Raw binary size of a frame

; Set base address for video memory
mov cx, 0xb800
mov es, cx

; Set base address for input data
mov cx, xdata
mov ds, cx

mov si, [cs:frameIndex]

; Display the frame
mov ah, 220 ; Save the character used
mov di, 1   ; Offset one byte

mov cx, frameSize
.draw:
	lodsb
	stosw
loop .draw

cmp si, frameCount*frameSize
jne .end

mov si, 0

.end:
mov [cs:frameIndex], si