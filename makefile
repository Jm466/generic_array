options = 	-std=c17\
			-Wall\
			-Wextra\
			-pedantic\
			-fanalyzer\
			-fsanitize=address\
			-fsanitize=bounds\
			-fsanitize=null\
			-g\
			-O0

compile : test.c garray.h
	gcc $(options) test.c -o test

debug : test.c garray.h
	gcc -save-temps test.c -o debug || true
	grep -v '^#.*' debug-test.i > debugP.c
	sed 's/}/}\n/g' debugP.c | sed 's/;/;\n/g' | sed 's/{/{\n/g' |sed 's/return\n/return /g' > debug.c
	rm -v debug-test.i debug-test.o debug-test.s debug debugP.c || true
	gcc $(options) debug.c -o debug.bin

clean:
	rm -v debug.bin debug.c test || true
