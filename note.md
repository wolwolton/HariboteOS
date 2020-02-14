## アセンブラ

・NASK：フリーソフトでは有名なアセンブラであるNASMの文法を多く踏襲し筆者が作成したアセンブラ。

###　命令

・DB命令：data byteの略。[func1]ファイルの内容を1バイトだけ直接書く。[func2]DB 文字列とすることで、文字列のそれぞれに対応する文字コードを並べる。

・RESB命令：reserve byteの略。指定バイトの領域をあけておく。NASKではあけるだけではなく、そこを0で埋める。

・DW命令、DD命令：それぞれdata word, data double-wordの略で、DB命令の仲間。wordは16bitをさす。つまり、double-wordは32bit

・＄：その行が先頭から何行目かを返す。つまり、DB $を5行目にかけば、ファイルに5と書き込む。

・ORG命令：originの略。機械語が実行時にPCのメモリのどこに読み込まれることになるかを指定する

・JMP命令：jumpの略。C言語でいうgo to。

・ラベル：「entry:」のように文字列:と書く。JMP命令の行き先などに用いる。ラベルが表すのはラベルのある番地である。つまり、MOV AX entryとすれば、entryのある番地がAXに代入される。そして、JMP AXとすれば、AXの表す番地、すなわち、entryのところに飛ぶことができる。

・MOV命令：moveの略。[func1]基本はMOV AX,0やMOV SS,AXと用いる。日本語に訳すと「移動」だが、実際の操作は代入であり、AX=0, SS=AXとなる。[func2]MOV BYTE [678],123などのように書くとメモリーに代入することができる。BYTEは代入する値の形式をさし、BYTEは1byte、WORDは2byteである。[678]はメモリー上のアドレスをさし、[]で囲って表記する。さきの命令は「メモリー上678番地に1byteのデータ”123”を代入せよ」となる。

・CMP命令：compareの略。CMP a,bなどのように用い、if(a==b)と相応の処理をする。

・JE命令：jump if equalの略。CMPの結果a==bの場合には指定されたラベルなどにjumpする。

・INT命令：interruptの略。ソフトウェア割込み命令で、BIOSの提供する関数を実行する。INT 3などのように書く。INTにつづく数字で度の関数を利用するか選ぶことができる。

・HLT命令：haltの略。CPUを待機状態にさせる。外部からの入力（マウスを動かす、キーボードに入力する等）で動作を再開する。

・キャリーフラグ(FLAGS.CF)：1ビットしか記憶できないレジスタをフラグという。CPUはフラグをいくつか持ち、キャリーフラグはキャリー（carry）という状態を示すためのもの。だが、フラグの中ではキャリーフラグが最も使いやすいのでそれ以外の用途でも用いられる。

・JC命令：jump if carryの略。キャリーフラグが1のときjumpする。

### フロッピーディスクの読み込み

近年のコンピュータではFDDは搭載されていないが当時のPCは多くがFDDドライブを搭載していた。FDDはBIOSだけで読み込みができ、手軽に利用できたようである。

参考：[https://ja.wikipedia.org/wiki/%E3%83%95%E3%83%AD%E3%83%83%E3%83%94%E3%83%BC%E3%83%87%E3%82%A3%E3%82%B9%E3%82%AF](https://ja.wikipedia.org/wiki/フロッピーディスク)

### QEMUの使い方
    ミニキャンでQEMUが扱ったが、そのときは、UEFIアプリを動かすための設定をいくつかしたと思う。
    たしかその設定が割と細かく、少しずれるとうまく動かなかったと思う。
    今回はそれほど細かく設定せずとも動作するようである。
#### ドライブの設定
    今回の自作OSはフロッピーディスクに入れるので、ドライブの設定をフロッピーディスクとする。それは簡単なオプションがあって、"-fda"で設定できる。
#### CPUアーキテクチャの設定
    QEMUでエミュレートするCPUのアーキテクチャを設定する。選択肢は割とあって、
    qemu-system-aarch64       qemu-system-mips64        qemu-system-s390x
    qemu-system-alpha         qemu-system-mips64el      qemu-system-sh4
    qemu-system-arm           qemu-system-mipsel        qemu-system-sh4eb
    qemu-system-cris          qemu-system-moxie         qemu-system-sparc
    qemu-system-i386          qemu-system-nios2         qemu-system-sparc64
    qemu-system-lm32          qemu-system-or1k          qemu-system-tricore
    qemu-system-m68k          qemu-system-ppc           qemu-system-unicore32
    qemu-system-microblaze    qemu-system-ppc64         qemu-system-x86_64
    qemu-system-microblazeel  qemu-system-ppc64le       qemu-system-xtensa
    qemu-system-mips          qemu-system-ppcemb        qemu-system-xtensaeb
    から選ぶ。今回は"qemu-system-x86_64"を選んだがあってるかは不明
    （⇐i386がネットでは多い）

### Ubuntuでのディスクイメージの作成
    mformat, mcopyコマンドはmtoolsの一部。mtoolsはflopydiskのimgファイルなどの
    操作のためのツールらしい。
    〇mformat
    flopy disk imageを作るためのコマンドらしい。おそらく、ファイルシステム上に仮想的なflopy diskを作るイメージでいいいと思う。今回は
        mformat -f 1440 -C -B ./build/ipl.bin -i helloos.img ::
    とした。
    "-f 1440"はフォーマットされたファイルの容量、すなわちflopy disk全体のファイルサイズを指定している。
    "-C"これは「ディスクイメージを作成する」という設定(?)
    "-B ./build/ipl.bin" "ipl.bin"の内容をboot sectorに書き込むという設定
    "-i helloos.img" たぶん"helloos.img"という内容で保存するの意味
    
    〇mcopy
    文字どおり、コピーする関数のよう…
	mcopy ./build/helloos.sys -i helloos.img ::
    
    参考：https://qiita.com/pollenjp/items/8fcb9573cdf2dc6e2668
    参考：http://takeisamemo.blogspot.com/2014/09/os30os-3-4.htmlhttp://takeisamemo.blogspot.com/2014/09/os30os-3-4.html
    参考：https://www.gnu.org/software/mtools/manual/mtools.html
### HRB形式