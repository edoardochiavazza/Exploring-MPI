#include <mpi.h>
#include <stdio.h>
#include <cstdlib>
#include<cmath>


int main(int argc, char **argv)
{   
    int random_number, random_rep, squared_random_number;

    random_rep = 5;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Status status;
    

    // Get the rank of the calling process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    while(random_rep != 0){
        if(world_rank == 0){ //server
            
            MPI_Recv( &random_number,  1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD , &status);
            printf("I'm the server with rank %d and the number i receive is %d from client n: %d \n", world_rank, random_number, status.MPI_SOURCE);
            squared_random_number = pow(random_number, 2);
            MPI_Send( &squared_random_number, 1 , MPI_INT , status.MPI_SOURCE , 0, MPI_COMM_WORLD);
            printf("I'm the server with rank %d and the number i send is %d \n", world_rank, squared_random_number);

        }else{
            random_number = rand() % 100 + 1;
            MPI_Send( &random_number, 1 , MPI_INT , 0, 0, MPI_COMM_WORLD);
            printf("I'm the client with rank %d and the number i send is %d \n", world_rank, random_number);
            MPI_Recv( &squared_random_number,  1, MPI_INT, 0, 0, MPI_COMM_WORLD , &status);
            printf("I'm the client with rank %d and the number i receive is %d \n", world_rank, squared_random_number);
        }
        random_rep -= 1;
    }
    // Finalize: Any resources allocated for MPI can be freed
    MPI_Finalize();
}