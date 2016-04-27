#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void get_results(int value, int my_rank, int comm_sz, MPI_Comm comm){
    int* values;
    if (my_rank == 0) {
        values = (int *) malloc(comm_sz*sizeof(int));
        MPI_Gather(&value, 1, MPI_INT, values, 1, MPI_INT, 0, comm);
        for (int q = 0; q < comm_sz; q++)
            printf("Thread %d -> val = %d\n", q, values[q]);
        printf("\n");
    }
    else{
        MPI_Gather(&value, 1, MPI_INT, values, 1, MPI_INT, 0, comm);
    }
}  

int Global_sum(int val, int my_rank, int comm_sz, MPI_Comm comm) {
    int sum = val, tmp, partner;
    unsigned bitmask = 1;

    while (bitmask < comm_sz) {
        partner = my_rank ^ bitmask;
        MPI_Sendrecv(&sum, 1, MPI_INT, partner, 0, &tmp, 1, MPI_INT, partner, 0, comm, MPI_STATUS_IGNORE);
        sum += tmp;
        bitmask <<= 1;
    }

    return sum;
} 
int main(int argc, char **argv) {
    int limit = atof(argv[1]);
    int comm_sz, my_rank, val, total;
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    srandom(my_rank+1);
    val = random() % limit;

    get_results(val, my_rank, comm_sz, MPI_COMM_WORLD);

    total = Global_sum(val, my_rank, comm_sz, MPI_COMM_WORLD);

    get_results(total, my_rank, comm_sz, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
} 