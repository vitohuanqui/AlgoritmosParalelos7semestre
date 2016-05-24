#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int find_bin(double data, double* bin_maxes, int nbins){
    for (int i=0;i<nbins-1;i++){
        if ((bin_maxes[i] <= data) && (data <= bin_maxes[i+1])){
            return i;
        }
    }
}

int main(int argc, char const *argv[])
{
	int n, nbins;
    double bin_width;

    n = argc-1;
    nbins = 4;
    double max = -1000000, min = 1000000000;
    double data[n];
    double* bin_maxes = new double[nbins+1];
    double* bin_counts = new double[nbins];
    for(int j = 0; j < nbins; ++j){
        bin_counts[j] = 0;
    }
    for (int i=0; i<n; i++){
        data[i] = atof(argv[i+1]);
        if (data[i]>max)
            max = data[i];
        if (data[i]<min)
            min = data[i];
    } 
    bin_width = (max - min) / nbins;
    for (int i=0; i<nbins+1;i++){
        bin_maxes[i] = min + bin_width*(i);
    }
    # pragma omp parallel num_threads(4)
    {
    	int k = omp_get_thread_num();
    	int j = omp_get_num_threads();
    	for (int i = k*(n/j); i < (k+1)*(n/j); ++i)
    	{
    		int b = find_bin(data[i],bin_maxes,nbins+1);
    		# pragma omp critical
    		bin_counts[b]++;
    	}
	}
    for (int i = 0; i < nbins; ++i)
    {
    	printf("%d->%f\n", i, bin_counts[i]);
    }
	return 0;
}