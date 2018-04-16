#include <stdio.h>
#include <mpi.h>

//************   LES TAGS
#define WANNA_CHOPSTICK 0		// Demande de baguette
#define CHOPSTICK_YOURS 1		// Cession de baguette
#define DONE_EATING     2		// Annonce de terminaison

//************   LES ETATS D'UN NOEUD
#define THINKING 0   // N'a pas consomme tous ses repas & n'attend pas de baguette
#define HUNGRY   1   // En attente de baguette
#define DONE     2   // A consomme tous ses repas

//************   LES REPAS
#define NB_MEALS 3	// nb total de repas a consommer par noeud

//************   LES VARIABLES MPI
int NB;               // nb total de processus
int rank;             // mon identifiant
int left, right;      // les identifiants de mes voisins gauche et droit

//************   LA TEMPORISATION
int local_clock = 0;                    // horloge locale
int clock_val;                          // valeur d'horloge recue
int meal_times[NB_MEALS];        // dates de mes repas

//************   LES ETATS LOCAUX
int local_state = THINKING;		// moi
int left_state  = THINKING;		// mon voisin de gauche
int right_state = THINKING;		// mon voisin de droite

//************   LES BAGUETTES 
int left_chopstick = 0;		// je n'ai pas celle de gauche
int right_chopstick = 0;	// je n'ai pas celle de droite

//************   LES REPAS 
int meals_eaten = 0;		// nb de repas consommes localement


//************   LES FONCTIONS   *************************** 
int max(int a, int b) {
   return (a>b?a:b);
}

void receive_message(MPI_Status *status) {
   int received_clock;

   MPI_Recv(&received_clock, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, status);
   local_clock = max(local_clock, received_clock) + 1;
}

void send_message(int dest, int tag) {
   local_clock++;
   MPI_Send(&local_clock, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
}

/* renvoie 0 si le noeud local et ses 2 voisins sont termines */
// je suis DONE et mes voisins aussi
int check_termination() {
   return ((local_state != DONE) && (left_state != DONE) && (right_state != DONE))?1:0;
}

//************   LE PROGRAMME   *************************** 
int main(int argc, char* argv[]) {

   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &NB);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
   left = (rank + NB - 1) % NB;
   right = (rank + 1) % NB;

   while(check_termination()) {

      /* Tant qu'on n'a pas fini tous ses repas, redemander les 2 baguettes  
         a chaque fin de repas */
      if ((meals_eaten < NB_MEALS) && (local_state == THINKING)) {
         
         //demande de baguette aux 2 voisins
         if( !left_chopstick ){
            send_message(left, WANNA_CHOPSTICK);
         }
         if( !right_chopstick ){
            send_message(right, WANNA_CHOPSTICK);
         }

         printf("P%d> Asking for chopsticks at %d\n", rank, local_clock);


	 //
	 //A Completer
         
         
      }
		
      receive_message(&status);
		
      if (status.MPI_TAG == DONE_EATING) {

         //Enregistrer qu'un voisin a fini de manger
         if(status.MPI_SOURCE == left){
            left_state = THINKING;
         }else{
            right_state = THINKING;
         }
	     
      } else 
         if (status.MPI_TAG == CHOPSTICK_YOURS) {
		
            //Recuperation d'une baguette
            if(status.MPI_SOURCE == left){
               left_chopstick = 1;
            }else{
               right_chopstick = 1;
            }
                 
		
         } else {

		/* c'est une demande */
            
            // test qui demande la baguette
            if(status.MPI_SOURCE == left){
               left_chopstick = 0;
            }else{
               right_chopstick = 0;
            }

            // signaler autorisation
            send_message(status.MPI_SOURCE, CHOPSTICK_YOURS);
          }
   }
	
   MPI_Finalize();
   return 0;
}
