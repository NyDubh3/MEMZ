%macro checkTimer 3
	inc byte %1
	cmp byte %1, %2
	jne %%checkNext

	mov byte %1, 0
	
	%include %3
	
	%%checkNext:
%endmacro