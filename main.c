#include <stdio.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;



int main(int argc, char *argv[]) {

	parse_args(argc,argv);
	init_status();
	

#ifdef ADEBUG
	printf("Args.width: %d\n",args.width);
#endif

	do{
		retire();

		writeback();

		execute();

		issue();

		dispatch();

		regread();

		Rename();

		decode();

		fetch();
	}while (AdvanceCycle());

	display_scope();
	

	return 0;



}

