ipl.bin :	ipl.asm	Makefile
	nasm ipl.asm -o ipl.bin -l ipl.lst

helloos.sys :	helloos.asm	Makefile
	nasm helloos.asm -o helloos.sys -l helloos.lst

helloos.img :	ipl.bin	helloos.sys	Makefile
	mformat -f 1440 -C -B ipl.bin -i helloos.img ::
	mcopy helloos.sys -i helloos.img ::

img	:
	make -r helloos.img

asm	:
	make -r ipl.bin

run :
	qemu-system-x86_64 -fda helloos.img

build : helloos.img
	make img
	make run