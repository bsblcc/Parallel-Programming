#include <ctime>
#include <cstdlib>
#include <iostream>


#include <fstream>
#include <cstring>



using namespace std;


const char *output_file_name = "input.txt";
const int precision = 10000;
int size;

inline static int generate_rand()
{
	return rand() % 10000;
}



static void generate_matrix()
{
	srand((unsigned) time(0));

	ofstream fout(output_file_name);

	fout << size << endl;

	float c = 0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fout << generate_rand()  << " " ;
			//fout << ++c  << " " ;
		}

		fout << endl;
	}
}



int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cerr << "invalid arg number" << endl;
		return 0;
	}

	size = atoi(argv[1]);

	if (size == 0)
	{
		cerr << "invalid matrix size" << endl;
		return 0;
	}

	generate_matrix();


	return 0;

}