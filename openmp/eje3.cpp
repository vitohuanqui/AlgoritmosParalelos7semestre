#include <string.h>
#include <iostream>
#include <omp.h>   
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void count_sort(int a[], int n)
{
	int i,j, count;
	int * temp = new int[n];

	for(i=0 ; i<n ; ++i)
	{
		count = 0;
		for(j=0 ; j<n ; ++j)
		{
			if(a[j] < a[i])
				++count;
			else if(a[j] == a[i] && j<i)
				++count;
		}
		temp[count] = a[i];
	}

	memcpy(a, temp, n*sizeof(int));
	free(temp);
}


int main(int argc, char* argv[])
{
	int thread_count = atof(argv[1]); 
	int n, i,j, count, my_rank;
	n = 10;

	int a[10] = {312,4,243,13,13,351,312,2,65,1};
		

	int * temp = new int[n];

	#pragma omp parallel num_threads(thread_count) \
		default(none) \
		private(i, j, count, my_rank) \
		shared(a, n, temp)
		{
			my_rank = omp_get_thread_num();

			#pragma omp for schedule(dynamic, 1)
			for(i=0 ; i<n ; ++i)
			{
				count = 0;
				for(j=0 ; j<n ; ++j)
				{
					if(a[j] < a[i])
						++count;
					else if(a[j] == a[i] && j<i)
						++count;
				}
				temp[count] = a[i];
			}			
		}
	memcpy(a, temp, n*sizeof(int));


	for(int i=0 ; i<n; ++i){
		cout<< a[i] << "-";
	}
	cout << endl;

	return 0; 
}