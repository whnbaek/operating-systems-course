all:
	gcc -g loader.c -o loader 
	gcc -g -fPIE -pie -nostdlib -o target target.c

target:
	gcc -g -static -o target target.c

clean:
	rm loader target
