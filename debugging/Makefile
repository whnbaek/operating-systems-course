prof: prof.c
	gcc -pg -o prof prof.c

analysis:
	gprof prof gmon.out > analysis.txt
