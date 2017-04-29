cli ; Disable Interrupts

; Setup the timer interrupt handler
setupInterrupt 0, timerHandler

; Setup the keyboard interrupt handler
setupInterrupt 1, keyboardHandler

sti ; Enable Interrupts again