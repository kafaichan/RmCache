#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define SETSHIFT 6
#define TAGSHIFT 19

#define BIAS(address) (address & 0x3F)
#define SETNUM(address) ((address >> SETSHIFT) & 0x1FFF)
#define TAG(address) ((address >> TAGSHIFT) & 0x1FFF)

class Instruction{
public:
	Instruction():bias(0),setnum(0),tag(0){}
	void set_instr(long long int address, long long int timestamp);
	int get_bias(); 
	int get_setnum(); 
	int get_tag();
	void print_instr();

private:
	int bias;
	int setnum; 
	int tag; 
	long long int timestamp;
};


void Instruction::set_instr(long long int address,long long int timestamp){
	bias = BIAS(address); 
	setnum = SETNUM(address);
	tag = TAG(address);
	this->timestamp = timestamp;
}

int Instruction::get_bias(){
	return bias;
}

int Instruction::get_setnum(){
	return setnum;
}

int Instruction::get_tag(){
	return tag;
}

void Instruction::print_instr(){
	printf("*****************************************************\n");
	printf("bias: %d\n", bias); 
	printf("setnum: %d\n", setnum); 
	printf("tag: %d\n", tag);
	printf("*****************************************************");
	printf("\n\n");
}
