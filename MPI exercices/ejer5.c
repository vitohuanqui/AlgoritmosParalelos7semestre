#include <iostream>
#include <mpi.h>
#include <cstdlib>

int main(int argc, char * argv[])
{
	int comm_size, my_rank;
	int* data = NULL, local_total;
	int divisions, length;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if(my_rank == 0)
	{		
		length = argc-1;

		data = (int*) malloc((length)*sizeof(int));
		divisions = length/comm_size;

		for (int i=0; i<length; i++){
            data[i] = atof(argv[i+1]);
        }
	}
	
	MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&divisions, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(!data)
		data =  (int*) malloc((length)*sizeof(int));

	MPI_Bcast(data, length, MPI_INT, 0, MPI_COMM_WORLD);

	local_total = 0;
	for(int i=my_rank*divisions ; i<((my_rank+1)*divisions) ; ++i)
		local_total += data[i];

	int odd_total;

	for(int j=1,f; j<comm_size;j<<=1)
	{		
		if(!((my_rank/j)&1))
		{
			MPI_Sendrecv(&local_total, 1, MPI_INT, my_rank+j, 0, 
				&odd_total, 1, MPI_INT, my_rank+j, 0, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else
		{
			MPI_Sendrecv(&local_total, 1, MPI_INT, abs(my_rank-j), 0, 
				&odd_total, 1, MPI_INT, abs(my_rank-j), 0, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		local_total += odd_total;
	}

	if(my_rank == 0)
		printf("Total = %d \n", local_total);

	MPI_Finalize();

	return 0;
}