#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#define MAX_LEN 	1024    /* Maximum Length of the command alogwith the arguments */

#define ARG_LEN 	128     /* Maximum Length of Argument */

#define ARG_COUNT	64    	/* Maximum number of arguments */

int main(int argc, char *argv[], char *envp[])

{

 	while(1){

         char 	*arg[ARG_COUNT];

         char 	cmd[MAX_LEN];

         char 	*pos = &cmd[0];

         int 	cmd_len, pid, status;

  	 printf ("\n$ "); 		/* Displaying the prompt */

 	 fgets (cmd, MAX_LEN, stdin); 	/* Reading user input	 */

         cmd_len = strnlen(cmd, MAX_LEN);

 	 int i=0,k;

         do{

              int 	len;

              char 	a[ARG_LEN];

              sscanf(pos,"%s",a); 		/* Parsing out the arguments */

 	      len=strnlen(a,ARG_LEN);

	                     arg[i] = malloc(len);

              if (!arg[i]) {

                    printf("Unable to allocate memory\n");

                    exit(1);

              }

	      strncpy(arg[i++],a,ARG_LEN);

	      pos+=(len+1);		/* Repositioning: for next argument */

	}while(pos<cmd+cmd_len);   	/* Obtained the arguments */

      	pid=fork();			/* Create child process to execute command */

	if(pid==0){       		/* In child process */

		arg[i] = NULL;

        	if (execve(arg[0], arg, envp) == -1){

              		 perror ("Error");

         	}

         	exit(1); 	 	/* Reached if Child failed to execute */

    	}

	wait(&status);  		/* Wait for Child to terminate */

/* continue */

  }

return 0;

}