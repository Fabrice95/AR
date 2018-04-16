#include <stdio.h>
#include <string.h>
#include <mpi.h>

#include "mpi_server.h"
#define MASTER 0
#define SIZE 2

void callback( int tag, int source)
{
	int my_rank;
	int nb_proc;
	char message[SIZE];
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	
	pthread_mutex_lock(getMutex());
	if ( my_rank == 0)
		MPI_Recv(message, SIZE, MPI_CHAR, nb_proc -1, tag, MPI_COMM_WORLD, &status);
	else
		MPI_Recv(message, SIZE, MPI_CHAR, my_rank-1, tag, MPI_COMM_WORLD, &status);
	pthread_cond_signal(getCondMessageNonRecu());
	pthread_mutex_unlock(getMutex());
	printf("J'ai eu un msg de tag %d de la source %d\n", tag, source);
}

int main(int argc, char **argv){
	int my_rank;
	int nb_proc;

	int source;
	int dest;
	int tag =0;
	char message[SIZE];
	MPI_Status status;
	
	MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	
	message[0] = my_rank+'0';
	message[1] = 0;
	
	start_server(callback);
	
	pthread_mutex_lock(getMutex());
	MPI_Send(message, strlen(message)+1, MPI_CHAR, (my_rank +1) % nb_proc ,tag,MPI_COMM_WORLD);
	pthread_cond_wait(getCondMessageNonRecu(), getMutex());
	pthread_mutex_unlock(getMutex());
	
	destroy_server();
	MPI_Finalize();
	return 0;
}


