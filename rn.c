#include <stdio.h>

#include "parse.h"
#include "register.h"


extern args_st args;
extern status_st status;

extern instr_st *rn;
extern instr_st *rr;
extern instr_st *di;

extern rob_st *rob_val;
extern instr_st *rob;

extern int RMT[NO_REGS];
extern mments_st mments;
extern scope_st scope[10000];


void Rename(){

	BYTE i = 0;
	//int ROBptr;
	int ROBtag;

#ifdef SCOPE1
	if ((!status.isRRe) && (!status.isRNe)){

		for(i=0;i<args.width;i++){
			scope[rn[i].seq_no].rn_duration++;
		}
			
	}
#endif

	if ( (status.isRRe) && (!status.isRNe)){

		if (status.lROBe >= args.width)
		{
			
			while (i<args.width)
			{

				//if fetch has fetched < width instructions
				if (rn[i].pc==0)
					break;
				//get ROBtag
				//initialize robval	

				ROBtag = getROBtag();

				rob_val[ROBtag -RPROT].rdy = 0;

				//put register in robval
				rob_val[ROBtag -RPROT].dest = rn[i].dest;
				rob_val[ROBtag -RPROT].seq_no = rn[i].seq_no;

				/*********SOURCE*********/

				//look up RMT and rename if there is an entry
				if (rn[i].src1 !=-1)
				{
					if (RMT[(rn[i].src1)] !=-1)
					{
						rn[i].src1 =  RMT[(rn[i].src1)];
					}
				}

				if (rn[i].src2 !=-1)
				{
					if (RMT[(rn[i].src2)] !=-1)
					{
						rn[i].src2 =  RMT[(rn[i].src2)];
					}
				}

				
				
				/********DESTINATION*****/

				if (rn[i].dest != -1) 
				{
					
					//update RMT here
					RMT[rn[i].dest] = ROBtag;					

				}

				//rename dest_reg in the instr_st
				//should happen even if dest ==-1
				rn[i].dest = ROBtag;
				fillROB(ROBtag, i);				

				i++;
			}

#ifdef BDEBUG
		printf("\nREGISTER_RENAME in Cycle Number:%d",status.cycles);
		printf("\nGseq_no:%d",mments.Gseq_no);
		printf("\nrn[0] seq_no:%d",rn[0].seq_no);
#endif

#ifdef SCOPE
		for (i=0;i<args.width1;i++){
			scope[rn[i].seq_no].rn_duration++;
			if(scope[rn[i].seq_no].rn_end==0)
				scope[rn[i].seq_no].rn_end = status.cycles;
		}
			
#endif

			memcpy(rr,rn,args.width * sizeof(instr_st));
			status.isRRe = FALSE;
			status.isRNe = TRUE;
		}

	}




}

//returns ROBtail, checks if tail is 
//end of array, wraps around
//decrements lROBe
//does not check for space in rob, doesn't 
//need to

int getROBtag() {

	int tag;

	tag = status.ROBtail;

	if(status.ROBtail < args.rob_size-1)
		status.ROBtail++;
	else
		status.ROBtail = 0;

	status.lROBe--;

	return (tag+RPROT);

}

void fillROB(int ROBtag, BYTE i){


	rob[ROBtag- RPROT].pc= rn[i].pc;
	rob[ROBtag- RPROT].seq_no =rn[i].seq_no;

	
	rob[ROBtag- RPROT].dest =rn[i].dest;
	rob[ROBtag- RPROT].src1 =rn[i].src1;
	rob[ROBtag- RPROT].src2 =rn[i].src2;

	rob[ROBtag- RPROT].type =rn[i].type;
	rob[ROBtag- RPROT].src1_rdy =rn[i].src1_rdy;
	rob[ROBtag- RPROT].src2_rdy =rn[i].src2_rdy;
	//rob[ROBtag- RPROT].iq_valid =rn[i].iq_valid;


}