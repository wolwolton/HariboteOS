// asmhead.asm
struct BOOTINFO{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
}; 

#define ADR_BOOTINFO 0x00000ff0;

// asmfunc.asm
extern void io_hlt(void);
extern void io_cli(void);
extern void io_out8(int port, int data);
extern int io_load_eflags(void);
extern int io_store_eflags(int eflags);
extern void load_gdtr(int limit, int addr);
extern void load_idtr(int limit, int addr);

//graphic.c
extern char hankaku[4096];
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
void putfonts8_asc(char* vram, int xsize, char* string, char* font, unsigned char c, int x, int y);
void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

//dsctbl.c
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

