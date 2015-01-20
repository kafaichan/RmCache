#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Instruction.h"
#include "RtmCache.h"

#pragma warning(disable:4996)
using namespace std; 
long long int maxinstruction; 

extern Clock cc; 
extern Statistics stats; 

const char* filepath = "input.txt";

int main(int argv, char** argc){
	FILE* fin = fopen(filepath,"r"); //open the file according to the path described by argc[1]
	scanf("%lld", &maxinstruction);

	char operation;  //r, w or u 
	long long int address;  //visit address
	long long int timestamp;
	int a, b;  //unknown parameter of the input document
	Instruction instr; 

	bool firstinstr = true;
	while(~fscanf(fin,"%c %lld %ld %ld %lld\n", &operation, &address, &a, &b, &timestamp) && maxinstruction > 0){
		printf("%c %lld %ld %ld %lld\n", operation, address, a, b, timestamp);
		if(firstinstr){
			cc.IncreaseTick(timestamp);
			firstinstr = false;
		}
		--maxinstruction;
		instr.set_instr(address,timestamp);
		instr.print_instr(); 

		/*record how many instructions each type has*/
		if(operation == 'r')stats.rtype_instr++; 
		else if(operation == 'w')stats.wtype_instr++; 
		else stats.utype_instr++; 

		long long int timetmp = cc.getTime(); 
		if(timetmp > timestamp)timestamp = timetmp;
		else cc.setTick(timestamp);

		int setnum = instr.get_setnum(); 
		int tag = instr.get_tag();
		RaceTrackMemoryCache* rtmc = &rtmcache[setnum];

		if(!rtmc->FindBlock(tag,timestamp)){
			rtmc->CacheMiss(setnum,tag,timestamp);
			if(!rtmc->InsertBlock(tag,timestamp)){
				rtmc->Evict(tag,timestamp);
			}
		}
	}
	stats.PrintReport();
	system("pause"); 
	return 0;
}
