#include <omp.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <immintrin.h>
#include <sys/time.h>
using namespace std;



float *data;


void init();


void read();
void finalize();


int N_THREAD;


const int REGISTER_LENGTH = 8;

int BLOCK_LENGTH;
int cur_row;


long long run_time_us;

int size;


