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

	Node()
	{
		Node(0);
	}

	Node(long long (SeqObject::*methodName)())
	{
		this->methodName = methodName;
		seq = 0;
	}

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
