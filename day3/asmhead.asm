;
    CYLS    EQU     0x0ff0      
    LEDS    EQU     0x0ff1
    VMODE   EQU     0x0ff2
    SCRNX   EQU     0x0ff4
    SCRNY   EQU     0x0ff6
    VRAM    EQU     0x0ff8

    ORG     0xc200

    MOV     AL,0x13     ;ビデオモード設定   VGAグラフィックス, 320x200x8bit Color   パックドピクセル
    MOV     AH,0x00     ;ビデオモード用の設定
    INT     0x10        ;ビデオモードの割込み

    MOV     BYTE [VMDOE], 8             ;ビデオモードをメモする
    MOV     WORD [SCRNX], 320           ;ビデオモードのXピクセル数をメモ
    MOV     WORD [SCRNY], 200           ;ビデオモードのYピクセル数をメモ
    MOV     DWORD [VRAM], 0x00a000      ;ビデオモードが8のときにはVRAMの位置が0x00a000~になるので
                                        ;メモように確保した[VRAM]のところにアドレスをメモ

;キーボードのLED状態をBIOSに教えてもらう

    MOV     AH, 0x02
    INT     0x16            ;keybord BIOS
    MOV     [LEDS], AL
fin:
    HLT
    JMP fin
