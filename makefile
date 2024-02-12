options = 	-std=c17\
			-Wall\
			-Wextra\
			-Werror\
			-pedantic\
			-fanalyzer\
			-fsanitize=address\
			-fsanitize=undefined\
			-fsanitize=null\
			-fsanitize=bounds\
			-g\
			-O0

compile : main.c garray.h
	gcc $(options) main.c -o test

debug : main.c garray.h
	gcc -save-temps main.c -o debug
	grep -v '^#.*' debug-main.i > debugP.c
	sed 's/}/}\n/g' debugP.c | sed 's/;/;\n/g' | sed 's/{/{\n/g' > debug.c
	nvim debug.c -c wq
	rm -v debug-main.i debug-main.o debug-main.s debug debugP.c || true
	gcc $(options) debug.c -o debug.bin
