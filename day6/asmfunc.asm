;thx https://qiita.com/pollenjp/items/8fcb9573cdf2dc6e2668
; nasmfunc.asm
; TAB=4

section .text
    GLOBAL  io_hlt, io_cli, io_sti, io_stihlt
    GLOBAL  io_in8, io_in16, io_in32
    GLOBAL  io_out8, io_out16, io_out32
    GLOBAL  io_load_eflags, io_store_eflags
    GLOBAL  load_idtr, load_gdtr
    EXTERN  _inthandler21, _inthandler2c, _inthandler27
    GLOBAL  asm_inthandler21, asm_inthandler2c, asm_inthandler27

io_hlt:
    HLT
    RET

io_cli:
    CLT
    RET

io_sti:
    STI
    RET

io_stihlt:
    STI
    HLT
    RET

io_in8: ;int io_in8(int port)
    MOV EDX, [ESP+4];
    MOV EAX, 0
    IN AL, DX
    RET

io_in16: ;int io_in16(int port)
    MOV EDX, [ESP+4]
    MOV EAX, 0
    IN AX, DX
    RET

io_in32: ;int io_in32(int port)
    MOV EDX, [ESP+4]
    IN EAX, DX
    RET

io_out8:
    MOV EDX, [ESP+4]
    MOV AL, [ESP+8]
    OUT DX, AL
    RET

io_out16:
    MOV EDX, [ESP+4]
    MOV EAX, [ESP+8]
    OUT DX, AX
    RET

io_out32:
    MOV EDX, [ESP+4]
    MOV EAX, [ESP+8]
    OUT DX, EAX
    RET

io_load_eflags:
    PUSHFD  ;   PUSH EFLAGS という意味
    POP EAX
    RET

io_store_eflags:
    MOV EAX, [ESP+4]
    PUSH EAX
    POPFD   ;   POP EFLAGS という意味
    RET

load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

