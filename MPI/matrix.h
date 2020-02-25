#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>

using namespace std;


struct Matrix
{
	int size;

	float *data;
	float* addr_of(int x, int y);
	inline float& element_of(int x, int y);
	inline int index_of(int x, int y);


	void init();
	void write();
	
};




#endif