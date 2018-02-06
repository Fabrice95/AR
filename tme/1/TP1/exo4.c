#include <stdio.h>
#include <string.h>
#include <mpi.h>
#define MASTER 0
#define SIZE 2
int main(int argc, char **argv){
	int my_rank;
	int nb_proc;
	int source;
	int dest;
	int tag =0;
	char message[SIZE];
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	
	message[0] = my_rank+'0';
	message[1] = 0;
	
	if( my_rank != (nb_proc-1))
		MPI_Ssend(message, strlen(message)+1, MPI_CHAR, (my_rank +1) % nb_proc ,tag,MPI_COMM_WORLD);
	
	if ( my_rank == 0)
		MPI_Recv(message, SIZE, MPI_CHAR, nb_proc -1, tag, MPI_COMM_WORLD, &status);
	else
	{
		MPI_Recv(message, SIZE, MPI_CHAR, my_rank-1, tag, MPI_COMM_WORLD, &status);
		if( my_rank == (nb_proc-1))
			MPI_Ssend(message, strlen(message)+1, MPI_CHAR, (my_rank +1) % nb_proc ,tag,MPI_COMM_WORLD);
	}
	printf("Je suis %d et j'ai eu le msg %s\n",my_rank, message);
	
	MPI_Finalize();
	return 0;
}

/* 
 * 2. Tous envoient leurs message puis attendent que le suivant le réceptionne. Cependant, le 
 * processus my_rank+1 envoie également le msg et attend que son suivant le reçoit, et ainsi de suite
 * Tous les processus attendent donc et personne ne fait de recv
 * 3. le dernier attend puis envoie pour débloquer la file 
*/
