#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 2
#define EAT_COUNT 2
#define EXECUTION_TIME 30

#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N

#define THINKING 2
#define HUNGRY 1 
#define EATING 0

int state[N];							// initiated to THINKING's
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;		// initialized to 1
pthread_mutex_t s[N] = PTHREAD_MUTEX_INITIALIZER;		// initialized to 0's
pthread_t threads[N];

void test( int i ){
	if( state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING ){
		state[i] = EATING;
		pthread_mutex_unlock( &s[i] );
	}
}

void grab_forks( int i ){
	pthread_mutex_lock( &m );
		state[i] = HUNGRY;
		test( i );
	pthread_mutex_unlock( &m );
	pthread_mutex_lock( &s[i] );
}

void put_away_forks( int i ){
	pthread_mutex_lock( &m );
		state[i] = THINKING;
		printf("Philosopher[%d]: PUTS AWAY forks (%d, %d)\n", i, i, (i+1)%N);
		printf("Philosopher[%d]: THINKS.\n", i);		
		test( LEFT );
		test( RIGHT );
	pthread_mutex_unlock( &m );
}

void* philosopher(void* num){

	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0){
		perror("Error");
		exit(EXIT_FAILURE);
	}
    
    	int counter = 0;
	int* i = num;

	while (counter < EAT_COUNT) {
	 	sleep(1);
		grab_forks(*i);
		printf("Philosopher[%d]: EATS with forks (%d, %d)\n", *i, *i, (*(i)+1)%N);
		sleep(3);
		put_away_forks(*i);
		counter++;
	}
    	return NULL;
}

void main(){
	
	// initialize philosophers states to THINKING
	for(int n = 0; n < N; n++){
		state[n] = THINKING;
	}
	// initialize mutexes to 0
	for(int n = 0; n < N; n++){
		pthread_mutex_lock(s+n);
	}
	
	pthread_t process[N];
	for(int n = 0; n < N; n++){
		process[n] = n;
		if(pthread_create(&threads[n], NULL, philosopher, &process[n]) != 0){
			perror("Thread error\n");
			exit(1);
		}
	}
	
	sleep(EXECUTION_TIME);
	
	for(int n = 0; n < N; n++){
		pthread_cancel(threads[n]);
		pthread_join(threads[n], NULL);
	}
	exit(0);
}	
