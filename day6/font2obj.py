import pathlib
import sys

#thx http://bttb.s1.valueserver.jp/wordpress/blog/2017/12/13/makeos-5-1/

args = sys.argv

key = ''
arg_kwrg = {}
for arg in args:
    if arg[0] == '-':
        key = arg[1:]
    elif key != '':
        arg_kwrg[key] = arg
        key = ''
    else:
        arg_kwrg["i"] = arg

if "i" in arg_kwrg:
    inputPath = pathlib.Path(arg_kwrg["i"])
else:
    print("Fatal Error! input file was not specified")
    sys.exit()
if not inputPath.exists():
    print("Fatal Error! input file does not exist")
    sys.exit()
r = inputPath.open()
if not r.readable():
    print("Fatal Error! can't read input file")

if "o" in arg_kwrg:
    outputPath = pathlib.Path(arg_kwrg["o"])
else:
    outputPath = pathlib.Path("out.asm")

data = []
f = inputPath.open()
for line in f.readlines():
    flg = True
    out = 0
    i = 0b10000000
    for c in line:
        if c!='\n' and c!="*" and c!= ".":
            flg = False
            break
        elif c!= '\n':
            if c=="*":
                out = out + i
                i = i>>1
            elif c==".":
                i = i>>1
        elif c=='\n' and i < 2:
            data.append(out)
f.close()

f = pathlib.Path.open(outputPath, "w")
f.write("char hankaku[4096] = {")
i = 0
for i in range(len(data)):
    f.write(f"{hex(data[i])}")
    if i < (4096-1):
        f.write(",")
f.write(r"};")
f.close()


