/* les tags */
#define WANNA_CHOPSTICK 0
#define CHOPSTICK_YOURS 1
#define DONE_EATING 2

/* etat des philosophes */
#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define NB_MEALS 10 // # de fois de le philosophe mange

//variables MPI
int NB; // # de processus
int rank;
int left, right;

//gestion d horloge
int local_clock = 0;
int clock_val; // buffer de reception
int meal_times[NB_MEALS]; //date de mes repas

//etat locaux
int local_state = THINKING;
int left_state = THINKING;
int right_state = THINKING;

//les baguettes
int left_chopstick = 0;
int right_chopstick = 0;

// # repas pris
int meal_eaten = 0;

void receive_message(MPI_status *status);