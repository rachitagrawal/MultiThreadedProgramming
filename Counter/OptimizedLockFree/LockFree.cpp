#include<iostream>
#include"../common/SeqObject.cpp"
#include"Main.cpp"

class LockFree
{
private:
	Node *head[MAX_THREADS];
	Node tail;
public:
	LockFree()
	{
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
	Node *current = head[threadId];
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

	SeqObject counter(current->cachedVal);
	current = current->next;

	while(current != prefer)
	{
		counter.apply(current->methodName);
		current = current->next;
	}


	//Write body to apply the method being passed
	head[threadId]->cachedVal = counter.apply(current->methodName);
	finalCount[threadId] = head[threadId]->cachedVal;
	seqNum[threadId] = current->seq;
	return head[threadId]->cachedVal;
}

