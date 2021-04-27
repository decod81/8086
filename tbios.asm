	cpu	8086
	org	0x100

	mov	di, 0
	mov	es, di
	mov	cx, 256
blints:
	mov	ax, blankint
	stosw
	mov	ax, cs
	stosw
	loop	blints

	mov	word [es:0x09*4], int09
	mov	word [es:0x10*4], int10
	mov	word [es:0x12*4], int12
	mov	word [es:0x13*4], int13
	mov	word [es:0x16*4], int16

	mov	ax, 0
	mov	es, ax
	mov	ds, ax
	mov	ax, 0x0201
	mov	bx, 0x7c00
	mov	cx, 1
	mov	dx, 0
	int	0x13

	jmp	0:0x7c00

int09:
	push	ax
	push	es
	mov	ax, 0x40
	mov	es, ax
	in	al, 0x60
	mov	byte [es:0x1e], al
	pop	es
	pop	ax
blankint:
	iret

int10:
	cmp	ah, 0x0e
	jne	int10_skip
	db	0x0f, 0
int10_skip:
	iret

int12:
	mov	ax, 640
	iret

int13:
	cmp	ah, 2
	je	int13_read_disk
	cmp	ah, 3
	je	int13_write_disk
	iret
int13_read_disk:
	db	0x0f, 2
	iret
int13_write_disk:
	db	0x0f, 3
	iret

int16:
	push	ds
	push	bx
	mov	bx, 0x40
	mov	ds, bx
	or	ah, ah
	jz	int16_read
	dec	ah
	jz	int16_wait
	xor	ax, ax
	pop	bx
	pop	ds
	iret
int16_read:
	cli
	mov	al, [ds:0x1e]
	cmp	al, 0
	sti
	je	int16_read
	mov	byte [ds:0x1e], 0
	pop	bx
	pop	ds
	iret
int16_wait:
	cli
	mov	al, [ds:0x1e]
	cmp	al, 0
	sti
	pop	bx
	pop	ds
	retf	2

	times	0xfff0-($-$$) db 0xff

	jmp	0xf000:0x100
