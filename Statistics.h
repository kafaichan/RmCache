class Statistics{
public:
	long long int hit; 
	long long int miss; 
	long long int waste_timestamp; 
	long long int stall_instr;
	long long int rtype_instr; 
	long long int wtype_instr; 
	long long int utype_instr;

	Statistics():hit(0), miss(0), waste_timestamp(0),rtype_instr(0), wtype_instr(0), utype_instr(0),stall_instr(0){}
	
	void PrintReport(){
		printf("****************************************************\n"); 
		printf("Hit: %lld\n", hit); 
		printf("Miss: %lld\n", miss); 
		printf("Waste Timestamp: %lld\n", waste_timestamp); 
		printf("Read Type Instruction: %lld\n", rtype_instr); 
		printf("Write Type Instruction: %lld\n", wtype_instr); 
		printf("Unknown Type Instruction: %lld\n", utype_instr); 
		printf("****************************************************\n\n");
	}
}stats;
