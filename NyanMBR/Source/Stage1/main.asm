use16
org 0x7c00

compressed:   equ 0x7e00
decompressed: equ 0x8E00

%include "decompress.asm" ; Decompress Code & Data
jmp decompressed          ; Jump to the decompressed Data, booting the actual Kernel

; Boot sector signature
times 510 - ($ - $$) db 0
dw 0xAA55

; Include the compressed data
comp:     incbin "../../Build/stage2-compressed.bin" ; Hardcoded build dir :(
compsize: equ $-comp

; Align it to sectors
;align 512
times 4096 - ($ - $$) db 0