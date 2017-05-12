%macro onKey 2
	cmp al, %1
	jne %%notPressed
	
	call %2
	
	%%notPressed:
%endmacro

keyboardHandler:
	startInterrupt
	
	in al, 60h ; Read keyboard state
	
	onKey 0x1F, speedUp ; Speed up the main timer when S is pressed
	
	finishInterrupt
