flex scanner.l

bison -vdty parser.y

gcc y.tab.c lex.yy.c -o parser.exe
