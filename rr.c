#include <stdio.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;

extern instr_st *rn;
extern instr_st *rr;
extern instr_st *di;
extern rob_st *rob_val;
extern mments_st mments;
extern scope_st scope[10000];


void regread(){

	BYTE i = 0;
	int src1;   //local vars for speed and convenience
	int src2;


#ifdef SCOPE1
	if ((!status.isDIe) && (!status.isRRe)){

		for(i=0;i<args.width;i++){
			scope[rr[i].seq_no].rr_duration++;
		}
			
	}
#endif
	


	if ( (status.isDIe) && (!status.isRRe)){

		while (i<args.width){

		src1 = rr[i].src1;
		src2 = rr[i].src2;	
		//check source operands readiness
		if (!rr[i].src1_rdy)
		{
			if (src1 < RPROT)   //means ARF
				rr[i].src1_rdy = 1;
			else               // means ROB
				rr[i].src1_rdy = rob_val[src1-RPROT].rdy;

		}
		if (!rr[i].src2_rdy)
		{
			if (src2 < RPROT)   //means ARF
				rr[i].src2_rdy = 1;
			else                // means ROB
				rr[i].src2_rdy = rob_val[src2-RPROT].rdy;
		}

		i++;

		}

#ifdef BDEBUG
		printf("\nREGISTER_READ in Cycle Number:%d",status.cycles);
#endif
#ifdef SCOPE
		for (i=0;i<args.width1;i++){
			scope[rr[i].seq_no].rr_duration++;
			if(scope[rr[i].seq_no].rr_end==0)
				scope[rr[i].seq_no].rr_end = status.cycles;
		}
			
#endif

		memcpy(di,rr,args.width * sizeof(instr_st));
		status.isDIe = FALSE;
		status.isRRe = TRUE;


	}





}
