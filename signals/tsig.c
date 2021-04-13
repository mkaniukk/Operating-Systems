#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define QUANTITY 5
#define WITH_SIGNALS
int COUNT = 0;

#ifdef WITH_SIGNALS
int interrupt = 0; 

void interrupt_handler(){
    
    printf("\nparent[%d]: intterrupt\n", getpid());
    interrupt = 1;
}

void terminate_handler(){
    
    printf("child[%d]: recieved SIGTERM signal\n", getpid());
}
#endif

void NUM_CHILD(){
    
    #ifdef WITH_SIGNALS
    // Ignore interrupt signal and set own SIGTERM handler
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, terminate_handler);
    #endif

    printf("child[%d]: process execution start | parent[%d]\n", getpid(), getppid());
    sleep(10);
    printf("child[%d]: process executing finished\n", getpid());
}

void terminate_child_processes(pid_t p[]){

    for(int i = 0; i < COUNT; i++){
        printf("parent[%d]: sending SIGTERM signal -> child[%d]\n", getpid(), p[i]);
        kill(p[i], SIGTERM);
    }    
}

int main(void){

    #ifdef WITH_SIGNALS
    // Ignore all signals and restore default handler for SIGCHLD
    for(int i=0; i < _NSIG; i++)
        signal(i, SIG_IGN);

    signal(SIGCHLD, SIG_DFL);
    // Set own SIGINT handler
    signal(SIGINT, interrupt_handler);
    #endif

    printf("parent[%d]: initialized\n", getpid());
    pid_t pid, child_process_array[QUANTITY];
    
    for(int i = 0; i < QUANTITY; i++){
        
        #ifdef WITH_SIGNALS
        // Interruption handling
        if(interrupt==1){
            terminate_child_processes(child_process_array);
            printf("parent[%d]: child creation iterrupted\n", getpid());
            break;
        }
        #endif
            
        pid = fork();
        
        // Child process
        if(pid==0){
            NUM_CHILD();
            return 0;
        }        
        
        // Fork error
        if(pid<0){
            printf("child[%d]: process has not been created correctly\n", pid);
            printf("parent[%d]: sending SIGTERM\n", getpid());
            terminate_child_processes(child_process_array);
            return 1;
        }

        // Parent process
        child_process_array[i] = pid;
        COUNT++;
        sleep(1);
    
    }
    printf("All child processes have been created\n");

    int exit_code, exit_codes[COUNT], count_codes = 0;
    
    while(true){
    
        pid = wait(&exit_code);
        
        // No more processes
        if(pid==-1)
            break;
        
        // Child process exit
        if(WIFEXITED(exit_code)){
            exit_codes[count_codes] = WEXITSTATUS(exit_code);
            count_codes++;
        }
    }   

    printf("\nparent[%d]: no more child processes\n", getpid());
    printf("Recieved %d exit codes:\n", count_codes);
    for(int i = 0; i < COUNT; i++)
        printf("child[%d]: exit code %d\n", child_process_array[i], exit_codes[i]);

    #ifdef WITH_SIGNALS
    // Restore old handlers
    for(int i=0; i < _NSIG; i++)
        signal(i, SIG_DFL);
    #endif
    
    return 0;
}
