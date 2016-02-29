#ifndef REGISTER_H_
#define REGISTER_H_

#define FALSE 0
#define TRUE !FALSE

#define RPROT 66
#define NO_REGS 67

//#include "parse.h"

//#define BDEBUG
#define SCOPE


typedef char BYTE;

typedef struct {

	unsigned int rob_size;
	unsigned int iq_size;
	unsigned int width;
	unsigned int width1;

	char *fname;

}args_st;

typedef struct{

	BYTE isTFe;   // trace file

	BYTE isDEe;   // DE stage
	BYTE isRNe;   // RN stage
	BYTE isRRe;   // RR stage
	BYTE isDIe;   // DI stage
	BYTE isIQe;   // IQ stage
	BYTE isEXe;   // EX stage
	BYTE isWBe;   // WB stage
	
	int lROBe;   // length of ROB empty
	int lIQf;	 // length of IQ full
	int ROBhead;
	int ROBtail;

	unsigned int cycles;

	

}status_st;

struct instr_st{

	unsigned int pc;
	unsigned int seq_no;

	
	int dest;
	int src1;
	int src2;

	BYTE type;
	BYTE src1_rdy;
	BYTE src2_rdy;
	BYTE ex_valid;
	BYTE EXtimer;
	BYTE wb_valid;

	struct instr_st *next;



};

typedef struct instr_st instr_st;


typedef struct{

	BYTE rdy;
	BYTE dest;
	unsigned int seq_no;


}rob_st;


typedef struct{
	unsigned int Gseq_no;

}mments_st;

typedef struct{

	BYTE dest;
	BYTE src1;
	BYTE src2;
	BYTE type;


	unsigned int fe_end;
	unsigned int fe_duration;

	unsigned int de_end;
	unsigned int de_duration;

	unsigned int rn_end;
	unsigned int rn_duration;
	
	unsigned int rr_end;
	unsigned int rr_duration;

	unsigned int di_end;
	unsigned int di_duration;

	unsigned int iq_end;
	unsigned int iq_duration;

	unsigned int ex_end;
	unsigned int ex_duration;

	unsigned int wb_end;
	unsigned int wb_duration;

	unsigned int rt_end;
	unsigned int rt_duration;

}scope_st;

#endif  /*REGISTER_H_*/