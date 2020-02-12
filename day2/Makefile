ipl.o : ipl.asm Makefile
	nasm ipl.asm -o ipl.o -l ipl.lst

helloos.img : ipl.o Makefile
	cat ipl.o > helloos.img

img	:
	make -r helloos.img

asm	:
	make -r ipl.o

run :
	qemu-system-x86_64 helloos.img

build :
	make img
	make run