nasm = nasm
gcc = gcc -march=i486 -c -m32 -nostdlib -fno-pie 
gcc2hrb = gcc -march=i486 -m32 -fno-pie -nostdlib -nostdinc -T hrb.ld bootpack.c
mformat = mformat -f 1440 -C -B
mcopy = mcopy

#asm2o
./build/%.o : %.asm Makefile
	$(nasm) $*.asm -o ./build/$*.o -l ./build/$*.lst

#c2o
./build/%.obj : %.c Makefile
	$(gcc) $*.c -o ./build/$*.obj

#asmfunc.o
./build/asmfunc.o:	asmfunc.asm	Makefile
	$(nasm) -f elf asmfunc.asm -o ./build/asmfunc.o -l ./build/asmfunc.lst

#hankaku.o
./build/hankaku.o: font2obj.py hankaku.txt Makefile
	python3 font2obj.py -i hankaku.txt -o ./build/hankaku.c
	gcc -c -m32 ./build/hankaku.c -o ./build/hankaku.o

#bootpack.hrb
./build/bootpack.hrb	:hrb.ld	./build/asmfunc.o	./build/hankaku.o	./build/mysprintf.obj	bootpack.c	./build/graphic.obj	./build/dsctbl.obj	Makefile
	$(gcc2hrb) -L ./build ./build/asmfunc.o ./build/hankaku.o ./build/mysprintf.obj ./build/graphic.obj ./build/dsctbl.obj  -o ./build/bootpack.hrb  
#https://hanachin.hateblo.jp/entry/2019/07/06/022100

#helloos.sys
./build/helloos.sys :	./build/bootpack.hrb	./build/asmhead.o	Makefile
	cat ./build/asmhead.o ./build/bootpack.hrb > ./build/helloos.sys

#helloos.img
./build/helloos.img :	./build/ipl.o	./build/helloos.sys	Makefile
	$(mformat) ./build/ipl.o -i ./build/helloos.img ::
	$(mcopy) ./build/helloos.sys -i ./build/helloos.img ::

img	:
	make -r ./build/helloos.img

asm	:
	make -r ./build/ipl.bin

run :
	qemu-system-i386 -drive file=./build/helloos.img,format=raw,index=0,media=disk,if=floppy

build : ./build/helloos.img Makefile
	make img
	make run

rebuild :./build/helloos.img Makefile
	rm -rf ./build
	mkdir ./build
	make img
	make run