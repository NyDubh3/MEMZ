playNote:
setDataSection

lastIntroNote equ song+26*2
lastNote      equ message

mov si, [cs:soundIndex]

cmp si, lastNote
jb .nextNote

; Go back to the beginning
mov si, lastIntroNote

.nextNote:
dec byte [cs:soundWait]
cmp byte [cs:soundWait], -1
jne .end

lodsw
mov cx, ax
and ah, 00011111b

; Set the frequency
out 0x42, al
mov al, ah
out 0x42, al

shr ch, 5
mov [cs:soundWait], ch

mov [cs:soundIndex], si

.end: