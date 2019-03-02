#include<iostream>
#include"Main.cpp"
using namespace std;

int main()
{
	Main testMain;
	Traces traces;
	traces.generate(MAX_THREADS, N);
	testMain.start();

	pthread_exit(NULL);
	return 0;
}

