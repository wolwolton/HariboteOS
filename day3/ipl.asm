; hello-os
; TAB=4
CYLS EQU 10

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; ブートセクタの名前を自由に書いてよい（8バイト）
		DW		512				; 1セクタの大きさ（512にしなければいけない）
		DB		1				; クラスタの大きさ（1セクタにしなければいけない）
		DW		1				; FATがどこから始まるか（普通は1セクタ目からにする）
		DB		2				; FATの個数（2にしなければいけない）
		DW		224				; ルートディレクトリ領域の大きさ（普通は224エントリにする）
		DW		2880			; このドライブの大きさ（2880セクタにしなければいけない）
		DB		0xf0			; メディアのタイプ（0xf0にしなければいけない）
		DW		9				; FAT領域の長さ（9セクタにしなければいけない）
		DW		18				; 1トラックにいくつのセクタがあるか（18にしなければいけない）
		DW		2				; ヘッドの数（2にしなければいけない）
		DD		0				; パーティションを使ってないのでここは必ず0
		DD		2880			; このドライブ大きさをもう一度書く
		DB		0,0,0x29		; よくわからないけどこの値にしておくといいらしい
		DD		0xffffffff		; たぶんボリュームシリアル番号
		DB		"HELLO-OS   "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX
;ディスクを読む
		MOV		AX,0x820
		MOV		ES,AX
		MOV		CH,0			;シリンダ0
		MOV		DH,0			;ヘッド0
		MOV		CL,2			;セクタ2

readloop:
		MOV		SI,0
retry:
		MOV		AH,0x02			;AH=0x02 : ディスク読み込みに設定
		MOV		AL,1			;1つ分のセクタを読む（AL:処理するセクタの数）
		MOV		BX,0			;セグメントレジスタを設定する。
								;メモリのES:BX番地のアドレスにディスク
								;から読みだした内容を書き込む（このアドレスを
								;バッファアドレスとよぶ)
								;BXに0を指定すると、ES:BXのセグメントレジスタは0
								;ということになり、0*16+ES=ES番地にディスクの内容が
								;書き込まれる。
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ディスクBIOS呼び出し
		JNC		next
		ADD		SI,1
		CMP		SI,5
		JAE		error
		MOV		AH,0x00			;AHレジスタ初期化 (システムのリセット1)
		MOV		DL,0x00			;Aドライブ (システムのリセット2)
		INT		0x13			;ドライブのリセット (システムのリセット3)
		JMP		retry
next:
		MOV		AX,ES
		ADD		AX,0x020
		MOV		ES,AX			;ADD ES,1ができないのでこうする
		ADD		CL,1
		CMP		CL,18
		JBE		readloop
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB 		readloop
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop
		JMP 	success
error:
		MOV		SI,errormsg
		JMP		putloop

success:
		JMP 	0xc200
		MOV		SI,successmsg
		JMP		putloop

fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ

putloop:
		MOV		AL,[SI]
		ADD		SI,1			; SIに1を足す
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 一文字表示ファンクション
		MOV		BX,14			; カラーコード
		INT		0x10			; ビデオBIOS呼び出し
		JMP		putloop

successmsg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"SUCCESSS"
		DB		0x0a			; 改行
		DB		0

		DB		0x55, 0xaa

errormsg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"ERROR"
		DB		0x0a			; 改行
		DB		0

		RESB	0x7dfe-($-$$)-0x7c00		; 0x7dfeまでを0x00で埋める命令
        ;0x7c00引くように変更(https://qiita.com/pollenjp/items/d15fce401bccd37e8059)
        ;おそらくQEMUのブートセクタの読み込まれるアドレスが0x7c00~でない
        ;EDKと合わせてメモリマップ取得->配置をやりたい

		DB		0x55, 0xaa
