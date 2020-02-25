#include <omp.h>
#include <iostream>




using namespace std;



int main()
{
	#pragma omp parallel
	for (int i = 0; i < 10; i++)
	{

		


		#pragma omp for
		for (int j = 0; j < 5; j ++)
		{

			#pragma omp critical
			{
				cout << omp_get_thread_num() << " " << i << " ";

				cout << j << endl;
			}
			
		}

	}


	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;


	exit(0);
}