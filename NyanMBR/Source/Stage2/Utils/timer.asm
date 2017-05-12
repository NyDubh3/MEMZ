defaultClock equ 11932 ; ~100 Hz
currentClock dw defaultClock

; Updates the current timer value
setTimer:
	mov ax, [currentClock]
	out 0x40, al
	mov al, ah
	out 0x40, al
	
	ret
	
maxClock equ defaultClock/6
minClock equ defaultClock*3

; Speed increase is calculated using the following formula:
; currentClock = currentClock * clockPreMul / clockDiv
clockPreMul  equ 2
clockDiv     equ 3

; Speeds up the current timer
speedUp:
	mov ax, [currentClock]
	
	mov bx, clockPreMul
	mul bx
	
	mov bx, clockDiv
	div bx
	
	cmp ax, maxClock
	ja .resetTimer
	
	; If too fast, reset the timer to the minimum speed
	mov ax, minClock
	
	.resetTimer:
		mov [currentClock], ax
		call setTimer
	
	ret
