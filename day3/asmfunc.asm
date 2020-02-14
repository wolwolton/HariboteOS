;thx https://qiita.com/pollenjp/items/8fcb9573cdf2dc6e2668
; nasmfunc.asm
; TAB=4

section .text
    GLOBAL  _io_hlt;
    GLOBAL _write_mem8;


_io_hlt:        ; void io_hlt(void);
    HLT
    RET

_write_mem8:
    MOV     ECX, DWORD [ESP+4]
    MOV     AL,  BYTE [ESP+8]
    MOV     BYTE [ECX], AL
    RET