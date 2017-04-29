; Setup the PC speaker timer
mov al, 10110110b
out 0x43, al

; Set the default frequency
mov ax, 1193 ; ~1000 Hz
out 0x42, al
mov al, ah
out 0x42, al

; Enable the PC speaker
in al, 61h
or al, 00000011b
out 61h, al