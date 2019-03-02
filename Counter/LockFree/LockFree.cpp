#include<iostream>
#include"../common/SeqObject.cpp"
#include"Node.h"
class LockFree
{
private:
	Node *head[MAX_THREADS];
	Node tail;
public:
	int check;
	LockFree()
	{
		check=0;
		for(int i = 0; i < MAX_THREADS; i++)
		{
			head[i] = &tail;
		}
		tail.seq = 1;
		tail.methodName = 0;
	}
	long long apply(long long (SeqObject::*methodName)());
};

long long LockFree::apply(long long (SeqObject::*methodName)())
{
	int threadId = Main::thread_find();

	Node *current;
	Node *prefer = new Node(methodName);
	
	while(prefer->seq == 0)
	{
		Node *before = Node::max(head);
		Node *after = before->decideNext.decide(prefer);
		before->next = after;
		after->seq = before->seq + 1;
		head[threadId] = after;
		counter[threadId]++;
	}

	SeqObject Counter;
	current = tail.next;

	while(current != prefer)
	{
		Counter.apply(current->methodName);
		current = current->next;
	}

	//Write body to apply the method being passed
	long long result = Counter.apply(current->methodName);
	finalCount[threadId] = result;
	seqNum[threadId] = current->seq;
	return result;
}

