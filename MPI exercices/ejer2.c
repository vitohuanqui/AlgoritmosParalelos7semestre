#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{
	const long long int limit=2000000;
	int REC=limit/2;
	int comm_sz, my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int toss_darts=atoi(argv[1]);
	if(toss_darts%comm_sz!=0 )
	{
		if(my_rank == 0)
			printf("error\n");
		MPI_Finalize();
		return 0;
	}
	int rec=0;
	int size = toss_darts/comm_sz;
	int local_sum=0;
	int number_in_circle=0;
	srand(time(NULL));
	for(int i=0;i<size;i++)
	{
		double x=((double)  (rand()%limit))/REC-1;
		double y=((double)  (rand()%limit))/REC-1;
		double distance_square=(x*x)+(y*y);
		if(distance_square <=1)
			local_sum++;
	}
	MPI_Reduce(&local_sum,&number_in_circle,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	if (my_rank == 0)
	{
		float pi_estimate = (4.0*number_in_circle)/(toss_darts);
		printf("%f\n", pi_estimate);
	}
	MPI_Finalize();
	return 0;
}