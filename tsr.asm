cpu 8086
org 100h

	mov	ah, 0x25	; set interrupt vector
	mov	al, 0x83	; interrupt number
	mov	dx, tsr
	int	0x21
	
	mov	ax, 0x3100	; terminate and stay resident
	mov	dx, 512/16	; memory size (times 16 bytes)
	int	0x21

tsr:
	mov	bx, [x]
	mov	al, 0x0c
	mov	ah, bh
	mov	dx, 0x03d4
	out	dx, ax
	mov	al, 0x0d
	mov	ah, bl
	mov	dx, 0x03d4
	out	dx, ax

	in	al, 0x60	; KEYBOARD

	cmp	al, 0x01	; ESC
	jne	skipesc
	iret
skipesc:	
	cmp	al, 0x48	; UP
	jne	skipup
	sub	[x], word 40
skipup:
	cmp	al, 0x4B	; LEFT
	jne	skipleft
	sub	[x], word 1
skipleft:
	cmp	al, 0x4D	; RIGHT
	jne	skipright
	add	[x], word 1
skipright:
	cmp	al, 0x50	; DOWN
	jne	skipdown
	add	[x], word 40
skipdown:

	mov	cx, 0
delay:
	inc	cx
	cmp	cx, 0xFFFF
	jne	delay

	jmp	tsr

x	dw	0
