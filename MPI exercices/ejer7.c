#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


int main(int argc, char **argv)
{
	int comm_sz, i, my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int tmp = 0, npingpongs = 0;
	if(comm_sz != 2)
	{
		if(my_rank==0)
			printf("incorect number of process\n");
		MPI_Finalize();
		return 0;
	}
	if(my_rank == 0)
	{
   		npingpongs = atof(argv[1]);
		MPI_Send(&npingpongs, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		double time=0;
		for(i=0 ; i<npingpongs; i++)
		{
			double start = MPI_Wtime();
			MPI_Send(&tmp, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			printf("Ping\n");
			MPI_Recv(&tmp, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			double fin = MPI_Wtime();
			time += fin - start;
		}
		time /= npingpongs;
		printf("Tiempo promedio: %f\n",time);
	}
	else if(my_rank == 1)
	{
		MPI_Recv(&npingpongs, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(i=0 ; i<npingpongs; i++)
		{
			MPI_Recv(&tmp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Pong\n");
			MPI_Send(&tmp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}