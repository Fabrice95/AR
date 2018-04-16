#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT    0
#define NB_SITE 6
#define TAGMIN 1
#define TAGRES 2

void simulateur(void) {
   int i;

   printf("\nAlgorithme de l'arbre\n");

   /* nb_voisins[i] est le nombre de voisins du site i */
   int nb_voisins[NB_SITE+1] = {-1, 2, 3, 2, 1, 1, 1};
   int min_local[NB_SITE+1] = {-1, 3, 11, 8, 14, 5, 17};

   /* liste des voisins */
   int voisins[NB_SITE+1][3] = {{-1, -1, -1},
         {2, 3, -1}, {1, 4, 5}, 
         {1, 6, -1}, {2, -1, -1},
         {2, -1, -1}, {3, -1, -1}};
                               
   for(i=1; i<=NB_SITE; i++){
      //printf("envoi a i: %d, nb_voisins: %d\n", i, nb_voisins[i]);
      MPI_Send(&nb_voisins[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);

      //printf("envoi a i: %d, tableau des voisins correspondant\n", i); 
      MPI_Send(voisins[i], nb_voisins[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);

      //printf("envoi a i: %d, min_local: %d\n", i, min_local[i]);
      MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD); 
   }
}


void recevoir_params(int *nb_voisins, int **voisins, int **voisins_restants, int *valeur) {
   int i;
   MPI_Status status;
   MPI_Recv(nb_voisins, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);

   *voisins = (int *) malloc( (*nb_voisins) * sizeof(int));
   if(*voisins == NULL) {
      perror("malloc - voisins");
      goto error_voisins;
   }

   *voisins_restants = (int *) malloc( (*nb_voisins) * sizeof(int));
   if(*voisins_restants == NULL){
      perror("malloc - voisins_restants");
      goto error_voisins_restants;
   }

   MPI_Recv(*voisins, *nb_voisins, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
   MPI_Recv(valeur, 1, MPI_INT, 0, TAGINIT, MPI_COMM_WORLD, &status);
  
   for(i=0;i<(*nb_voisins);i++)
      (*voisins_restants)[i] = (*voisins)[i];

   return;

   error_voisins_restants:
   free(*voisins);

   error_voisins:
   exit(-1);
}

void calcul_min(int rang) {
   int nb_voisins, *voisins, *voisins_restants, valeur, i, recu, j;
   MPI_Status status;

   // reception des parametres
   recevoir_params(&nb_voisins, &voisins, &voisins_restants, &valeur);

   // attente de la réception des messages des voisins
   // si feuille: pas entree dans boucle => pas d'attente
   for(i=0; i<nb_voisins-1; i++){
      MPI_Recv(&recu, 1, MPI_INT, MPI_ANY_SOURCE, TAGMIN, MPI_COMM_WORLD, &status);
      //printf("\n%d: reception du voisin: %d, min_recu: %d\n", rang, status.MPI_SOURCE, recu);
      if (recu < valeur)
         valeur = recu;
      //parcours du tableau contenant les voisins restants pour la mise a jour
      for (j=0;j<nb_voisins;j++)
         if(voisins_restants[j] == status.MPI_SOURCE)
            voisins_restants[j] = -1;
   }

   // recherche du voisin restant, pas de recherche pour une feuille
   j=0;
   while(voisins_restants[j] == -1)
      j++;

   // envoi au voisin restant
   MPI_Send(&valeur, 1, MPI_INT, voisins_restants[j], TAGMIN, MPI_COMM_WORLD);

   // attente annonce et mise a jour
   MPI_Recv(&recu, 1, MPI_INT, voisins_restants[j], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
   if (recu < valeur)
         valeur = recu;
   printf("site: %d, min = %d", rang, valeur);
   if(status.MPI_TAG == TAGMIN)
      printf(" -> decideur");
   printf("\n");

   // annonce
   for(i=0; i<nb_voisins; i++)
      if(voisins[i] != voisins_restants[j])
         MPI_Send(&valeur, 1, MPI_INT, voisins[i], TAGRES, MPI_COMM_WORLD);

   free(voisins);
   free(voisins_restants);
}


/******************************************************************************/

int main (int argc, char* argv[]) {
   int nb_proc,rang;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);

   if (nb_proc != NB_SITE+1) {
      printf("Nombre de processus incorrect !\n");
      MPI_Finalize();
      exit(2);
   }
  
   MPI_Comm_rank(MPI_COMM_WORLD, &rang);
  
   if (rang == 0) {
      simulateur();
   } else {
      calcul_min(rang);
   }
  
   MPI_Finalize();
   return 0;
}
