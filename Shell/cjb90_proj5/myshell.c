#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>

#define MAX_LEN 1024
#define MAX_ARG_LEN 128 
#define MAX_ARG 64
#define EXIT "exit"
#define CHANGE_DIR "cd"
#define TIME "time"

int main(int argc, char *argv[], char *envp[]){

	while(1){
	
		char cmd[MAX_LEN];
		char temp_str[MAX_ARG_LEN];
		char filename[MAX_ARG_LEN];
		char *args[MAX_ARG];
		char *arg_curr;
		int status,i;
		int timed,read,write;
		struct tms *cpu_times;
	
		printf ("\n$ "); 						// Display the shell prompt
		
		fgets(cmd, MAX_LEN, stdin); 			// Read in command and arguments
		
		if(strchr(cmd, '<') != NULL) read = 1;
		else if(strchr(cmd, '>') != NULL) write = 1;
		else{
			read = 0;
			write = 0;
		}
		
		arg_curr = strtok(cmd," \t\n<>()|&;"); 		//Tokenize command
		i = 0;
		timed = 0;
		
		while(arg_curr != NULL){					//Tokenize all arguments
			//printf("%s \n",arg_curr);
			if(strncmp(arg_curr,TIME,4) == 0){
				timed = 1;
				arg_curr = strtok(NULL," \t\n<>()|&;");
			}
			
			else{
    			args[i] = arg_curr;
    			arg_curr = strtok(NULL," \t\n<>()|&;");
    			i++;
    		}
 		}
 		
 		if(read || write){
 			strcpy(filename,args[i-1]);
 			args[i-1] = NULL;
 		}
 		
 		args[i] = NULL;
	
		if(strncmp(args[0],EXIT,4) == 0){			//Internal CMD for "exit"
			printf ("Exiting Shell\n"); 
			exit(0);
		}
		
		else if(strncmp(args[0],CHANGE_DIR,2) == 0){	//Internal CMD for "cd"
			int ret = chdir(args[1]); 
			if(ret < 0){
				printf ("Directory change failed: %s\n", strerror(errno));  
			}
		}
		
		else{
		
			strcpy(temp_str,"/bin/");
			args[0] = strcat(temp_str,args[0]);
		
			if(timed){	//Internal CMD for "time"
				cpu_times = malloc(sizeof(struct tms));
				times(cpu_times);	
			}
		
			if(fork() == 0){
			
				if(read){
					if(freopen(filename,"r",stdin) == NULL){
						printf ("Redirection failed: %s\n", strerror(errno));
					}
				}
			
				else if(write){
					if(freopen(filename,"w",stdout) == NULL){
						printf ("Redirection failed: %s\n", strerror(errno));
					}
				}

       		 	if (execvp(args[0], args) == -1) printf ("Command failed: %s\n", strerror(errno));   
				
         		exit(1);

    		}

			wait(&status); 
			
			read = 0;
			write = 0;
		
			if(timed){
    			printf("User Time %jd, System Time %jd\n",
       		 	(cpu_times->tms_utime/sysconf(_SC_CLK_TCK)),
        		(cpu_times->tms_stime/sysconf(_SC_CLK_TCK)));
				timed = 0;
				free(cpu_times);

			}
		
			if(status < 0) printf ("Command failed: %s\n", strerror(errno));
		
		}		
	}
	
	return 0;
}