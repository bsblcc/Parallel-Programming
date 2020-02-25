#include <iostream>
#include <fstream>

#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/time.h>
#include <cstdlib>
#include <cstring>
using namespace std;
ifstream fin("input.txt");
ofstream fout("raw.out");

int size;
float *data;

long long run_time_us;
float& at(int x, int y)
{
	return data[x * size + y];
}
void read()
{
	fin >> size;
	data = new float [size * size];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fin >> at(i, j);
		}
	}
}
void write()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fout << at(i, j) << " ";
		}

		fout << endl;
	}
}
void gaussian_raw()
{


	timeval tv;
 	gettimeofday(&tv, NULL);
 	long long ts1 = tv.tv_sec*1000*1000 + tv.tv_usec;

	for (int i = 0; i < size; i++)
	{

		for (int j = i + 1; j < size; j++)
		{
			//matrix[i][j] /= matrix[i][i];

			at(i, j) /= at(i, i);
		}
		at(i, i) = 1;
		
		for (int j = i + 1; j < size; j++)
		{
			for (int k = i + 1; k < size; k++)
			{
				at(j, k) -= at(j, i) * at(i, k);
			}
			at(j, i) = 0;

		}


		//break;
		
	}


	gettimeofday(&tv, NULL);
	long long ts2 = tv.tv_sec*1000*1000 + tv.tv_usec;
	run_time_us = ts2 - ts1;
}




int main()
{
	read();
	gaussian_raw();
	write();


	printf("%lld\n", run_time_us);
	return 0;
}