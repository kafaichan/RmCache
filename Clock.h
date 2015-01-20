class Clock{
private:
		long long int timestamp;
public:
	Clock(long long int t):timestamp(t){}
	Clock(){}
	
	void IncreaseTick(long long int delta){
		timestamp += delta;
	}

	long long int& getTime(){
		return timestamp;
	}

	void setTick(long long int t){
		timestamp = t;
	}
}cc; 
