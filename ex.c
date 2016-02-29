#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;


extern instr_st *ex;
extern instr_st *wb;
extern instr_st *di;
extern instr_st *rr;
extern mments_st mments;
extern scope_st scope[10000];

extern instr_st *iq_head;


void execute () {

	int i;
	int wakeup_dest;
	instr_st* ptr;
	instr_st* dest_ptr;
	int tot=0;

	for (i=((5*args.width)-1);i>=0;i--) {

		if (ex[i].ex_valid){
			if (ex[i].EXtimer==1)
			{

				wakeup_dest = ex[i].dest;
#ifdef BDEBUG
		printf("\nEXECUTE with wakeup_dest:%d in Cycle Number:%d",wakeup_dest,status.cycles);
#endif
#ifdef SCOPE
		if (scope[ex[i].seq_no].ex_end==0)
			scope[ex[i].seq_no].ex_end = status.cycles;
		scope[ex[i].seq_no].ex_duration++;
#endif			
				dest_ptr = wb + tot;
				ptr = ex + i;
				ex[i].ex_valid = 0;
				ex[i].wb_valid = 1;
				memcpy(dest_ptr,ptr,sizeof(instr_st));
				tot++;
			

				wakeup(wakeup_dest);
			}
			else
			{
#ifdef SCOPE1
				if(ex[i].EXtimer>0)
						scope[ex[i].seq_no].ex_duration++;
#endif
				ex[i].EXtimer--;

				ptr = ex + i;
				dest_ptr = ptr + args.width;
				memcpy(dest_ptr,ptr,sizeof(instr_st));
				ex[i].ex_valid = 0;
			}	
		}

		
	}
}

// does wakeup of IQ, DI, RR bundle 
void wakeup(int wakeup_dest){


	instr_st *ptr;
	BYTE i;
	//instr_st *prev;

	/***ISSUE WAKEUP *****/

	ptr = iq_head;
	while (ptr!=NULL){
		if (ptr->src1 == wakeup_dest)
			ptr->src1_rdy = 1;
		if (ptr->src2 == wakeup_dest)
			ptr->src2_rdy = 1;

		ptr = ptr->next;

	}

	/***DI RR WAKEUP*///

	i = 0;
	while (i<args.width){

		if (di[i].src1 == wakeup_dest)
			di[i].src1_rdy = 1;
		if (di[i].src2 == wakeup_dest)
			di[i].src2_rdy = 1;


		if (rr[i].src1 == wakeup_dest)
			rr[i].src1_rdy = 1;
		if (rr[i].src2 == wakeup_dest)
			rr[i].src2_rdy = 1;
		
		i++;
	}



}
