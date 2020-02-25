#include "gaussian_omp.h"

#include <cstdio>

inline int index_of(int x, int y)
{
	return x * size + y;
}


inline int x_of(int index)
{
	return index / ((size - cur_row - 1) / REGISTER_LENGTH + 1) + cur_row + 1;
}


inline int y_of(int index)
{
	return (index % ((size - cur_row - 1) / REGISTER_LENGTH + 1)) * REGISTER_LENGTH + 1 + cur_row;
}

void read()
{
	ifstream fin("input.txt");


	fin >> size;

	data = new float [size * size + 2 * REGISTER_LENGTH];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fin >> data[index_of(i, j)];
		}
	}
}


void init()
{
	read();

}

void finalize()
{
	if (data != nullptr)
	{
		delete [] data;
		data = nullptr;

	}
}


void write()
{
	ofstream fout("omp.out");
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
				fout << data[index_of(i, j)] << " ";
			}
			
		}

		fout << endl;
	}	


}





void gaussian()
{
	timeval tv;
 	gettimeofday(&tv, NULL);
 	long long ts1 = tv.tv_sec*1000*1000 + tv.tv_usec;


 	//#pragma omp parallel 

	for (cur_row = 0; cur_row < size; cur_row++)
	{

		int i;
		__m256 b, a;
		int x, y;
		float tmp[REGISTER_LENGTH];



		// do the first step 

//
		//cerr << cur_row << endl;
		//fprintf(stderr, "REGISTER_LENGTH: %d, size: %d\n", REGISTER_LENGTH, size);

		b = _mm256_broadcast_ss((float*)data + index_of(cur_row, cur_row));

		#pragma omp parallel for schedule(static) private(a)
		for (i = cur_row + 1; i < size - REGISTER_LENGTH; i += REGISTER_LENGTH)
		{

			a = _mm256_loadu_ps((float*)data + index_of(cur_row, i));
			a = _mm256_div_ps (a, b);
			_mm256_storeu_ps((float*)data + index_of(cur_row, i), a);

		}
		i = cur_row + 1 + ((size - cur_row - 1) / REGISTER_LENGTH) * REGISTER_LENGTH;
		if (i == size)
		{
			i -= REGISTER_LENGTH;
			a = _mm256_loadu_ps((float*)data + index_of(cur_row, i));
			a = _mm256_div_ps (a, b);
			_mm256_storeu_ps((float*)data + index_of(cur_row, i), a);
		}
		else
		{
			a = _mm256_loadu_ps((float*)data + index_of(cur_row, i));
			a = _mm256_div_ps (a, b);

			_mm256_storeu_ps(tmp, a);


			memcpy((float*)data + index_of(cur_row, i), tmp, (size - i) * sizeof(float));
		}

		
		

		// second step


		int n_blocks = (size - cur_row - 1) * ((size - cur_row - 1) / REGISTER_LENGTH + 1);




		#ifdef BY_ROW

		__m256 c;

			#ifdef DYNAMIC
		#pragma omp parallel for schedule(dynamic) private(x, y, a, b, tmp) 

			#else
				#ifdef STATIC
		#pragma omp parallel for schedule(static) private(x, y, a, b, tmp) 
				#else
		#pragma omp parallel for schedule(guided) private(x, y, a, b, tmp)
				#endif
			#endif

		for (x = cur_row + 1; x < size; x += 1)
		{

			b = _mm256_broadcast_ss((float*)data + index_of(x, cur_row));

			for (y = cur_row + 1; y < size - REGISTER_LENGTH; y += REGISTER_LENGTH)
			{
				a = _mm256_loadu_ps((float*)(data + index_of(cur_row, y)));
				a = _mm256_mul_ps(a, b);
				c = _mm256_loadu_ps((float*)data + index_of(x, y));
				c = _mm256_sub_ps(c, a);
				_mm256_storeu_ps((float*)data + index_of(x, y), c);
			}

			if (y == size)
			{
				y -= REGISTER_LENGTH;
				a = _mm256_loadu_ps((float*)(data + index_of(cur_row, y)));
				a = _mm256_mul_ps(a, b);
				c = _mm256_loadu_ps((float*)data + index_of(x, y));
				c = _mm256_sub_ps(c, a);
				_mm256_storeu_ps((float*)data + index_of(x, y), c);
			}
			else
			{

				a = _mm256_loadu_ps((float*)(data + index_of(cur_row, y)));
				a = _mm256_mul_ps(a, b);
				c = _mm256_loadu_ps((float*)data + index_of(x, y));
				c = _mm256_sub_ps(c, a);

				_mm256_storeu_ps(tmp, c);
				memcpy((float*)data + index_of(x, y), tmp, sizeof(float) * (size - y));
			}

		}

		#else

			#ifdef DYNAMIC
		#pragma omp parallel for schedule(dynamic) private(x, y, a, b, tmp) 

			#else
				#ifdef STATIC
		#pragma omp parallel for schedule(static) private(x, y, a, b, tmp) )
				#else
		#pragma omp parallel for schedule(guided) private(x, y, a, b, tmp) 
				#endif
			#endif
		for (i = 0; i < n_blocks; i += 1)
		{
			//fprintf(stderr, "tid: %d, i: %d, a\n", omp_get_thread_num(), i);

			//fprintf(stderr, "a\n");
			int x = x_of(i);
			int y = y_of(i);

			a = _mm256_loadu_ps((float*)(data + index_of(cur_row, y)));
			b = _mm256_broadcast_ss((float*)data + index_of(x, cur_row));
			//fprintf(stderr, "b\n");
			a = _mm256_mul_ps(a, b);
			b = _mm256_loadu_ps((float*)data + index_of(x, y));

			b = _mm256_sub_ps(b, a);

			//fprintf(stderr, "c\n");
			if (size - y < REGISTER_LENGTH)
			{
				
				_mm256_storeu_ps(tmp, b);
				memcpy((float*)data + index_of(x, y), tmp, (size - y) * sizeof(float));
			}
			else
			{
				_mm256_storeu_ps((float*)data + index_of(x, y), b);
			}
			//fprintf(stderr, "d\n");

		}

		#endif
		//break;
	}


	gettimeofday(&tv, NULL);
	long long ts2 = tv.tv_sec*1000*1000 + tv.tv_usec;
	run_time_us = ts2 - ts1;



}






int main(int argc, char **argv)
{


	if (argc < 3)
	{
		N_THREAD = 7;
	}
	else 
	{
		N_THREAD = atoi(argv[2]);
	}

	if (argc < 2)
	{
		BLOCK_LENGTH = 1024;
	}
	else
	{
		BLOCK_LENGTH = atoi(argv[1]);
	}
	init();

	gaussian();
	write();
	finalize();


	cout << run_time_us << endl;

	exit(0);
}