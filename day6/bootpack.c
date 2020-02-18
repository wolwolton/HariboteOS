#include "bootpack.h"

void HariMain(void)
{
    struct BOOTINFO *binfo;

    binfo=(struct BOOTINFO*) 0x0ff0;

    init_palette();

    init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

    
    static char font_A[16] = {0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, 0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x0, 0x0};

    putfonts8_asc(binfo->vram, binfo->scrnx, "Hello World!!!", hankaku, COL8_FFFFFF, 5, 5);

    char* s;
    sprintf(s, "scrnx = %d", binfo->scrnx);
    
    putfonts8_asc(binfo->vram, binfo->scrnx, s, hankaku, COL8_FF00FF, 5, 20);
    

    for(;;){
        io_hlt();
    }
}
