#ipl.o
./build/ipl.o :	ipl.asm	Makefile
	nasm ipl.asm -o ./build/ipl.o -l ./build/ipl.lst

#asmhead.o
./build/asmhead.o: asmhead.asm Makefile
	nasm -g -f elf asmhead.asm -o ./build/asmhead.o -l ./build/asmhead.lst

#bootpack.hrb
./build/bootpack.hrb	:	./build/asmhead.o	bootpack.c	Makefile
	gcc -march=i486 -fno-pic -m32 -nostdlib -T hrb.ld bootpack.c -o ./build/bootpack.hrb ./build/asmhead.o 
	#https://hanachin.hateblo.jp/entry/2019/07/06/022100

#helloos.sys
./build/helloos.sys :	./build/bootpack.hrb	./build/ipl.o	Makefile
	cat ./build/ipl.o ./build/bootpack.hrb > ./build/helloos.sys

#helloos.img
helloos.img :	./build/ipl.bin	./build/helloos.sys	Makefile
	mformat -f 1440 -C -B ./build/ipl.bin -i helloos.img ::
	mcopy ./build/helloos.sys -i helloos.img ::

img	:
	make -r helloos.img

asm	:
	make -r ./build/ipl.bin

run :
	qemu-system-x86_64 -fda helloos.img

build : helloos.img
	make img
	make run