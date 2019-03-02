#ifndef _NODE_
#define _NODE_
#include <iostream>
#include "../common/Consensus.cpp"
#include "../common/SeqObject.cpp"
class Node
{
	public:
	long long (SeqObject::*methodName)();
	Consensus<Node> decideNext;
	Node* next;
	int seq;
	long long cachedVal;

	Node()
	{
		Node(0);
	}

	Node(long long (SeqObject::*methodName)())
	{
		this->methodName = methodName;
		cachedVal = 0;
		seq = 0;
	}

/*	long long (SeqObject::*method()) getMethodName()
	{
		return methodName;
	}*/

	static Node* max(Node* array[])
	{
		Node *max = array[0];
		for(int i = 1; i < MAX_THREADS; i++)
		{
			if(max->seq < array[i]->seq)
			{
				max = array[i];
			}
		}
		return max;
	}

};
#endif
