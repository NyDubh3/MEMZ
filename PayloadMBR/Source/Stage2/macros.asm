%macro checkTimer 3
	inc byte %1
	cmp byte %1, %2
	jne %%checkNext

	mov byte %1, 0
	
	%include %3
	
	%%checkNext:
%endmacro

%macro setDataSection 0
	; Set base address for input data
	mov cx, 0
	mov ds, cx
%endmacro