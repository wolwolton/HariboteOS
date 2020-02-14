extern void io_hlt(void);
extern void io_cli(void);
extern void io_out8(int port, int data);
extern int io_load_eflags(void);
extern int io_store_eflags(int eflags);

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void init_palette(void);
void init_screen(unsigned char* vram, int xsize, int ysize);
void set_palette(int start, int end, unsigned char *rgb);
void putfont8(char* vram, int xsize, char* font, unsigned char c, int x, int y);
void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

struct BOOTINFO{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

void HariMain(void)
{
    struct BOOTINFO *binfo;

    binfo=(struct BOOTINFO*) 0x0ff0;

    init_palette();

    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    
    static char font_A[16] = {0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, 0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x0, 0x0};

    putfont8(binfo->vram, binfo->scrnx, font_A, COL8_FFFFFF, 5, 5);
    

    for(;;){
        io_hlt();
    }
}

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