#ifndef _CONSENSUS_
#define _CONSENSUS_
#include<iostream>
#include<pthread.h>
using namespace std;

template <class T> class Consensus
{
	private:
	T* proposed[MAX_THREADS];
	public:
	int FIRST;
	int r;

	//Default Constructor
	Consensus()
	{
		FIRST = -1;
		r = FIRST;
	}

	void propose(T *value)
	{
		proposed[Main::thread_find()] = value;
	}
	
	T* decide(T *value)
	{
		propose(value);
		int i = Main::thread_find();
		if(compare_and_exchange(r, FIRST, i))
		{
			return proposed[i];
		}
		else
		{
			return proposed[r];
		}
	}

	inline static bool compare_and_exchange(volatile int& dest, int old_val, int new_val)
	{
		bool result;
		__asm__ __volatile__(
					"lock cmpxchgl %3, %1; setzb %0"
					: "=qm"(result), "+m" (dest)
					: "a" (old_val), "r" (new_val)
					: );
		return result;
	}
};
#endif
