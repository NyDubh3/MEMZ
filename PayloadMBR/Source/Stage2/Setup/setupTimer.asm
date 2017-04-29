mov al, 00110100b
out 0x43, al

; Set the frequency
mov ax, 11932 ; ~100 Hz
out 0x40, al
mov al, ah
out 0x40, al