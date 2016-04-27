#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *a, int *b, int l, int m, int r) 
{	
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;	
	while((h <= m) && (j <= r)) 
	{	
		if(a[h] <= a[j]) 
		{	
			b[i] = a[h];
			h++;	
		}	
		else 
		{	
			b[i] = a[j];
			j++;
		}	
		i++;
		
	}	
	if(m < h) 
	{	
		for(k = j; k <= r; k++) 
		{	
			b[i] = a[k];
			i++;	
		}	
	}
	else 
	{
		for(k = h; k <= m; k++) 
		{
			b[i] = a[k];
			i++;
		}	
	}
	for(k = l; k <= r; k++) 
		a[k] = b[k];
}

void mergeSort(int *a, int *b, int l, int r) 
{
	int m;
	if(l < r) 
	{
		m = (l + r)/2;
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
	}	
}
int main(int argc, char** argv)
{
	int n = atof(argv[1]);
	int *array = malloc(n * sizeof(int));
	
	srand(time(NULL));
	for(int i = 0; i < n; i++)
	{	
		array[i] = rand() % n;
	}

	int my_rank;
	int comm_sz;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	int size = n/comm_sz;
	
	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	int *tmp_array = malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));
	
	int *sorted = NULL;
	if(my_rank == 0) 
		sorted = malloc(n * sizeof(int));

	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

	if(my_rank == 0) 
	{	
		int *other_array = malloc(n * sizeof(int));
		mergeSort(sorted, other_array, 0, (n - 1));
		printf("MergeSort con %d threads : " ,comm_sz);
		for(c = 0; c < n; c++) 
			printf("%d ", sorted[c]);
		printf("\n");
		//Limpia la raiz
		free(sorted);
		free(other_array);
	}

	//Limpia el resto
	free(array);
	free(sub_array);
	free(tmp_array);
	//Finaliza mpi
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();	
}