#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int arg, char* args[])
{
	int count;
	char *c;
	char *output;
	
	//Open Specified File for reading in binary.
	FILE* exe_file = fopen(args[1],"rb");
	
	//Check to see if File exists.
	if(exe_file != NULL)
	{
		//Loop until EOF is read in
		while(!feof(exe_file))
		{
		
		//Allocate size for the current character to be read in (c) and for the current string (output)
		c = malloc(sizeof(char));
		output = malloc(sizeof(char)*1000);
		count = 0;
		fread(c,1,1,exe_file);
		
		//Loop while the character being read in is an ASCII character between 32 and 126
		while(*c >= 32 && *c <= 126)
		{
			//Add current character to the current string 
			*(output + count) = *c;
			free(c);
			c = malloc(sizeof(char));
			fread(c,1,1,exe_file);
			count++;
		}
		
		//If the string is greater then or equal to 4 characters print it out to the screen
		if(count >= 4)
		{
			*(output + count) = '\0';
			printf("%s\n",output);
		}
		
		free(c);
		free(output);
		
		}
		
		fclose(exe_file);
	}
	
	//If File does not exist print error.
	else
	{
      printf("Error: No such file exists or File is not readable.\n");
      return -1;
	}

	return 0;
}