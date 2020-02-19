#include "bootpack.h"

void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;

    init_gdtidt();
    init_pic();
    io_sti();
	io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
	io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

    init_palette();

    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    putfonts8_asc(binfo->vram, binfo->scrnx, "Hello World!!!", hankaku, COL8_FFFFFF, 5, 5);

    char* s;
    sprintf(s, "scrnx = %d", binfo->scrnx);
    
    putfonts8_asc(binfo->vram, binfo->scrnx, s, hankaku, COL8_FF00FF, 5, 20);
    
    for(;;){
        io_hlt();
    }
}
