all:
	gcc -c src/buffer.c -o ./build/buffer.o
	gcc -c src/utils.c -o ./build/utils.o
	gcc -c src/scan.c -o ./build/scan.o
	gcc -c src/parser.c -o ./build/parser.o

	gcc src/main.c ./build/*.o -o ./build/quati
	rm ./build/*.o