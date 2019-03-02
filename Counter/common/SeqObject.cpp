#ifndef _SEQ_OBJECT_
#define _SEQ_OBJECT_
#define INCREMENT 0
#define DECREMENT 1

class SeqObject
{
	long long C;
	public:

	SeqObject()
	{
		C = 0;
	}

	SeqObject(long long count)
	{
		C = count;
	}

	long long apply(long long (SeqObject::*methodName)())
	{
		long long result = (*this.*methodName)();
		
		return result;
	}

	long long increment()
	{
		return ++C;
	}

	long long decrement()
	{
		return --C;
	}
};
#endif
