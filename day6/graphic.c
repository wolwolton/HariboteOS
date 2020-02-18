#include "bootpack.h"

void init_screen(unsigned char* vram, int xsize, int ysize){

    boxfill8(vram, xsize, COL8_0000FF, 0, 0, xsize, ysize);

    boxfill8(vram, xsize, COL8_FFFFFF, 0, ysize-26, xsize, ysize);
    boxfill8(vram, xsize, COL8_C6C6C6, 0, ysize-25, xsize, ysize);

    boxfill8(vram, xsize, COL8_000000, 3, ysize-21, 31, ysize-3);
    boxfill8(vram, xsize, COL8_FFFFFF, 3, ysize-21, 30, ysize-4);
    boxfill8(vram, xsize, COL8_C6C6C6, 4, ysize-20, 30, ysize-4);
    
    boxfill8(vram, xsize, COL8_FFFFFF, xsize-30, ysize-22, xsize-2, ysize-3);
    boxfill8(vram, xsize, COL8_848484, xsize-30, ysize-22, xsize-3, ysize-4);
    boxfill8(vram, xsize, COL8_C6C6C6, xsize-29, ysize-21, xsize-3, ysize-4);
    return;
}

void init_palette(void){
    static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黒 */
		0xff, 0x00, 0x00,	/*  1:明るい赤 */
		0x00, 0xff, 0x00,	/*  2:明るい緑 */
		0xff, 0xff, 0x00,	/*  3:明るい黄色 */
		0x00, 0x00, 0xff,	/*  4:明るい青 */
		0xff, 0x00, 0xff,	/*  5:明るい紫 */
		0x00, 0xff, 0xff,	/*  6:明るい水色 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:明るい灰色 */
		0x84, 0x00, 0x00,	/*  9:暗い赤 */
		0x00, 0x84, 0x00,	/* 10:暗い緑 */
		0x84, 0x84, 0x00,	/* 11:暗い黄色 */
		0x00, 0x00, 0x84,	/* 12:暗い青 */
		0x84, 0x00, 0x84,	/* 13:暗い紫 */
		0x00, 0x84, 0x84,	/* 14:暗い水色 */
		0x84, 0x84, 0x84	/* 15:暗い灰色 */
    };
    set_palette(0, 15, table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb){
    int i, eflags;
    eflags = io_load_eflags();  //割込み許可フラグの値を記録する
    io_cli();       //許可フラグを0にして割込み禁止にする
    io_out8(0x03c8, start);
    for(i = start; i <= end; i++){
        io_out8(0x03c9, rgb[0] /4);
        io_out8(0x03c9, rgb[1] /4);
        io_out8(0x03c9, rgb[2] /4);
        rgb += 3;
    }
    /*
    0x3c8とか0x3c9というのはビデオDAコンバータの設定に関する数字*
    詳細は本誌p88下
    */
    io_store_eflags(eflags);    //割込み許可フラグを元に戻す
    return;
}

void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1){
    int x, y;
    for(y = y0; y <= y1; y++){
        for(x = x0; x<=x1; x++){
            vram[y*xsize + x] = c;
        }
    }
    return;
}

void putfont8(char* vram, int xsize, char* font, unsigned char c, int x, int y){
    for(int i=0; i<16; i++){
        char* p = vram + (y+i)*xsize + x;
        unsigned char d = font[i];
        if((d & 0x80) != 0){ p[0] = c;}
        if((d & 0x40) != 0){ p[1] = c;}
        if((d & 0x20) != 0){ p[2] = c;}
        if((d & 0x10) != 0){ p[3] = c;}
        if((d & 0x08) != 0){ p[4] = c;}
        if((d & 0x04) != 0){ p[5] = c;}
        if((d & 0x02) != 0){ p[6] = c;}
        if((d & 0x01) != 0){ p[7] = c;}
    }
    return;
}

void putfonts8_asc(char* vram, int xsize, char* string, char* font, unsigned char c, int x, int y){
    int i = 0;
    while(string[i]!=0){
        putfont8(vram,xsize,font+string[i]*16,c,x+i*9,y);
        i++;
    }
}
