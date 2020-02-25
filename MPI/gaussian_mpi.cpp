






#include "gaussian_mpi.h"

int n_worker;

int my_rank;


int main(int argc, char **argv)
{



    MPI_Init(&argc, &argv);
    

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_worker);
	n_worker -= 1; 


    if (my_rank == 0)
    {
    	
    	master_start();
    }
    else
    {
    	worker_start();
    }


    MPI_Finalize();


    return 0;


}