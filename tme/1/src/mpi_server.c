#include <pthread.h>
#include <unistd.h>

#include "mpi_server.h"

static server the_server;
#define SIZE 2
//ICI CODE THREAD SERVEUR A COMPLETER

void* fonction_serveur() {
	
	int received = 0;
	char message[SIZE];
	MPI_Status status;

	while ( received == 0) {
		pthread_mutex_lock(getMutex());
		MPI_Iprobe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &received, &status);
		pthread_mutex_unlock(getMutex());
		sleep(1);
	} 
	
	/* appel de la methode de rappel lorsque le message est recu */
	(the_server.callback) ( status.MPI_TAG, status.MPI_SOURCE);
	return NULL;
}

void start_server(void (*callback)(int tag, int source)) {
	the_server.callback = callback;
	pthread_mutex_init(&the_server.mutex ,NULL);
	if(pthread_create(&the_server.listener, NULL, fonction_serveur, NULL) == -1) {
		perror("pthread_create");
		exit(1);
	}
}

void destroy_server() {
	if(pthread_join(the_server.listener ,NULL) != 0) {
		perror("pthread_join");
		exit(1);
	}
}

pthread_mutex_t* getMutex() {
	return &the_server.mutex;
}

pthread_cond_t* getCondMessageNonRecu() {
	return &the_server.messageNonRecu;
}