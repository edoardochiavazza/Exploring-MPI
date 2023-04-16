#include <mpi.h>
#include <stdio.h>
#include <cstdlib>
#include<cmath>


int main(int argc, char **argv)
{   
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int number, random_number, squared_random_number,n_client, random_rep, size, request; 
    int end = 1;

    // Check that more then 1 MPI processes are used.
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size < 2){
        printf("This application is meant to be run with 2 MPI processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    } 

    // Get the rank of the calling process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank == 0){ //server
        n_client = size - 1;
        while (n_client != 0){
            MPI_Recv(&request, 1 , MPI_INT , MPI_ANY_SOURCE, MPI_ANY_TAG , MPI_COMM_WORLD, &status);
            if(status.MPI_TAG == 0){
                squared_random_number = pow(random_number, 2);
                MPI_Send(&squared_random_number, 1 , MPI_INT , status.MPI_SOURCE , 0, MPI_COMM_WORLD);
            }else{
                n_client -= 1;
            }
        }

    }else{

        while(random_rep != 0){ //client 
            random_number = rand() % 100 + 1;
            MPI_Send(&random_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&squared_random_number,  1, MPI_INT, 0, 0, MPI_COMM_WORLD , &status);
            printf("I'm the client with rank %d and the number i receive is %d \n", world_rank, squared_random_number);
            random_rep -= 1;
        }   
            MPI_Send(&end, 1 , MPI_INT , 0, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize(); 
}