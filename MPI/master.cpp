
#include "master.h"


Matrix matrix;

static Buffer comm_buffer;

	

inline int sqrt_n_worker();

void daemonize_write();
static void finalize();

// distribute the parts of the matrix to every worker node.
void distribute_matrix();


void write_result();


// routine of each round.
void round_routine();

inline int sqrt_n_worker()
{
	return int(floor(sqrt(n_worker)));
}

void daemonize_result()
{


	int cord[4];

	for (int i = 0; i < n_worker; i++)
	{
		comm_buffer.clear();

		MPI_Recv(comm_buffer.data, comm_buffer.max_size, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//log("got a result\n");
		comm_buffer.pop((char *)cord, 4 * sizeof(int));

		if (cord[0] == -1)
		{
			continue;
		}
		int length = (cord[3] - cord[2] + 1);
		for (int k = cord[0]; k <= cord[1]; k++)
		{

			comm_buffer.pop((char *)matrix.addr_of(k, cord[2]), length * sizeof(float));
		}
	}




}


void write_result()
{
	matrix.write();
}

void distribute_matrix()
{

	int cur_node = 1;
	int cord[4];
	int length_per_node = matrix.size / sqrt_n_worker();
	int max_length_per_node = ((matrix.size - 1) - ((sqrt_n_worker() - 1) * length_per_node) + 1);
	int buffer_size = (max_length_per_node) * (max_length_per_node) * sizeof(float) + 4 * sizeof(int);
	comm_buffer.init(buffer_size);
	for (int i = 0; i < sqrt_n_worker(); i++)
	{
		cord[0] = i * length_per_node;
		cord[1] = (i == sqrt_n_worker() - 1) ? (matrix.size - 1) : (cord[0] + length_per_node - 1);
		for (int j = 0; j < sqrt_n_worker(); j++)
		{
			cord[2] = j * length_per_node;
			cord[3] = (j == sqrt_n_worker() - 1) ? (matrix.size - 1) : (cord[2] + length_per_node - 1);


			MPI_Send(cord, 4 * sizeof(int), MPI_BYTE, cur_node, 0, MPI_COMM_WORLD);


			int size = (cord[1] - cord[0] + 1) * (cord[3] - cord[2] + 1);
			int length = (cord[3] - cord[2] + 1);

			comm_buffer.clear();

			int s = 0;
			for (int k = cord[0]; k <= cord[1]; k++)
			{

				comm_buffer.push((char *)matrix.addr_of(k, cord[2]), sizeof(float) * length);
				s += length * sizeof(float);
			}



			MPI_Send(comm_buffer.data, comm_buffer.count, MPI_BYTE, cur_node, 0, MPI_COMM_WORLD);

			cur_node++;
			
		}
	}



}


static void finalize()
{
	comm_buffer.finalize();
}

void master_start()
{
	


	//pthread_create(&pid, nullptr, daemonize_result, nullptr); 
	
    matrix.init();


	distribute_matrix();

	//MPI_Barrier(MPI_COMM_WORLD);

	timeval tv;
	gettimeofday(&tv, NULL);
	long long ts1 = tv.tv_sec*1000*1000 + tv.tv_usec;

	daemonize_result();

	gettimeofday(&tv, NULL);
	long long ts2 = tv.tv_sec*1000*1000 + tv.tv_usec;
	long long run_time_us = ts2 - ts1;
	write_result();

	finalize();

	printf("%lld\n", run_time_us);

}