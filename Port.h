#include<memory.h>
#include<stdio.h>

#define STRIPENUM 1024
#define STRIPESIZE 192
#define RONLYPTR 1
#define WONLYPTR 2
#define RWPTR 3

//										  |
//                                      V	 read/write port ptr					
//------------------------------------------------------------------------//
//|					 |		data	 blocks										|				|//
//|--------------|-------------------------------------------|-----------|//

class Ports{
private:
	short ptrtag[STRIPESIZE]; 
public:
	Ports(){
		memset(ptrtag,0,sizeof(ptrtag));
		/*Initialization  for baseline*/

		ptrtag[64] = RWPTR;
		ptrtag[80] = RWPTR; 
		ptrtag[96] = RWPTR; 
		ptrtag[112] = RWPTR;
	}
	
	bool Move(int blockpos){
		blockpos += 64; 
		if(ptrtag[blockpos] == RWPTR){
			//printf("On the read write port!\n");
			return false; //no need to shift
		}else{
			/*Find Upper*/
			int upidx = blockpos; 
			while(upidx >= 0 && ptrtag[upidx] != RWPTR)upidx--;

			/*Find lower*/
			int lowidx = blockpos;
			while(lowidx < 192 && ptrtag[lowidx] != RWPTR)lowidx++; 

			int deltaup = blockpos-upidx; 
			int deltadown = lowidx-blockpos;
			int delta = deltaup>deltadown ?-deltadown: deltaup;

			if(delta < 0){
				int i = 0; 
				while(ptrtag[i] == 0)i++;
				for(; i < STRIPESIZE; i++){
					ptrtag[i+delta] = ptrtag[i];
					if(ptrtag[i] == RWPTR)ptrtag[i] = 0;
				}
			}else{
				int i = STRIPESIZE-1; 
				while(i >= 0 && ptrtag[i] == 0)i--; 
				for(; i >= 0; i--){
					ptrtag[i+delta] = ptrtag[i]; 
					if(ptrtag[i] == RWPTR)ptrtag[i] = 0;
				}
			}
			return true;   //need to shift
		}
	}

}stripe[STRIPENUM];
