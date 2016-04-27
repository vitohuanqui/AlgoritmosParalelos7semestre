/*Multimplicacion de una matriz con un vector*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(void)
{
	int n = 4;
	int vector[4]={23,43,32,5};
	int matrix[4][4] = {{1, 0, 0, 0},
						{0, 1, 0, 0},
						{0, 0, 1, 0},
						{0, 0, 0, 1}};
	int comm_sz, my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int size=comm_sz-1;
	if(size!=n)
	{
		if (!my_rank)
			printf("FATAL ERROR: there are not enough threads\n");
		MPI_Finalize();
		return 0;
	}
	int size_submatrix=sqrt(n);
	int acum;
	if (my_rank < size)
	{
		int l=0;
		for (int i = size_submatrix*((my_rank/size_submatrix)%size_submatrix); i < size_submatrix*(((my_rank/size_submatrix)%size_submatrix)+1); ++i)
		{
			acum = 0;
			for (int j = size_submatrix*(my_rank%size_submatrix); j < size_submatrix*((my_rank%size_submatrix)+1); j++)
				acum += matrix[i][j]*vector[j];
			MPI_Send(&acum, 1, MPI_INT, size, my_rank*size_submatrix+l,MPI_COMM_WORLD);
			++l;
		}
	}
	else
	{
		int vector[n*size_submatrix];
		for (int k=0; k<n*size_submatrix; ++k)
		{
			int domain=k/size_submatrix;
			MPI_Recv(&acum, 1, MPI_INT, domain,k, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			vector[k]=acum;
		}
		for (int i = 0; i < size; i+=2)
		{
			printf("%d- %d = %d \n", vector[i],vector[i+1],vector[i]+vector[i+1]);
		}
	}
	MPI_Finalize();
	return 0;
}