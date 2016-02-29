#include <stdio.h>
#include <stdlib.h>

#include "register.h"
#include "parse.h"

extern args_st args;
extern status_st status;

extern instr_st *fe;
extern instr_st *de;
extern instr_st *rn;
extern mments_st mments;
extern scope_st scope[10000];
int *SeqNo_temp;

void fetch(){

	static FILE *fp;
	static BYTE init =0;

	BYTE i = 0;
	BYTE k = 0;

	unsigned int pc;
	unsigned int sn;
	int type;
	int dest;
	int src1;
	int src2;
	char string[80];

	
	static BYTE SNT=0;




	if (init==0){

	fp = fopen(args.fname,"r");
	SeqNo_temp = calloc(args.width,sizeof(int));

#ifdef ADEBUG
	if (fp==NULL){
		printf("\n\n!!Error!!, cannot open file\n\n");
		exit(1);
		}
#endif

		init = 1;
	
	}
#ifdef SCOPE1
	if ((!status.isDEe) && (!status.isTFe)){

		if (SNT==0){
			SNT =1;
			for(i=0;i<args.width;i++)
			{
				SeqNo_temp[i] = mments.Gseq_no;
				mments.Gseq_no++;
			}

		}

		for(i=0;i<args.width;i++){
			scope[SeqNo_temp[i]].fe_duration++;
		}

			
	}
#endif	


	if ( (status.isDEe) && (!status.isTFe) )
	{

		do{
			fgets(string,80,fp);
#ifdef ADEBUG
			printf("String: %s\n",string);
#endif
			sscanf(string,"%8x %d %d %d %d\n",&pc, &type, &dest, &src1, &src2);

			fe[i].pc = pc;
			fe[i].type = (BYTE) type;
			fe[i].dest = (BYTE) dest;
			fe[i].src1 = (BYTE) src1;
			fe[i].src2 = (BYTE) src2;


			if(SNT==0)
				fe[i].seq_no = mments.Gseq_no;
			else{
				fe[i].seq_no = SeqNo_temp[i];
				SNT = 0;

			}
				


			sn = fe[i].seq_no;
#ifdef SCOPE
			scope[sn].type = (BYTE) type;
			scope[sn].dest = (BYTE) dest;
			scope[sn].src1 = (BYTE) src1;
			scope[sn].src2 = (BYTE) src2;
			scope[sn].fe_duration++;
#endif			
			mments.Gseq_no++;
			i++;
			if (feof(fp))
				status.isTFe = TRUE;


		} while( (i!=args.width) && (!status.isTFe));

		args.width1= args.width;

		k=i;
		if ((status.isTFe) && (i!=args.width) )
		{
			
			set_zero(k,fe);

		}
#ifdef BDEBUG
		printf("\nFETCH in Cycle Number:%d",status.cycles);
		printf("\nGseq_no:%d",mments.Gseq_no);
		printf("\nfe[0] seq_no:%d",fe[0].seq_no);
#endif


#ifdef SCOPE
		for (i=0;i<args.width1;i++)
			if(scope[fe[i].seq_no].fe_end==0)
				scope[fe[i].seq_no].fe_end = status.cycles;
#endif
		memcpy(de,fe,(args.width * sizeof(instr_st)));
		status.isDEe = FALSE;
		//status.isFEe = TRUE;

	}


}


void decode(){

	BYTE i = 0;

#ifdef SCOPE1
	if ((!status.isRNe) && (!status.isDEe)){

		for(i=0;i<args.width;i++){
			scope[de[i].seq_no].de_duration++;
		}
			
	}
#endif	

	if ( (status.isRNe) && (!status.isDEe)){

		for (i=0;i<args.width;i++)
		{
			de[i].src1_rdy =0;
			de[i].src2_rdy =0;
		}

#ifdef BDEBUG
		printf("\nDECODE in Cycle Number:%d",status.cycles);
		printf("\nGseq_no:%d",mments.Gseq_no);
		printf("\nde[0] seq_no:%d",de[0].seq_no);
#endif


#ifdef SCOPE
		for (i=0;i<args.width1;i++){
			scope[de[i].seq_no].de_duration++;
			if(scope[de[i].seq_no].de_end==0)
				scope[de[i].seq_no].de_end = status.cycles;
		}
			
#endif
		memcpy(rn,de,args.width * sizeof(instr_st));
		status.isRNe = FALSE;
		status.isDEe = TRUE;



	}





}


void set_zero(BYTE k, instr_st *p) {

	while (k<args.width)
	{
		p[k].pc = 0;
		k++;

	}

}