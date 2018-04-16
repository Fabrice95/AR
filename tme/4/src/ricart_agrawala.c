#include <stdio.h>
#include <string.h>
#include <mpi.h>



/*	le nombre max de fois de 
	que chaque processus 
	accede a la secton critique
*/
#define MAX_CS 1

/*	les tags pour differencier
	les types de messages
*/

#define REQUEST 1
#define REPLY 2
#define RELEASE 3



int main(int argc, char **argv){
	int my_rank;
	int nb_proc;
	
	int cpt=0;
	int *file;
	int index;
	int horloge = 0;

	int horloge_recu;
	int nb_reply;
	int i;
	MPI_Status status;



	MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	
	file = (int*) malloc(nb_proc*sizeof(int));
	
	if(file == NULL){
		perror("malloc: allocation de la file");
		exit(-1);
	}



	while(cpt < MAX_CS){
		
		// mise a jour de l'horloge et broadcast a tous les autres sites
		horloge++;
		for(int i=0; i<nb_proc; i++)
			if(i != my_rank)
				MPI_Send(&horloge, 1, MPI_INT, i, REQUEST, MPI_COMM_WORLD);

		// attente des reponses de tous les serveurs
		nb_reply = 0;
		while(nb_reply != nb_proc-1){
			MPI_Recv(&horloge_recu, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			horloge++;

			printf("%d: message recu de %d pour %s\n", my_rank, status.MPI_SOURCE);

			switch(status.MPI_TAG){
				case REQUEST:
					printf("\tREQUEST:");
					if(horloge_recu < horloge){
						horloge = (horloge<horloge_recu)?horloge_recu++:horloge;
						MPI_Send(&horloge, 1, MPI_INT, status.MPI_SOURCE, REPLY, MPI_COMM_WORLD);
					}else{
						horloge = (horloge<horloge_recu)?horloge_recu++:horloge;
						file[index] = status.MPI_SOURCE;
					}
					break;
				case REPLY:
					printf("\tREPLY");
					reply++;
					break;
				case RELEASE:
					printf("\tRELEASE");
					
					break;
			}
		}

		// acces en section critique
		printf("autorisation recu de %d pour id ...%s\n", );
		// sortie de la section critique
		MPI_Send(&horloge, 1, MPI_INT, i, REQUEST, MPI_COMM_WORLD);

		cpt++;
	}
	
	
	free(file);
	
	destroy_server();
	MPI_Finalize();

	return 0;
}
