options = 	-std=c17\
			-Wall\
			-Wextra\
			-pedantic\
			-Wno-unused-parameter\
			-Wno-unused-function\
			-ggdb\
			-O0

fanalyzer = -fanalyzer\
			-fsanitize=address\
			-fsanitize=bounds\
			-fsanitize=null\
			-fsanitize=pointer-compare\
			-fsanitize=pointer-subtract\
			-fsanitize=leak\
			-fsanitize=undefined

normal : test.c garray.h garray.c
	gcc $(options) test.c -o test

analyzer : test.c garray.h garray.c
	gcc $(options) $(fanalyzer) test.c -o test-analyzer
