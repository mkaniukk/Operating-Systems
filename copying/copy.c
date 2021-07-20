#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

size_t B_SIZE;

void help(){
	printf("\nCOPY MANUAL\n");
	printf("\tcopy [-m] <file_name> <new_file_name>\n");
	printf("\tcopy [-h]\n");
	printf("\tIf parameter 'm' is selected copying is performed using mapping\n");
	printf("\tOtherwise read() and write() methods are used\n\n");
}

int copy_rw(int fd_from, int fd_to){
	
	char buff[B_SIZE];		
	int loc, des;
	
	while((loc = read(fd_from, buff, B_SIZE)) > 0){
		des = write(fd_to, buff, loc);
		if(des <= 0){
			perror("Error copy_read_write");
			exit(1);
		}
	}	
	
	return 0;
}

int copy_nmap(int fd_from, int fd_to){
	
    	char* src_buffer;
	src_buffer = mmap(NULL, B_SIZE, PROT_READ, MAP_SHARED, fd_from, 0);
    	if(src_buffer == (void*)(-1))
    	{
        	perror("Mapping source to memory error");
        	exit(1);
    	}
	
    	char* dest_buffer;
    	dest_buffer = mmap(NULL, B_SIZE, PROT_WRITE, MAP_SHARED, fd_to, 0);
    	if(dest_buffer == (void*)(-1))
    	{
        	perror("Mapping destination to memory error");
        	exit(1);
    	}

    	dest_buffer = memcpy(dest_buffer, src_buffer, B_SIZE);
    	if(dest_buffer == (void*)(-1))
    	{
        	perror("Memory copy error");
        	exit(1);
    	}

	return 0;
}

int main(int argc, char** argv){
	
	bool m = false;
    	int opt;
    	size_t n_args;
	
	// handle arguments
	while((opt = getopt(argc,argv,":hm")) != -1){
       	switch (opt){
			case 'h':
			    help();
			    return 0;
			case 'm':
			    m = true;
			    break;
			case '?':
			    perror("Unknown parameter");
			    exit(1);
        	}
	}
	
	// too many arguments case
    	if(argc > 4){
    		printf("Too many arguments\n");
    	    	help();
    	    	exit(1);
    	}
	
	// adjust number of arguments to function
    	if (!m)
    		n_args = 3;
    	else
     		n_args = 4;
    
    	// get paths
   	char* in_file = argv[n_args-2];
	char* out_file = argv[n_args-1];
   
    	// open file
    	int src_file = open(in_file, O_RDONLY);
    	if(src_file == -1){
        	perror("Source file openning error");
        	exit(1);
    	}
	
	// get file stats
    	struct stat stat_src;
    	if(fstat(src_file, &stat_src) == -1){
        	perror("Source file stats error");
        	exit(1);
    	}
    	// get file size
    	B_SIZE = stat_src.st_size;

	// open destination file
    	int dest_file = open(out_file, O_RDWR | O_CREAT, stat_src.st_mode);
   	if(dest_file == -1){
        	perror("Destination file openning error");
        	exit(1);
    	}
	
	// adjust destination file
	if(ftruncate(dest_file, B_SIZE) == -1){
		perror("Destination file adjustment error");
        	exit(1);
	} 
	
	// perform copy function
    	if(m)
    		copy_nmap(src_file, dest_file);
    	else
    		copy_rw(src_file, dest_file);

	// close files
	int close_src = close(src_file);	
	int close_dest = close(dest_file);

   	if(close_src == -1){
        	perror("Source file closing error");
        	exit(1);
    	}
   	if(close_dest == -1){
        	perror("Destination file closing error");
        	exit(1);
    	}

	return 0;
}
