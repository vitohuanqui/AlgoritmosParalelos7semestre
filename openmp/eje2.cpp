#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


 int main(int argc, char* argv[])
 {
 	int thread_count = atof(argv[1]);

 	double pi=0, x, y, distance_squared;
 	int i, number_of_tosses=14, number_in_circle=0, divisions;
	srand(time(NULL));


 	divisions = number_of_tosses/thread_count;

	#  pragma omp parallel num_threads(thread_count)\
 	default(none) reduction(+:pi)\
 	private(number_in_circle, x, y, distance_squared, i)\
 	shared(number_of_tosses, divisions, thread_count)
 	{
 		for(i=0 ; i<number_of_tosses ; ++i)
 		{
 			x = (rand()%2)-1;
 			y = (rand()%2)-1;
 			distance_squared = x*x + y*y;
 			if(distance_squared <= 1)
 				++number_in_circle;
 		}	
 		pi = number_in_circle/((double)number_of_tosses);
 	}
 	
 	printf("Valor de pi: %f\n", pi);

 	return 0; 
 }