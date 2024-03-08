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

#-Werror\

compile : main.c garray.h
	gcc $(options) main.c -o test

debug : main.c garray.h
	gcc -save-temps main.c -o debug
	grep -v '^#.*' debug-main.i > debugP.c
	sed 's/}/}\n/g' debugP.c | sed 's/;/;\n/g' | sed 's/{/{\n/g' |sed 's/return\n/return /g' debugP.c  > debug.c
	rm -v debug-main.i debug-main.o debug-main.s debug debugP.c || true
	gcc $(options) debug.c -o debug.bin

clean:
	rm -v debug.bin debug.c test || true
