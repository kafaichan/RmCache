#include <iostream>
#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include "Statistics.h"
#include"Clock.h"
#include "Port.h"

#define SETIDX 8192
#define SETSIZE 8

// L2 cache size: 4MB 	cache block size: 512 bits/8 = 64 bytes
// 2^2*2^20/(64*64) = 1024 groups
//there are 1024*8=8192 sets

class RaceTrackMemoryCache{
public:
	int valid[SETSIZE];
	int tags[SETSIZE];
	long long int timestamp[SETSIZE];

	RaceTrackMemoryCache(){
		memset(valid,-1,sizeof(valid));
		memset(tags,0,sizeof(tags)); 
		memset(timestamp,0,sizeof(timestamp));
	}

	bool FindBlock(int tag, long long int timestamp){
		cc.IncreaseTick(1000);  //for the read or write latency(1 cycle=1000 ticks)
		stats.waste_timestamp += 1000; 

		for(int i = 0; i < SETSIZE; i++){
			if(valid[i] != -1 && this->tags[i] == tag){
				this->timestamp[i] = timestamp;
				stats.hit++;		

				int delta = this-rtmcache;
				int setidx = delta >> 10;
				int groupidx = delta - (setidx<<10);

				if(stripe[groupidx].Move(setidx*8+i)){ //shift latency
					cc.IncreaseTick(1000);	
					stats.waste_timestamp += 1000;
				}
				return true;
			}
		}
		stats.miss++;
		return false;
	}

	bool InsertBlock(int tag, long long int timestamp){
		for(int i = 0; i < SETSIZE; i++){
			if(valid[i] < 0){
				valid[i] = 1; 
				this->tags[i] = tag; 
				this->timestamp[i] = timestamp;

				int delta = this-rtmcache;
				int setidx = delta >> 10;
				int groupidx = delta - (setidx<<10);

				if(stripe[groupidx].Move(setidx*8+i)){ //shift latency
					cc.IncreaseTick(1000);	
					stats.waste_timestamp += 1000;
				}
				return true;
			}
		}
		return false;
	}

	void Evict(int tag, long long int timestamp){
		long long int min = cc.getTime(); 
		int replaceidx; 

		for(int i = 0; i < SETSIZE; i++){
			if(this->timestamp[i] < min){
				min = this->timestamp[i]; 
				replaceidx  = i;
			}
		}
		this->timestamp[replaceidx] = timestamp;
		this->tags[replaceidx] = tag; 
		this->valid[replaceidx] = 1;
		
		int delta = this-rtmcache;
		int setidx = delta >> 10;
		int groupidx = delta - (setidx<<10);

		if(stripe[groupidx].Move(setidx*8+replaceidx)){ //shift latency
			cc.IncreaseTick(1000);	
			stats.waste_timestamp += 1000;
		}
	}

	void CacheMiss(int& setnum, int& tag, long long int& timestamp){
		printf("*****************************************************\n");
		printf("Oops Cache Miss\n");
		printf("setnum: %d\n", setnum);
		printf("tag: %d\n", tag); 
		printf("timestamp: %ld\n", timestamp);
		printf("*****************************************************");
		printf("\n\n");
	}

	void CacheHit(int& setnum, int& tag, long long int& timestamp){
		printf("*****************************************************\n");
		printf("Cache Hit\n");
		printf("setnum: %d\n", setnum);
		printf("tag: %d\n", tag); 
		printf("timestamp: %ld\n", timestamp);
		printf("*****************************************************");
		printf("\n\n");		
	}
}rtmcache[SETIDX]; 



