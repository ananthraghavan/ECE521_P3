#include <stdio.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;
extern instr_st *iq_head;
extern instr_st *ex;
extern instr_st *wb;


BYTE checkWBf(){

	int i;
	i=0;

	while (i<(5*args.width)){

		if (wb[i].wb_valid)
			return TRUE;
		i++;
	}

	return FALSE;

}

BYTE checkEXf(){
	
	int i;
	i=0;

	while (i<(5*args.width)){

		if (ex[i].ex_valid == 1)
			return TRUE;
		i++;
	}

	return FALSE;


}

BYTE checkIQf(){

	if (status.lIQf == 0)
		return FALSE;
	else
		return TRUE;



}


BYTE AdvanceCycle(){


	status.cycles++;

	BYTE isPIPEf;
	BYTE isROBf;
	BYTE isIQf;
	BYTE isEXf;
	BYTE isWBf;

	isROBf = (status.lROBe==args.rob_size)?FALSE:TRUE;
	isIQf  = (checkIQf()==0)?FALSE:TRUE;
	isEXf  = (checkEXf()==0)?FALSE:TRUE;
	isWBf  = (checkWBf()==0)?FALSE:TRUE;

	isPIPEf = isROBf || isIQf || isEXf || isWBf || (!status.isDEe)|| (!status.isRNe) || (!status.isRRe) || (!status.isDIe);
#ifdef BDEBUG
		printf("\nAdvanceCycle in Cycle Number:%d",status.cycles);
		printf("\nisROBf: %s",(isROBf==0?"FALSE":"TRUE"));
		printf("\nisWBf: %s",(isWBf==0?"FALSE":"TRUE"));
		printf("\nisEXf: %s",(isEXf==0?"FALSE":"TRUE"));
		printf("\nisIQf: %s",(isIQf==0?"FALSE":"TRUE"));
		printf("\nisDIf: %s",((!status.isDIe)==0?"FALSE":"TRUE"));
		printf("\nisRRf: %s",((!status.isRRe)==0?"FALSE":"TRUE"));
		printf("\nisRNf: %s",((!status.isRNe)==0?"FALSE":"TRUE"));
		printf("\nisDEf: %s",((!status.isDEe)==0?"FALSE":"TRUE"));
		
		printf("\n\n");
		printIQ();
		printf("\n\n");
		printROB();

		//if (status.cycles==110)
		//	return FALSE;

		
#endif


    return (isPIPEf || (!status.isTFe));
	//return TRUE;


}




