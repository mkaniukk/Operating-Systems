#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N 2
#define LEFT (left_fork_id) % N
#define RIGHT (left_fork_id + 1) % N
 
int philosophers[N];
static int SEMID;

void grab_forks(int left_fork_id){
	
	struct sembuf sops[2];
	sops[0].sem_num = LEFT;
	sops[0].sem_op = -1;
	sops[0].sem_flg = 0;
	sops[1].sem_num = RIGHT;
	sops[1].sem_op = -1;
	sops[1].sem_flg = 0;
	
	if(semop(SEMID, sops, 2) == -1){
		perror("Semop error\n");
		exit(1);
	}
}

void put_away_forks(int left_fork_id){

	struct sembuf sops[2];
	sops[0].sem_num = LEFT;
	sops[0].sem_op = 1;
	sops[0].sem_flg = 0;
	sops[1].sem_num = RIGHT;
	sops[1].sem_op = 1;
	sops[1].sem_flg = 0;

	if(semop(SEMID, sops, 2) == -1){
		perror("Semop error\n");
		exit(1);
	}
	printf("Philosopher[%d]: PUTS AWAY forks (%d, %d)\n", LEFT, LEFT, RIGHT);
	printf("Philosopher[%d]: THINKS.\n", LEFT);
}

void* philosopher(void* num){
    
    	int counter = 0;
	int* left_fork_id = num; 
	while (counter < 3) {
 
		sleep(1);
	 
		grab_forks(*left_fork_id);
	 	sleep(1);
		printf("Philosopher[%d]: EATS with forks (%d, %d)\n", *LEFT, *LEFT, *RIGHT);
		sleep(2);
		
		put_away_forks(*left_fork_id);
		counter += 1;
	}
    return NULL;
}

void terminate(int i, pid_t processes[]){
	for(int n = 0; n < i; n++){
		kill(processes[n], SIGTERM);
	}
}

int main(){
 
 	// CREATE SEMAPHORES
	SEMID = semget(IPC_PRIVATE, N, 0600);
	if(SEMID < 0) {
		perror("Semaphores error\n");
		exit(1);
	}

	int i;
	// ASSIGN ID'S TO PHILOSOPHERS
	for(i = 0; i < N; i++){

		philosophers[i] = i;
		
		if(semctl(SEMID, i, SETVAL, 1) == -1){
			perror("Semctl error");
			exit(1);
		}
	}

	pid_t process;
	pid_t philosophers_processes[N];
    
	for (i = 0; i < N; i++) {
		switch(process = fork()){
			case 0:
				printf("Philosopher[%d]: THINKS.\n", i);
				philosopher(&philosophers[i]);
		        	exit(EXIT_SUCCESS);
			case -1:
				printf("Fork() failed\n");
				terminate(i, philosophers_processes);
				exit(EXIT_FAILURE);
			default:
				philosophers_processes[i] = process;
				break;
		}
	}
	
	int status;

	while(1){
		process = wait(&status);
		if(process == -1)
			break;
	}

	
	return 0;
}
