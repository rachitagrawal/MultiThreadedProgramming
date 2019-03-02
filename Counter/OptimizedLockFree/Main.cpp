#ifndef _MAIN_
#define _MAIN_
#include<iostream>
#include<pthread.h>
#include<cstdlib>
#include<sys/time.h>
#include"../common/INIT.H"

using namespace std;

pthread_barrier_t barrier; 
int ret; 
//Array to store the local counts of each thread
long counter[MAX_THREADS];

//Array to store the local number of nodes traversed by each thread
long seqNum[MAX_THREADS];
//Final Counter
long long finalCount[MAX_THREADS];

//Array to store the local time of each thread
time_t times[MAX_THREADS];

void *Go(void *threadid);
long long findTotal(long *counter);
void printTimes();
int findLastNode(long *seqNum);
time_t findMax(time_t *times);
time_t calculate_time_difference(struct timeval time1, struct timeval time2);
class Main{

	//Mutex object to lock the threadlist
	static pthread_mutex_t threadlistlock;
	//Array to contain the id of the threads
	static pthread_t threads[];
	public:
		
	void start()
	{
		int t, rc;
		void *status;
		pthread_attr_t attr;

		cout<<"MAX_THREADS:"<<MAX_THREADS<<" N:"<<N<<endl;
		//initialize and set thread detach state
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		//Number of threads to wait for
		unsigned count = MAX_THREADS;

		//Initializing barrier
		pthread_barrier_init(&barrier, NULL, count);

		for(t = 0; t < MAX_THREADS; t++)
		{
			rc = pthread_create(&(threads[t]), &attr, Go, NULL);
			if(rc) //Some Error
			{
				cout<<"Error code:"<<rc<<endl;
				exit(-1);
			}
		}
		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);
   		for(t=0; t<MAX_THREADS; t++)
   		{
      		rc = pthread_join(threads[t], &status);
      		if (rc)
      		{
         		printf("ERROR; return code from pthread_join() is %d\n", rc);
         		exit(-1);
      		}
   		}
		for(int i =0; i < MAX_THREADS; i++)
		{
			cout<<"Number of execution of while loop by thread "<<i<<" is:"<<counter[i]<<endl;
		}
		cout<<"Average Number of loop executions before a call is linearized:"<<(double)findTotal(counter)/(N*MAX_THREADS)<<endl;
		cout<<"Final Counter Value:"<<finalCount[findLastNode(seqNum)]<<endl;
		cout<<"Total time Taken by all the threads:"<<(double)(findMax(times)/1000.0)<<" msec"<<endl;
		pthread_exit(NULL);
	}
	
	//Gives the unique integer thread ids
	static int thread_find()
	{
		int thread = -1;
		pthread_t self = pthread_self();

		for(int i = 0; i < MAX_THREADS; i++)
		{
			if(pthread_equal(self, threads[i]))
			{
				thread = i;
				break;
			}
		}
		return thread;
	}
};
pthread_mutex_t Main::threadlistlock;
pthread_t Main::threads[MAX_THREADS];

#include"Node.cpp"
#include"../common/Traces.cpp"
#include"LockFree.cpp"
Node tail;
LockFree LFU;


void *Go(void *threadid)
{
	int tid, i;
	
	long long result;
	
	//Function pointer to the methods in the Sequential Objects
	long long (SeqObject::*methodName)() = NULL;

	//Thread Id
	tid = Main::thread_find();

	struct timeval start_time, end_time;
	//Trace object to store the traces
	Traces traces;
	//Array to store the traces of the current thread
	int myTraces[N];

	traces.read(myTraces, N);

	//Waiting for all threads	
	ret = pthread_barrier_wait(&barrier);

	counter[tid] = 0;
	seqNum[tid] = 0;

	gettimeofday(&start_time, NULL);

	for(i = 0; i < N; i++)
	{
		if(myTraces[i] == INCREMENT)
		{
			methodName = &SeqObject::increment;
		}
		else
		{
			methodName = &SeqObject::decrement;
		}

		result = LFU.apply(methodName);
	}

	gettimeofday(&end_time, NULL);
	times[tid] = calculate_time_difference(start_time, end_time);
	pthread_exit(NULL);
}

//Calculates the time difference(in microsecond) between the two input time structures
time_t calculate_time_difference(struct timeval time1, struct timeval time2)
{
	time_t time_difference;


 	if(time2.tv_sec == time1.tv_sec)
    {
         time_difference = time2.tv_usec - time1.tv_usec;
    }
    else
    {
        time_difference = (time2.tv_sec - time1.tv_sec - 1) * 1000000 + 1000000 - time1.tv_usec + time2.tv_usec;
    }

	return time_difference;
}

long long findTotal(long *counter)
{
	int i;

	long long total = 0;

	for(i = 0; i < MAX_THREADS; i++)
	{
		total += counter[i];
	}
	
	return total;
}

time_t findMax(time_t *times)
{
	int i;

	time_t max = times[0];

	for(i = 1; i < MAX_THREADS; i++)
	{
		if(max < times[i])
		{
			max = times[i];
		}
	}

	return max;
}

int findLastNode(long *array)
{
	int i;

	int max = 0;

	for(i = 0; i < MAX_THREADS; i++)
	{
		if(array[max] < array[i])
		{
			max = i;
		}
	}

	return max;
}


#endif
