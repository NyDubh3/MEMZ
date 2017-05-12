use16
org 0x7c00

;setup cpu

;correct cs
jmp 0x0000:correct_cs
correct_cs:

; setup stack properly
cli
xor ax, ax
mov ss, ax
mov sp, 0x7BF0
sti

%include "decompress.asm" ; Decompress Code & Data

; Prepare the CPU segments

mov ax, 0x2000
mov ds, ax
mov es, ax

jmp 0x2000:0x0000         ; Jump to the decompressed Data, booting the actual "Kernel"

; Boot sector signature
times 510 - ($ - $$) db 0
dw 0xAA55

; Include the compressed data
comp:     incbin "../../Build/stage2-compressed.bin" ; Hardcoded build dir :(
compsize: equ $-comp

; Align it to sectors
;align 512
times 4096 - ($ - $$) db 0
