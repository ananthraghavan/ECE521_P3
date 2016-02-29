#include <stdio.h>
#include <stdlib.h>


#include "register.h"
#include "parse.h"


args_st args;
status_st status;
scope_st scope[10000];


instr_st *fe;
instr_st *de;
instr_st *rn;
instr_st *rr;
instr_st *di;
instr_st *iq_head;
instr_st *iq_tail;
instr_st *ex;
instr_st *wb;
instr_st *rob;

rob_st *rob_val;

int RMT[NO_REGS];
mments_st mments;

void parse_args (int argc, char *argv[]) {


	unsigned int i;
	char *ptr;

	if (argc!=5)
	{
		printf("\nError number of arguments incorrect!\nExiting!\n");
		exit(1);
	}

	args.rob_size = strtol(argv[1],&ptr,10);
	args.iq_size  = strtol(argv[2],&ptr,10);
	args.width    = strtol(argv[3],&ptr,10);


	args.fname  = argv[4]; 
}


void init_status() {

	int i;

	mments.Gseq_no = 0;

	status.isTFe = FALSE;


	status.isDEe = TRUE;
	status.isRNe = TRUE;
	status.isRRe = TRUE;
	status.isDIe = TRUE;
	status.isIQe = TRUE;
	status.isEXe = TRUE;
	status.isWBe = TRUE;

	status.lROBe = args.rob_size;
	status.lIQf = 0;
	status.ROBhead = 0;
	status.ROBtail = 0;

	status.cycles = 0;




	
	fe = calloc(args.width, sizeof(instr_st));
	de = calloc(args.width, sizeof(instr_st));
	rn = calloc(args.width, sizeof(instr_st));
	rr = calloc(args.width, sizeof(instr_st));
	di = calloc(args.width, sizeof(instr_st));
	//iq = calloc(args.iq_size, sizeof(instr_st));
	ex = calloc((5 * args.width), sizeof(instr_st));
	wb = calloc((5 * args.width), sizeof(instr_st));
	rob = calloc(args.rob_size, sizeof(instr_st));

	rob_val = calloc(args.rob_size, sizeof(rob_st));

	iq_head =NULL;
	iq_tail =NULL;
#ifdef PDEBUG
	print_bundle(fe,'f');
	print_bundle(de,'d');
	print_bundle(rn,'n');
	print_bundle(rr,'r');
	print_bundle(di,'i');

	print_bundleEX(ex,'e');
	print_bundleEX(wb,'w');
#endif
	for (i=0;i<args.rob_size;i++){
		rob_val[i].rdy = -2;
		rob_val[i].dest = -2;
	}

	for (i=0;i<NO_REGS;i++){

		RMT[i] = -1;
	}

	for (i=0;i<10000;i++){

		scope[i].dest = 0;
		scope[i].src1 = 0;
		scope[i].src2 = 0;
		scope[i].type = 0;

		scope[i].fe_end = 0;
		scope[i].fe_duration = 0;

		scope[i].de_end = 0;
		scope[i].de_duration = 0;

		scope[i].rn_end = 0;
		scope[i].rn_duration = 0;

		scope[i].rr_end = 0;
		scope[i].rr_duration = 0;

		scope[i].di_end = 0;
		scope[i].di_duration = 0;

		scope[i].iq_end = 0;
		scope[i].iq_duration = 0;

		scope[i].ex_end = 0;
		scope[i].ex_duration = 0;

		scope[i].wb_end = 0;
		scope[i].wb_duration = 0;

		scope[i].rt_end = 0;
		scope[i].rt_duration = 0;

	}
	
}


void print_bundle(instr_st *ptr, char ch){

	BYTE i;
	printf("\nStage: %c",ch);

	for (i=0;i<args.width;i++)
	{
		printf("\nPipeline register number %d",i);

		printf("\nPC: %x",ptr[i].pc);
		printf("\nSeqNo: %d",ptr[i].seq_no);
		printf("\nDest: %d",ptr[i].dest);
		printf("\nSRC1: %d",ptr[i].src1);
		printf("\nSRC2: %d",ptr[i].src2);
		printf("\nTYPE: %d",ptr[i].type);
		printf("\nSRC1_RDY: %d",ptr[i].src1_rdy);
		printf("\nSRC2_RDY: %d",ptr[i].src2_rdy);
		printf("\nEX_VALID: %d",ptr[i].ex_valid);
		printf("\nEXTimer: %d",ptr[i].EXtimer);
		printf("\nWB_VALID: %d",ptr[i].wb_valid);

	}


}


void print_bundleEX(instr_st *ptr, char ch){

	BYTE i;
	printf("\nStage: %c",ch);

	for (i=0;i<(5*args.width);i++)
	{
		printf("\nPipeline register number %d",i);

		printf("\nPC: %x",ptr[i].pc);
		printf("\nSeqNo: %d",ptr[i].seq_no);
		printf("\nDest: %d",ptr[i].dest);
		printf("\nSRC1: %d",ptr[i].src1);
		printf("\nSRC2: %d",ptr[i].src2);
		printf("\nTYPE: %d",ptr[i].type);
		printf("\nSRC1_RDY: %d",ptr[i].src1_rdy);
		printf("\nSRC2_RDY: %d",ptr[i].src2_rdy);
		printf("\nEX_VALID: %d",ptr[i].ex_valid);
		printf("\nEXTimer: %d",ptr[i].EXtimer);
		printf("\nWB_VALID: %d",ptr[i].wb_valid);

	}


}

void printIQ(){

	instr_st *ptr;
	BYTE i=1;

	ptr = iq_head;
	printf("\nPRINTING IQ");

	while (ptr!=NULL)
	{
		printf("\nIQ number %d",i);

		printf("\nPC: %x",ptr->pc);
		printf("\nSeqNo: %d",ptr->seq_no);
		printf("\nDest: %d",ptr->dest);
		printf("\nSRC1: %d",ptr->src1);
		printf("\nSRC2: %d",ptr->src2);
		printf("\nTYPE: %d",ptr->type);
		printf("\nSRC1_RDY: %d",ptr->src1_rdy);
		printf("\nSRC2_RDY: %d",ptr->src2_rdy);
		printf("\nEX_VALID: %d",ptr->ex_valid);
		printf("\nEXTimer: %d",ptr->EXtimer);
		printf("\nWB_VALID: %d",ptr->wb_valid);

		printf("\n\n");
		i++;
		ptr = ptr->next;
	}


}

void printROB(){

	BYTE i =status.ROBhead;

	printf("\nPRINTING ROB");
	printf("\nROBhead:%d",status.ROBhead);
	printf("\nROBtail:%d",status.ROBtail);
	printf("\nlROBe:%d",status.lROBe);

	for (i=0;i<args.rob_size;i++)
	{
		if(i==status.ROBhead)
			printf("\nTHIS IS ROBhead");
		if(i==status.ROBtail)
			printf("\nTHIS IS ROBtail");
		printf("\nROB INDEX:%d",i);
		printf("\nROBtag:%d",(i+RPROT));
		printf("\nRDY:%d",rob_val[i].rdy);
		printf("\nDEST:%d",rob_val[i].dest);
		printf("\nSEQ_NO:%d",rob_val[i].seq_no);
		printf("\n\n");

	}
}

void display_scope(){

	int i;
	float IPC;
	for (i=0;i<mments.Gseq_no;i++){

		printf("%d fu{%d} src{%d,%d} dst{%d}",
			i, scope[i].type, scope[i].src1,
			scope[i].src2, scope[i].dest);
		// printf(" FE{%d,%d}",(scope[i].fe_end-scope[i].fe_duration),scope[i].fe_duration);
		// printf(" DE{%d,%d}",(scope[i].de_end-scope[i].de_duration),scope[i].de_duration);
		// printf(" RN{%d,%d}",(scope[i].rn_end-scope[i].rn_duration),scope[i].rn_duration);
		// printf(" RR{%d,%d}",(scope[i].rr_end-scope[i].rr_duration),scope[i].rr_duration);
		// printf(" DI{%d,%d}",(scope[i].di_end-scope[i].di_duration),scope[i].di_duration);
		// printf(" IS{%d,%d}",(scope[i].iq_end-scope[i].iq_duration),scope[i].iq_duration);
		// printf(" EX{%d,%d}",(scope[i].ex_end-scope[i].ex_duration),scope[i].ex_duration);
		// printf(" WB{%d,%d}",(scope[i].wb_end-scope[i].wb_duration),scope[i].wb_duration);
		// printf(" RT{%d,%d}",(scope[i].rt_end-scope[i].rt_duration),scope[i].rt_duration);

		printf(" FE{%d,%d}",(scope[i].fe_end),scope[i].fe_duration);
		printf(" DE{%d,%d}",(scope[i].fe_end+1),(scope[i].de_end - scope[i].fe_end));
		printf(" RN{%d,%d}",(scope[i].de_end+1),(scope[i].rn_end - scope[i].de_end));
		printf(" RR{%d,%d}",(scope[i].rn_end+1),(scope[i].rr_end - scope[i].rn_end));
		printf(" DI{%d,%d}",(scope[i].rr_end+1),(scope[i].di_end - scope[i].rr_end));
		printf(" IS{%d,%d}",(scope[i].di_end+1),(scope[i].iq_end - scope[i].di_end));
		printf(" EX{%d,%d}",(scope[i].iq_end+1),(scope[i].ex_end - scope[i].iq_end));
		printf(" WB{%d,%d}",(scope[i].ex_end+1),(scope[i].wb_end - scope[i].ex_end));
		printf(" RT{%d,%d}\n",(scope[i].wb_end+1),(scope[i].rt_end - scope[i].wb_end));




	}
	IPC = (float) ((float)(mments.Gseq_no+1)/status.cycles);

		printf("# === Simulator Command =========");
		printf("\n# ./sim %d %d %d %s",args.rob_size,args.iq_size,args.width,args.fname);
		printf("\n# === Processor Configuration ===");
		printf("\n# ROB_SIZE  = %d",args.rob_size);
		printf("\n# IQ_SIZE   = %d",args.iq_size);
		printf("\n# WIDTH     = %d",args.width);
		printf("\n# === Simulation Results ========");
		printf("\n# Dynamic Instruction Count = %d",mments.Gseq_no);
		printf("\n# Cycles = %d",status.cycles);
		printf("\n# Instructions Per Cycle (IPC) = %1.2f\n", IPC);
}