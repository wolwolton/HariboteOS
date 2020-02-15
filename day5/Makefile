#ipl.o
./build/ipl.o :	ipl.asm	Makefile
	nasm ipl.asm -o ./build/ipl.o -l ./build/ipl.lst

#asmhead.o
./build/asmhead.o: asmhead.asm Makefile
	nasm asmhead.asm -o ./build/asmhead.o -l ./build/asmhead.lst

#hankaku.o
./build/hankaku.o: font2obj.py hankaku.txt Makefile
	python3 font2obj.py -i hankaku.txt -o ./build/hankaku.c
	gcc -c -m32 ./build/hankaku.c -o ./build/hankaku.o

#asmfunc.o
./build/asmfunc.o: asmfunc.asm Makefile
	nasm -f elf asmfunc.asm -o ./build/asmfunc.o -l ./build/asmfunc.lst

#mysprintf.o
./build/mysprintf.o:	mysprintf.c	Makefile
	gcc -march=i486 -c -m32 -nostdlib mysprintf.c -fno-pie -o ./build/mysprintf.o

#bootpack.hrb
./build/bootpack.hrb	:hrb.ld	./build/asmfunc.o	./build/hankaku.o	./build/mysprintf.o	bootpack.c	Makefile
	gcc -march=i486 -m32 -fno-pie \
	-nostdlib -nostdinc \
	-T hrb.ld bootpack.c \
	./build/asmfunc.o ./build/hankaku.o ./build/mysprintf.o  -o ./build/bootpack.hrb  
#https://hanachin.hateblo.jp/entry/2019/07/06/022100

#helloos.sys
./build/helloos.sys :	./build/bootpack.hrb	./build/asmhead.o	Makefile
	cat ./build/asmhead.o ./build/bootpack.hrb > ./build/helloos.sys

#helloos.img
./build/helloos.img :	./build/ipl.o	./build/helloos.sys	Makefile
	mformat -f 1440 -C -B ./build/ipl.o -i ./build/helloos.img ::
	mcopy ./build/helloos.sys -i ./build/helloos.img ::

img	:
	make -r ./build/helloos.img

asm	:
	make -r ./build/ipl.bin

run :
	qemu-system-i386 -drive file=./build/helloos.img,format=raw,index=0,media=disk,if=floppy

build : ./build/helloos.img Makefile
	make img
	make run