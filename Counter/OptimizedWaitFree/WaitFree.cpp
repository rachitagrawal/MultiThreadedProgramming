#include<iostream>
#include"../common/SeqObject.cpp"
#include"Main.cpp"

class WaitFree
{
private:
	//Array to store the head seen by each threads
	Node *head[MAX_THREADS];
	//Array to coordinate helping
	Node *announce[MAX_THREADS];
	Node tail;
public:
	WaitFree()
	{
		for(int i = 0; i < MAX_THREADS; i++)
		{
			head[i] = &tail;
			announce[i] = &tail;
		}
		tail.seq = 1;
		tail.methodName = 0;
	}
	long long apply(long long (SeqObject::*methodName)());
};

long long WaitFree::apply(long long (SeqObject::*methodName)())
{
	int threadId = Main::thread_find();
	announce[threadId] = new Node(methodName);
	Node *current = head[threadId];
	head[threadId] = Node::max(head);
	Node *prefer;

	while(announce[threadId]->seq == 0)
	{
		Node *before = head[threadId];

		int index = (before->seq + 1) % MAX_THREADS;
		Node *help = announce[index];
			
		if(help->seq == 0)
		{
			prefer = help;
		}
		else
		{
			prefer = announce[threadId];
		}
		Node *after = before->decideNext.decide(prefer);
		before->next = after;
		after->seq = before->seq + 1;
		head[threadId] = after;
		counter[threadId]++;
	}

	SeqObject myObject(current->cachedVal);
	current = current->next;

	if(current == NULL)
	{
		cout<<"current is null\n";
	}
	while(current != announce[threadId])
	{
		myObject.apply(current->methodName);
		current = current->next;
	}

	head[threadId] = announce[threadId];

	//Write body to apply the method being passed
	head[threadId]->cachedVal = myObject.apply(current->methodName);
	seqNum[threadId] = current->seq;
	finalCount[threadId] = head[threadId]->cachedVal;
	return head[threadId]->cachedVal;
}
