#include <stdio.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;


extern instr_st *ex;
extern instr_st *wb;
extern instr_st *rob;
extern rob_st *rob_val;
extern int RMT[NO_REGS];
extern mments_st mments;
extern scope_st scope[10000];


void writeback(){

	int i;

	for (i=((5*args.width)-1);i>=0;i--) {

		if (wb[i].wb_valid){
			// set ready bit in ROB
#ifdef BDEBUG
		printf("\nWRITEBACK with ROBtag:%d in Cycle Number:%d",wb[i].dest,status.cycles);
#endif
#ifdef SCOPE
		if (scope[wb[i].seq_no].wb_end==0)
			scope[wb[i].seq_no].wb_end = status.cycles;
		scope[wb[i].seq_no].wb_duration++;
#endif	
			rob_val[((wb[i].dest)-RPROT)].rdy = 1;
			wb[i].wb_valid =0;

		}
	}

}


void retire(){

	int i;
	int rmt_ROBtag;
	BYTE count = 0;

	i = status.ROBhead;

	while (count<args.width){

		if (status.lROBe!=args.rob_size){
			if (rob_val[i].rdy){
						//rob_val[i].rdy = 0;
#ifdef BDEBUG
				printf("\nRETIRING ROB INDEX:%d in Cycle Number:%d",i,status.cycles);
#endif
#ifdef SCOPE
		if (scope[rob_val[i].seq_no].rt_end == 0)
			scope[rob_val[i].seq_no].rt_end = status.cycles;
		scope[rob_val[i].seq_no].rt_duration++;
#endif

				if(rob_val[i].dest!=-1)
				{
					rmt_ROBtag = RMT[rob_val[i].dest];

					if (rmt_ROBtag == (i+RPROT))
						RMT[rob_val[i].dest] = -1;
				}
			}
			else{
#ifdef SCOPE1
		scope[rob_val[i].seq_no].rt_duration++;
#endif
				break;
			}
				


			if(i==args.rob_size-1)
				i=0;
			else
				i++;
			status.lROBe++;
			
			status.ROBhead = i;

		}
		count++;
		

	}

}