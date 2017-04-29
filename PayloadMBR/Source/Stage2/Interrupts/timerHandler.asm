%macro checkTimer 3
	inc byte %1
	cmp byte %1, %2
	jne %%checkNext

	mov byte %1, 0
	
	%include %3
	
	%%checkNext:
%endmacro

timerHandler:
	startInterrupt
	
	checkTimer [cs:frameTickCounter],  8, "Animation/displayFrame.asm"
	checkTimer [cs:noteTickCounter],  12, "Animation/playNote.asm"
	checkTimer [cs:nyanTickCounter],  10, "Animation/countNyan.asm"
	
	finishInterrupt