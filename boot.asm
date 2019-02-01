ORG 0x7C00
CPU 8086

	mov	bx, 0x0200	; segment
	mov	es, bx
	mov	bx, 0x0100	; offset (address = 16*segment+offset)

	mov	ah, 0x02	; read
	mov	al, 1		; read 1 sector (512 bytes)
	mov	ch, 0		; cylinder
	mov	cl, 2		; sector
	mov	dh, 0		; head
	mov	dl, 0x80	; drive C
	int	0x13		; BIOS

	jmp	0x0200:0x0100

times 510 - ($-$$) db 0
dw 0xAA55
