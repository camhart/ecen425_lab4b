#####################################################################
# ECEn 425 Lab 4B Makefile

lab4B.bin:	lab4Bfinal.s
		nasm lab4Bfinal.s -o lab4B.bin -l lab4B.lst

lab4Bfinal.s:	clib.s myisr.s myinth.s lab4b_app.s yakc.s yaks.s
		cat clib.s myisr.s myinth.s yakc.s  yaks.s > lab4Bfinal.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

yakc.s:		yakc.c
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s
	
lab4b_app.s:	lab4b_app.c
		cpp lab4b_app.c lab4b_app.i
		c86 -g lab4b_app.i lab4b_app.s

clean:
		rm lab4b.bin lab4b.lst lab4Bfinal.s myinth.s myinth.i \
		yakc.s yakc.i lab4b_app.s lab4b_app.i
