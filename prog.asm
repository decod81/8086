ORG 0x100
CPU 186

	cli

	; pit frequency = 1193180 / ax
	mov	ax, 0xFFFF
	mov	dx, 0x40
	out	dx, al
	mov	al, ah
	out	dx, al

	; pit interrupt
	mov	ax, 0
	mov	es, ax
	mov	ax, cs
	mov	word [es:4*0x08+2], ax
	mov	word [es:4*0x08+0], pit

	; keyboard interrupt
	mov	ax, 0
	mov	es, ax
	mov	ax, cs
	mov	word [es:4*0x09+2], ax
	mov	word [es:4*0x09+0], keyb

	sti

	mov	cx, 0
mainloop:
	cmp	cx, 0
	jz	mainloop

	mov	ax, 0x13
	int	0x10

	cli

	mov	al, 0
	mov	dx, 0x3c8	; vga set palette
	out	dx, al
	mov	dx, 0x3c9
loop1:
	out	dx, al		; red = index
	out	dx, al		; green = index
	out	dx, al		; blue = index
	inc	al
	cmp	al, 64
	jne	loop1

	mov	bx, 0xa000
	mov	es, bx
	mov	di, 0
loop:
	mov	ax, di
	shr	al, 2
	stosb
	cmp	di, 320*200
	jne	loop
endloop:
	jmp	endloop

pit:
	mov	al, [value]

	mov	bx, 0xb800
	mov	es, bx
	mov	byte [es:202], al

	inc	al
	mov	byte [value], al

	mov	al, 0x20
	out	0x20, al	; ack pic

	iret
keyb:
	in	al, 0x60
	cmp	al, 1
	jne	skip
	mov	cx, 1
skip:
	mov	bx, 0xb800
	mov	es, bx
	mov	byte [es:206], al

	mov	al, 0x20
	out	0x20, al	; ack pic

	iret

value:
	db	0
