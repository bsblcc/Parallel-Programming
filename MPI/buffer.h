#ifndef BUFFER_H

#define BUFFER_H

#include <cstring>
using namespace std;

struct Buffer
{
	char *data;
	int count;
	int max_size;


	void init(int size);



	void finalize();


	void push(char *p, int size);

	void pop(char *p, int size);

	void clear();


};









#endif