#include "mpi_server.h"

#include <pthread.h>
static server the_server;
#define SIZE 2
//ICI CODE THREAD SERVEUR A COMPLETER

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
	pthread_mutex_unlock(getMutex());
	printf("J'ai eu un msg de tag %d de la source %d\n", tag, source);
}

void *fonction_serveur()
{
	
	int received =0;
	char message[SIZE];
	MPI_Status status;
	while ( received == 0)
	{
		pthread_mutex_lock(getMutex());
		MPI_Iprobe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &received, &status);
		pthread_mutex_unlock(getMutex());
		sleep(1);
	} 
	
	(the_server.callback) ( status.MPI_TAG, status.MPI_SOURCE);
	return NULL;
}

void start_server(void (*callback)(int tag, int source))
{
	the_server.callback = callback;
	pthread_mutex_init(&the_server.mutex ,NULL);
	if(pthread_create( &the_server.listener, NULL, fonction_serveur,NULL) == -1)
	{
		perror("pthread_create");
		exit(1);
	}
	
	
	
}
void destroy_server()
{
	if(pthread_join(the_server.listener ,NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}
}
pthread_mutex_t* getMutex()
{
	return &the_server.mutex;
}
