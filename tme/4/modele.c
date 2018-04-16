#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <unistd.h>

#define MAX_CS 3
#define REQ 1
#define REPLY 2
#define FIN 3
#define REQUEST 2
#define NOT_REQUEST 1
#define SC 0

static int horloge;
static int nb_process;
static int nb_reply;
static int etat;
static int nb_process_fini;
static int rank;
static int date_last_req;
static int *file;

void init_file()
{
	int i;

	file = (int*)malloc(sizeof(int) * nb_process);
	for(i = 0; i < nb_process; i++)
		file[i] = -1;
}

void send_msg_req()
{
	int i;

	printf("%d envoie REQ\n", rank);
	horloge++;
	date_last_req = horloge;
	etat = REQUEST;
	for (i = 0; i < nb_process; i++)
	{
		if (i != rank)
			MPI_Send(&horloge, 1, MPI_INT, i, REQ, MPI_COMM_WORLD);
	}
}

void send_msg_fin()
{
	int i;

	horloge++;
	for (i = 0; i < nb_process; i++)
	{
		if (i != rank)
			MPI_Send(&horloge, 1, MPI_INT, i, FIN, MPI_COMM_WORLD);
	}
}

void send_msg_release()
{
	int i;
	
	horloge++;
	for (i = 0; i < nb_process; i++)
	{
		if (file[i] >= 0 && file[i] < nb_process)
		{
			printf("%d j'envoie a %d\n", rank, file[i]);
			MPI_Send(&horloge, 1, MPI_INT, file[i], REPLY, MPI_COMM_WORLD);
			file[i] = -1;
		}
	}
	etat = NOT_REQUEST;
}

void receive_msg()
{
	MPI_Status status;
	int clock_val;
	int i;

	MPI_Recv(&clock_val, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	horloge = (clock_val > horloge ? clock_val : horloge) + 1;
	switch (status.MPI_TAG)
	{
		case REQ:
			if ((etat == NOT_REQUEST) || (etat == REQUEST && clock_val < date_last_req) || (clock_val == date_last_req && status.MPI_SOURCE > rank))
			{
				horloge++;
				MPI_Send(&horloge, 1, MPI_INT, status.MPI_SOURCE, REPLY, MPI_COMM_WORLD);
			}
			else
			{
				for (i = 0; i < nb_process; i++)
				{
					if (file[i] == -1)
					{
						printf("%d met en attente %d\n", rank, status.MPI_SOURCE);
						file[i] = status.MPI_SOURCE;
						break;
					}
				}
			}
		break;
		case REPLY:
			nb_reply++;
			if (nb_reply == nb_process - 1)
				etat = SC;
		break;
		case FIN:
			nb_process_fini++;
		break;
	}
}

int main(int argc, char **argv)
{
	int cpt;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_process);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	init_file();
	cpt = 0;
	while (cpt < MAX_CS)
	{
		send_msg_req();
		nb_reply = 0;
		while (nb_reply != nb_process - 1)
			receive_msg();
		cpt++;
		printf("%d SC %d ieme fois\n", rank, cpt);
		sleep(1);
		send_msg_release();
	}
	send_msg_fin();
	nb_process_fini++;
	while (nb_process_fini != nb_process)
	{
		printf("%d attend encore %d processus\n", rank, nb_process - nb_process_fini);
		receive_msg();
	}
	free(file);
	MPI_Finalize();
	return (0);

}