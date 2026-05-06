INPUT = entrada.txt

test: original.c
	gcc -o programa.exe original.c

build: main.c
	gcc -o programa.exe main.c

run: programa.exe
	./programa.exe $(INPUT)