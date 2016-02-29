#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;


extern instr_st *rr;
extern instr_st *di;
extern instr_st *iq_head;
extern instr_st *iq_tail;
extern instr_st *ex;

extern mments_st mments;
extern scope_st scope[10000];


void dispatch() {
	BYTE i;

#ifdef SCOPE1
	if (((args.iq_size -status.lIQf) >= args.width) && (!status.isDIe)){

		for(i=0;i<args.width;i++){
			scope[di[i].seq_no].di_duration++;
		}
			
	}
#endif

#ifdef ADEBUG
		printf("\nInside Dispatch\nstatus.isDIe:%d \nargs.iq_size:%d",status.isDIe,args.iq_size);
		printf("\nstatus.lIQf: %d",status.lIQf);
#endif


	if ( (!status.isDIe) && ((args.iq_size -status.lIQf) >= args.width) ){
#ifdef BDEBUG
		printf("\nDISPATCH in Cycle Number:%d",status.cycles);
#endif
#ifdef SCOPE
		for (i=0;i<args.width1;i++){
			scope[di[i].seq_no].di_duration++;
			if(scope[di[i].seq_no].di_end==0)
				scope[di[i].seq_no].di_end = status.cycles;
		}
			
#endif
		fillIQ();
		status.isDIe = TRUE;

	}


	}

void fillIQ() {

	BYTE i;
	instr_st *ptr;
#ifdef ADEBUG
	//printIQ();
	if (iq_head == NULL)
		printf("\nIQ HEAD IS NULL\n");
	if (iq_tail == NULL)
		printf("\nIQ TAIL IS NULL\n");
#endif

	for (i=0;i<args.width;i++){

	if (iq_tail == NULL)
	{
		iq_tail = calloc(1, sizeof(instr_st));
		iq_head = iq_tail;

		ptr = iq_tail;
		status.lIQf++;

	}
	else
	{
		ptr = calloc(1, sizeof(instr_st));
		iq_tail->next = ptr;
		iq_tail = ptr;
		status.lIQf++;

	}

	ptr->pc= di[i].pc;
	ptr->seq_no =di[i].seq_no;

	
	ptr->dest =di[i].dest;
	ptr->src1 =di[i].src1;
	ptr->src2 =di[i].src2;

	ptr->type =di[i].type;
	ptr->src1_rdy =di[i].src1_rdy;
	ptr->src2_rdy =di[i].src2_rdy;
	//ptr->iq_valid =1;
	ptr->next = NULL;

	}

}


void issue(){

	
	BYTE tot =0;
	instr_st *ptr;
	instr_st *prev;
	ptr = iq_head;
	prev = NULL;



	while ( (tot<args.width) && (ptr!=NULL) )
	{

		if ( (ptr->src1_rdy) && (ptr->src2_rdy) )
		{
		// ****ISSUE THIS INSTRUCTION *****/////
			fillEX(tot, ptr);
			tot++;
#ifdef BDEBUG
		printf("\nISSUE %d in Cycle Number:%d",tot,status.cycles);
#endif
#ifdef SCOPE
		if (scope[ptr->seq_no].iq_end==0)
			scope[ptr->seq_no].iq_end = status.cycles;
		scope[ptr->seq_no].iq_duration++;
#endif	

			//delete this node
			if (prev!=NULL)
			{
				prev->next= ptr->next;
				status.lIQf--;
				if (ptr==iq_tail)
					iq_tail = prev;
				free(ptr);

				ptr = prev->next;

			}
			else
			{
				//this is the head
				iq_head = ptr->next;
				status.lIQf--;
				if (ptr==iq_tail)
					iq_tail = NULL;
				free(ptr);
				ptr = iq_head;

			}


		}
		else
		{

#ifdef SCOPE
		scope[ptr->seq_no].iq_duration++;
#endif	
		prev = ptr;
		ptr = ptr->next;


		}
		

		

	}


}

void fillEX(BYTE i, instr_st *ptr){


	ex[i].pc= ptr->pc;
	ex[i].seq_no =ptr->seq_no;

	
	ex[i].dest =ptr->dest;
	ex[i].src1 =ptr->src1;
	ex[i].src2 =ptr->src2;

	ex[i].type =ptr->type;
	ex[i].src1_rdy =ptr->src1_rdy;
	ex[i].src2_rdy =ptr->src2_rdy;
	
	ex[i].next = NULL;
	ex[i].ex_valid = 1;


	switch (ex[i].type){
		case 0: ex[i].EXtimer = 1;
		break;

		case 1: ex[i].EXtimer = 2;
		break;

		case 2: ex[i].EXtimer = 5;
		break;
	}




}
