
#include "matrix.h"
float* Matrix::addr_of(int x, int y)
{
	return data + index_of(x, y);
}

inline int Matrix::index_of(int x, int y)
{
	return (x * size + y);
}


inline float& Matrix::element_of(int x, int y)
{
	return data[index_of(x, y)];
}



void Matrix::write()
{
	ofstream fout("mpi.out");



	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
			{
				fout << "1 ";
			}
			else if (i > j)
			{
				fout << "0 ";
			}
			else
			{
				fout << element_of(i, j) << " ";
			}

			
		}
		fout << endl;
	}
}

void Matrix::init()
{
	ifstream fin("input.txt");


	fin >> size;

	data = new float [size * size];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fin >> element_of(i, j);
		}
	}
}