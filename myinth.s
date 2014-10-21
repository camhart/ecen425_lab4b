; Generated by c86 (BYU-NASM) 5.1 (beta) from myinth.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
	ALIGN	2
resetHandler:
	; >>>>> Line:	6
	; >>>>> void resetHandler() { 
	jmp	L_myinth_1
L_myinth_2:
	; >>>>> Line:	7
	; >>>>> exit(0); 
	xor	al, al
	push	ax
	call	exit
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_myinth_1:
	push	bp
	mov	bp, sp
	jmp	L_myinth_2
L_myinth_7:
	DB	") IGNORED",0xA,0
L_myinth_6:
	DB	0xA,"KEYPRESS (",0
L_myinth_5:
	DB	0xA,"DELAY COMPLETE",0xA,0
L_myinth_4:
	DB	0xA,"DELAY KEY PRESSED",0xA,0
	ALIGN	2
keyHandler:
	; >>>>> Line:	10
	; >>>>> void keyHandler() { 
	jmp	L_myinth_8
L_myinth_9:
	; >>>>> Line:	13
	; >>>>> if(key == 'd') { 
	mov	al, byte [KeyBuffer]
	mov	byte [bp-1], al
	mov	word [bp-4], 0
	; >>>>> Line:	13
	; >>>>> if(key == 'd') { 
	cmp	byte [bp-1], 100
	jne	L_myinth_10
	; >>>>> Line:	14
	; >>>>> printString("\nDELAY KEY PRESSED\n"); 
	mov	ax, L_myinth_4
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	15
	; >>>>> while (c < 5000) 
	jmp	L_myinth_12
L_myinth_11:
	; >>>>> Line:	16
	; >>>>> c++; 
	inc	word [bp-4]
L_myinth_12:
	cmp	word [bp-4], 5000
	jl	L_myinth_11
L_myinth_13:
	; >>>>> Line:	17
	; >>>>> printString("\nDELAY COMPLETE\n"); 
	mov	ax, L_myinth_5
	push	ax
	call	printString
	add	sp, 2
	jmp	L_myinth_14
L_myinth_10:
	; >>>>> Line:	20
	; >>>>> printString("\nKEYPRESS ("); 
	mov	ax, L_myinth_6
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	21
	; >>>>> printChar(key); 
	push	word [bp-1]
	call	printChar
	add	sp, 2
	; >>>>> Line:	22
	; >>>>> printString(") IGNORED\n"); 
	mov	ax, L_myinth_7
	push	ax
	call	printString
	add	sp, 2
L_myinth_14:
	mov	sp, bp
	pop	bp
	ret
L_myinth_8:
	push	bp
	mov	bp, sp
	sub	sp, 4
	jmp	L_myinth_9
L_myinth_16:
	DB	0xA,"TICK ",0
	ALIGN	2
YKTickHandler:
	; >>>>> Line:	26
	; >>>>> void YKTickHandler() { 
	jmp	L_myinth_17
L_myinth_18:
	; >>>>> Line:	30
	; >>>>> YKTickNum++; 
	mov	ax, word [delayedHead]
	mov	word [bp-2], ax
	mov	word [bp-4], 0
	; >>>>> Line:	30
	; >>>>> YKTickNum++; 
	inc	word [YKTickNum]
	; >>>>> Line:	31
	; >>>>> printString("\nTICK "); 
	mov	ax, L_myinth_16
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	32
	; >>>>> printInt(YKTickNum); 
	push	word [YKTickNum]
	call	printInt
	add	sp, 2
	; >>>>> Line:	33
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	35
	; >>>>> while(cur != 0) { 
	jmp	L_myinth_20
L_myinth_19:
	; >>>>> Line:	36
	; >>>>> nextDelayed = cur 
	mov	si, word [bp-2]
	add	si, 10
	mov	ax, word [si]
	mov	word [bp-4], ax
	; >>>>> Line:	37
	; >>>>> cur->delay--; 
	mov	si, word [bp-2]
	add	si, 12
	dec	word [si]
	; >>>>> Line:	38
	; >>>>> if(cur->delay <= 0) { 
	mov	si, word [bp-2]
	add	si, 12
	mov	ax, word [si]
	test	ax, ax
	jne	L_myinth_22
	; >>>>> Line:	39
	; >>>>> cur->state = READY; 
	mov	si, word [bp-2]
	add	si, 6
	mov	word [si], 3
	; >>>>> Line:	41
	; >>>>> delayedHead = removeFromQueue(cur, delayedHead); 
	push	word [delayedHead]
	push	word [bp-2]
	call	removeFromQueue
	add	sp, 4
	mov	word [delayedHead], ax
	; >>>>> Line:	43
	; >>>>> readyHead = addToQueue(cur, readyHead); 
	push	word [readyHead]
	push	word [bp-2]
	call	addToQueue
	add	sp, 4
	mov	word [readyHead], ax
L_myinth_22:
	; >>>>> Line:	46
	; >>>>> cur = nextDelayed; 
	mov	ax, word [bp-4]
	mov	word [bp-2], ax
L_myinth_20:
	mov	ax, word [bp-2]
	test	ax, ax
	jne	L_myinth_19
L_myinth_21:
	mov	sp, bp
	pop	bp
	ret
L_myinth_17:
	push	bp
	mov	bp, sp
	sub	sp, 4
	jmp	L_myinth_18
