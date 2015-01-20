#include<memory.h>
#include<stdio.h>

#define STRIPENUM 1024
#define STRIPESIZE 320
#define RONLYPTR 1
#define WONLYPTR 2
#define RWPTR 3

//										  |
//                                      V	 read/write port ptr					
//------------------------------------------------------------------------//
//|					 |		data	 blocks										|				|//
//|--------------|-------------------------------------------|-----------|//

//Instruction Type
//Read_Type: 1
//Write_Type: 2
//Unknown_Type: 3(assume this is type 2)

class Ports{
private:
	short ptrtag[STRIPESIZE]; 
public:
	Ports(){
		memset(ptrtag,0,sizeof(ptrtag));
		
		/*Initialize my implementation*/
		ptrtag[63] = RONLYPTR;
		ptrtag[67] = RONLYPTR;
		ptrtag[71] = RONLYPTR;
		ptrtag[78] = WONLYPTR;
		ptrtag[83] = RONLYPTR;
		ptrtag[87] = RONLYPTR;
		ptrtag[95] = RWPTR;
		ptrtag[104] = RONLYPTR;
		ptrtag[108] = RONLYPTR;
		ptrtag[113] = WONLYPTR; 
		ptrtag[120] = RONLYPTR;
		ptrtag[124] = RONLYPTR;
		ptrtag[128] = RONLYPTR;
	}

	void PrintState(){
		for(int i = 64; i <= 128; i++){
			printf("%d ", ptrtag[i]); 
		}
		printf("\n");
	}

	bool JudgeType1(int& blockpos, int& instr_type){
		if(ptrtag[blockpos] == RWPTR || ptrtag[blockpos] == RONLYPTR) //read type instr, no need to shift
			return false;

		//Find Upper
		int upidx = blockpos; 
		while(upidx >= 0 && (ptrtag[upidx] != RWPTR && ptrtag[upidx] != RONLYPTR))upidx--; 

		//Find lower
		int lowidx = blockpos; 
		while(lowidx < STRIPESIZE && (ptrtag[lowidx] != RWPTR && ptrtag[lowidx] != RONLYPTR))lowidx++;

		int deltaup = blockpos-upidx;
		int deltadown = lowidx-blockpos;
		int delta = deltaup>deltadown ?-deltadown:deltaup;

		int i = 0; 
		if(delta < 0){
			int i = 0; 
			while(ptrtag[i] == 0)i++;
			for(; i < STRIPESIZE; i++){
				ptrtag[i+delta] = ptrtag[i];
				if(ptrtag[i] != 0)ptrtag[i] = 0;
			}
		}else{
			int i = STRIPESIZE-1; 
			while(i >= 0 && ptrtag[i] == 0)i--; 
			for(; i >= 0; i--){
				ptrtag[i+delta] = ptrtag[i]; 
				if(ptrtag[i] != 0)ptrtag[i] = 0;
			}
		}
		return true;   //need to shift
	}

	bool JudgeType2and3(int& blockpos, int& instr_type){
		if(ptrtag[blockpos] == RWPTR || ptrtag[blockpos] == WONLYPTR) //write or unknown type instr, no need to shift
			return false;

		//Find Upper
		int upidx = blockpos; 
		while(upidx >= 0 && (ptrtag[upidx] != RWPTR && ptrtag[upidx] != WONLYPTR))upidx--; 

		//Find lower
		int lowidx = blockpos; 
		while(lowidx < STRIPESIZE && (ptrtag[lowidx] != RWPTR && ptrtag[lowidx] != RONLYPTR))lowidx++;

		int deltaup = blockpos-upidx;
		int deltadown = lowidx-blockpos;
		int delta = deltaup>deltadown ?-deltadown:deltaup;

		int i = 0; 
		if(delta < 0){
			int i = 0; 
			while(ptrtag[i] == 0)i++;
			for(; i < STRIPESIZE; i++){
				ptrtag[i+delta] = ptrtag[i];
				if(ptrtag[i] != 0)ptrtag[i] = 0;
			}
		}else{
			int i = STRIPESIZE-1; 
			while(i >= 0 && ptrtag[i] == 0)i--; 
			for(; i >= 0; i--){
				ptrtag[i+delta] = ptrtag[i]; 
				if(ptrtag[i] != 0)ptrtag[i] = 0;
			}
		}
		return true;   //need to shift		
	}

	bool Move(int blockpos, int instr_type){
		blockpos += 64; 
		if(instr_type == 1)return JudgeType1(blockpos,instr_type);
		return JudgeType2and3(blockpos,instr_type);
	}
}stripe[STRIPENUM];
