#include "buffer.h"


void Buffer::init(int size)
{
	count = 0;
	max_size = size;
	data = new char [size];
}



void Buffer::finalize()
{

	delete [] data;
	data = nullptr;
}


void Buffer::push(char *p, int size)
{
	memcpy(data + count, p, size);
	count += size;
}

void Buffer::pop(char *p, int size)
{
	memcpy(p, data + count, size);
	count += size;
}

void Buffer::clear()
{
	count = 0;
}