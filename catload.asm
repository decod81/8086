; turns exe-file into a com-file (within reason, works with alleycat.exe)
; 3e80bb5c82acd6b6f7cf6d414b745a8e *alleycat.exe

cpu 8086
org 100h

	mov	ax, cs
	add	ax, (0x100+512)/16

	mov	bx, [exe_header + 24]		; relocation table offset
	add	bx, exe_header			; relocation table location
	mov	cx, [exe_header + 6]		; number of relocations
next_relocation:
	mov	dx, [bx + 2]			; relocation segment
	add	dx, ax
	mov	es, dx
	mov	dx, [bx + 0]			; relocation offset
	mov	di, dx
	mov	dx, [es:di]			; load address
	add	dx, ax
	mov	[es:di], dx
	add	bx, 4
	dec	cx
	cmp	cx, 0
	jne	next_relocation

	mov	ax, cs
	add	ax, [exe_header + 14]		; initial SS
	add	ax, (512+0x100)/16		; skip stuff
	mov	ss, ax
	mov	sp, [exe_header + 16]		; initial SP

	mov	ax, cs
	add	ax, [exe_header + 22]		; initial CS
	add	ax, (512+0x100)/16		; skip stuff
	push	ax
	mov	ax, [exe_header + 20]		; initial IP
	push	ax
	retf

	times 512 - ($-$$) db 0

program:
	incbin "alleycat.exe",512

exe_header:
	incbin "alleycat.exe",0,512
