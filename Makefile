build.c:
	gcc -c *.c # Pré compila todos .c para utilização posterior

build:	build.c 
	gcc OrdenacaoExterna1.o main.o -lm -o run # Gera o executavel run

clear:#Comando de limpeza
	rm *.o

all: build.c build clear

