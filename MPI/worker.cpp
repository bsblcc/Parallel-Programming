
#include "worker.h"


int const RANK_MASTER = 0;
int const REGISTER_LENGTH = 8;


static Buffer comm_buffer;

Buffer column_buffer, row_buffer;



// describe the current active part of the sub-matrix.
int cord[4];


// describe the frame of the sub-matrix, that is the first "cord"s it got.
int frame[4];


// the progress of the big loop.
int cur_round;

float factor;

float *data;



inline char* adr_of(int x, int y);
inline float& element_of(int x, int y);


inline bool is_irrelevant();
inline bool is_leftmost();
inline bool is_upmost();
inline int rank_down();
inline int rank_right();
inline int rank_up();
inline int rank_left();

inline int sqrt_n_worker();


static void finalize();

void recv_matrix();
void send_result();

void forward_column();
void recv_column();

void forward_row();
void recv_row();

void worker_routine();





void do_division();
void do_elimination();

inline int sqrt_n_worker()
{
	return int(floor(sqrt(n_worker)));
}


inline char* adr_of(int x, int y)
{

	float *res = data + (x - frame[0]) * (frame[3] - frame[2] + 1) + (y - frame[2]);
	return (char *) res;
}

inline float& element_of(int x, int y)
{
	return data[(x - frame[0]) * (frame[3] - frame[2] + 1) + (y - frame[2])];
}



inline bool is_irrelevant()
{
	return (cord[1] < cur_round || cord[3] < cur_round);
}

 
inline bool is_leftmost()
{
	return (cord[2] <= cur_round);
}

inline bool is_upmost()
{
	return (cord[0] <= cur_round);
}

inline int rank_down()
{

	return  (my_rank + sqrt_n_worker() > n_worker) ? (-1) : (my_rank + sqrt_n_worker());
}

inline int rank_right()
{
	
	return (my_rank % sqrt_n_worker() == 0) ? (-1) : (my_rank + 1);
}




inline int rank_up()
{
	return my_rank - sqrt_n_worker();
}

inline int rank_left()
{
	return my_rank - 1;
}


bool is_lower()
{
	int sr = sqrt_n_worker();
	int x = (my_rank / sr);
	int rm = my_rank - sr * x;

	return (rm <= x);


}

static void finalize()
{
	comm_buffer.finalize();
	row_buffer.finalize();
	column_buffer.finalize();
	delete [] data;
	data = nullptr;

}


void recv_matrix()
{


	MPI_Recv(cord, 4 * sizeof(int), MPI_BYTE, RANK_MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	memcpy(frame, cord, 4 * sizeof(int));
	int row_length = cord[3] - cord[2] + 1;
	int column_length = cord[1] - cord[0] + 1;





	comm_buffer.init(sizeof(float) * row_length * column_length + sizeof(int) * 4);
	column_buffer.init(column_length * sizeof(float));
	row_buffer.init(row_length * sizeof(float));


	comm_buffer.clear();
	MPI_Recv(comm_buffer.data, comm_buffer.max_size, MPI_BYTE, RANK_MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	


	data = new float [row_length * column_length];


	comm_buffer.pop((char *)data, row_length * column_length * sizeof(float));

	
}
void send_result()
{


	comm_buffer.clear();

	
	
	// if this part is lower than the diagonal, then there's no need to send the elements.
/*
	if (is_lower())
	{
		int tmp[4] = {-1, -1, -1, -1};
		comm_buffer.push((char *)tmp, sizeof(int) * 4);
	}
	else*/
	{
		int size = (frame[3] - frame[2] + 1) * (frame[1] - frame[0] + 1);
		comm_buffer.push((char *)frame, sizeof(int) * 4);
		comm_buffer.push((char *)data, sizeof(float) * size);
	}
	


	//log("buffer count is %d, and it should be %d\n", comm_buffer.count, 4 * sizeof(int) + sizeof(float) * length * length );

	MPI_Send(comm_buffer.data, comm_buffer.count, MPI_BYTE, RANK_MASTER, 0, MPI_COMM_WORLD);


}




void recv_column()
{
	column_buffer.clear();
	
	MPI_Recv(column_buffer.data, column_buffer.max_size, MPI_BYTE, rank_left(), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	column_buffer.count = (cord[1] - cord[0] + 1) * sizeof(float);


}


void recv_row()
{


	row_buffer.clear();
	MPI_Recv(row_buffer.data, row_buffer.max_size, MPI_BYTE, rank_up(), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	row_buffer.count = (cord[3] - cord[2] + 1) * sizeof(float);
}


void fecth_column()
{
	column_buffer.clear();


	for (int i = cord[0]; i <= cord[1]; i++)
	{
		column_buffer.push(adr_of(i, cord[2]), sizeof(float) * 1);
	}
}


void fecth_row()
{
	row_buffer.clear();
	row_buffer.push(adr_of(cord[0], cord[2]), sizeof(float) * (cord[3] - cord[2] + 1));
}



void forward_column()
{



	int target = rank_right();
	if (target == -1)
	{
		return;
	}

	MPI_Send(column_buffer.data, column_buffer.count, MPI_BYTE, target, 0, MPI_COMM_WORLD);


	
	

}

//18780128501
void forward_row()
{

	int target = rank_down();

	if (target == -1)
	{
		return;
	}

	//log("send row to pid#%d, %f, %f\n", target, element_of(cord[0], cord[2]), element_of(cord[0], cord[3]));
	MPI_Send(row_buffer.data, row_buffer.count, MPI_BYTE, target, 0, MPI_COMM_WORLD);

}

void display(__m256 t)
{
	float tmp[8];

	_mm256_storeu_ps(tmp, t);

	for (int i = 0; i < 8; i++)
	{
		fprintf(stderr, " %f ", tmp[i]);
	}


}


void do_division()
{


	log("start the division\n");
	
	
	__m256 f = _mm256_broadcast_ss((float *)column_buffer.data);
	
	
	
	__m256 a;
	int length = cord[3] - cord[2] + 1;
	float tmp[8];


	//int end = (length % REGISTER_LENGTH == 0) ? (length) : (length - REGISTER_LENGTH);

	int end = (length / REGISTER_LENGTH) * REGISTER_LENGTH;


	#pragma omp parallel for schedule(dynamic) private(a)
	for (int i = 0; i < end; i += REGISTER_LENGTH)
	{
		a = _mm256_loadu_ps((float *)adr_of(cord[0], cord[2] + i));
		


		a = _mm256_div_ps(a, f);
		
		_mm256_storeu_ps((float *)adr_of(cord[0], cord[2] + i), a);

		


	}

	if (end != length)
	{
		
		

		a = _mm256_loadu_ps((float *)adr_of(cord[0], cord[2] + end));

		a = _mm256_div_ps(a, f);

		_mm256_storeu_ps(tmp, a);
		memcpy(adr_of(cord[0], cord[2] + end), tmp, (length - end) * sizeof(float));
	}


	if (cur_round == 10 && my_rank == 2)
	{
		log("cord: %d %d %d %d\n", cord[0], cord[1], cord[2], cord[3]);
	}
}



void do_elimination()
{

	log("start the elimination\n");
	

	float tmp[REGISTER_LENGTH];
	int length = (cord[3] - cord[2] + 1);
	__m256 f, a, b;


	int end = (length / REGISTER_LENGTH) * REGISTER_LENGTH;

	int start;

	char *p = row_buffer.data;

	if (is_upmost())
	{
		start = 1;
	}
	else
	{
		start = 0;
	}


	
	

	#pragma omp parallel for schedule(dynamic) private(tmp, f, a, b)
	for (int i = start; i < (cord[1] - cord[0] + 1); i++)
	{

		//int start = max(cur_row + 1, cord[2]);

		f = _mm256_broadcast_ss((float *)(column_buffer.data + i * sizeof(float)));



		for (int j = 0; j < end; j += REGISTER_LENGTH)
		{

			a = _mm256_loadu_ps((float *)adr_of(cord[0] + i, cord[2] + j));
			

			b = _mm256_loadu_ps((float *)(p + j * sizeof(float)));

			b = _mm256_mul_ps(f, b);

			a = _mm256_sub_ps(a, b);

			_mm256_storeu_ps((float *)adr_of(cord[0] + i, cord[2] + j), a);

		}


		if (end != length)
		{
			a = _mm256_loadu_ps((float *)adr_of(cord[0] + i, cord[2] + end));

			b = _mm256_loadu_ps((float *)(p + end * sizeof(float)));

			b = _mm256_mul_ps(f, b);

			a = _mm256_sub_ps(a, b);

			_mm256_storeu_ps(tmp, a);

			memcpy(adr_of(cord[0] + i, cord[2] + end), tmp, sizeof(float) * (length - end));
		}




	}


	
}




void worker_routine()
{
	for (cur_round = 0; ; cur_round++)
	{
			
		// if the sub-matrix is out of range, then the result of it is final,
		// just send it back to the master.



		if (is_irrelevant())
		{
			//log("ready to send result\n");
			//spin_forever();


			send_result();

			//spin_forever();
			break;
		}

		

		//continue;
		
		// I think this two forward/recv don't neccessarily have to be paralleled.
		// because once the data that we're waiting for is sent out and reach the destination,
		// then we decide to need it and invoke the "recv" function,
		// all the work is just to fecth it from the buffer, which won't take long.
 
		{

			cord[0] = (cord[0] > cur_round) ? cord[0] : cur_round;
			cord[2] = (cord[2] > cur_round) ? cord[2] : cur_round;
			// if the sub-matrix this node holds is currently the leftmost,
			// then it should act first, starting to forward that column to next node.
			if (is_leftmost())
			{

				//log("leftmost\n");
				// update the left boundary
				//cord[2] = cur_round;


				fecth_column();
			}

			// otherwise this node should block to wait for the column of the other node, 
			// before it can make some acitons.
			else
			{

				recv_column();
			}


			forward_column();

			// similarly
			if (is_upmost())
			{
				// if this node is upmost, AND it got the column from the left node,
				// it can do the division.

				// update the upper boundary


				//log("upmost\n");
				//cord[0] = cur_round;

/*
				if (is_leftmost())
				{
					cord[0] = cur_round;
					cord[2] = cur_round;

				}
				*/
				do_division();

				
				fecth_row();

			}
			else
			{
				recv_row();
			}
			forward_row();
		}

		do_elimination();

		
		

	}

}

void worker_start()
{
	
	recv_matrix();
	worker_routine();

	finalize();


}

