#ifndef _TRACES_
#define _TRACES_
#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;

class Traces
{
	public:
	void generate(int maxThreads, int n)
	{
		int i, j;
		int num;
		ofstream myFile;
		char fileName[25];

		//open file
		for(j = 0; j < maxThreads; j++)
		{
			//Generate the file name
			sprintf(fileName, "./common/trace%d.txt", j);

			myFile.open(fileName);

			//Writing random inputs (method names) into the file
			for(i = 0; i < n; i++)
			{
				num = rand()%2;

				//write into the file
				myFile<<num<<endl;
			}

			myFile.close();
		}

	}

	void read(int *traces, int n)
	{
		int id = Main::thread_find(), i;
		char fileName[25];
		ifstream myFile;

		//Reading the file corresponding the thread id
		sprintf(fileName, "./common/trace%d.txt", id);
		myFile.open(fileName);
	
		for(i = 0; i < n; i++)
		{
			myFile>>traces[i];
		}
		myFile.close();
	}
};
#endif
