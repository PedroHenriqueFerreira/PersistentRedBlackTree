INPUT = entrada.txt

build: main.c
	gcc -o programa.exe main.c

test: original.c
	gcc -o programa.exe original.c

run: programa.exe
	./programa.exe $(INPUT)