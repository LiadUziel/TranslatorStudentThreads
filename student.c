#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define FIFO_READ "a_fifo"
#define FIFO_WRITE "b_fifo"
#define STR_LEN 100

int main()
{
	char s[STR_LEN];
	FILE *fdw;//descriptor fot writing
	FILE *fdr;//descriptor for reading 
	int isExit = 1;//flag to while if exit will exit from func
	// errno == EEXIST if fifo already exists


	//for writing
	if (mkfifo(FIFO_WRITE, S_IFIFO | 0644) == -1 &&
		errno != EEXIST) {
		perror("cannot create fifo file") ;
		exit(EXIT_FAILURE) ;
	}
	if (!(fdw = fopen(FIFO_WRITE, "w"))) {
		perror("cannot open fifo file for w") ;
		exit(EXIT_FAILURE) ;
	}

	printf("Enter English word: ");
	while ( fgets(s, STR_LEN, stdin) != NULL && isExit!=0 ) 
	{
		fprintf(fdw, " %s\n", s) ;
		fflush(fdw) ; // <== important
		fclose(fdw);

		//for reading
		if (mknod(FIFO_READ, S_IFIFO | 0666, 0) == -1 &&
		errno != EEXIST) 
		{
			perror("cannot create fifo file") ;
			exit(EXIT_FAILURE) ;
		}
		if (!(fdr = fopen(FIFO_READ, "r"))) {
			perror("cannot open fifo file for r") ;
			exit(EXIT_FAILURE) ;
		}
		if(fscanf(fdr, " %s", s) != EOF)
		{
			printf("The word in German: %s\n", s) ;
		}
		fclose(fdr);
		
		if (strcmp(s, "Bye") == 0)
		{
			isExit=0;
			break;//because no need to write
		}
		//for writing
		if (mkfifo(FIFO_WRITE, S_IFIFO | 0644) == -1 &&
			errno != EEXIST) {
			perror("cannot create fifo file") ;
			exit(EXIT_FAILURE) ;
		}
		if (!(fdw = fopen(FIFO_WRITE, "w"))) {
			perror("cannot open fifo file for w") ;
			exit(EXIT_FAILURE) ;
		}

		if(isExit!=0)//it will do the while if not exit
		{
			printf("Enter English word: ");
		}
	
	}
	//fclose(fdw); - because we exit in only "exit"
	//fclose(fdr); - no need because we read and close
	unlink(FIFO_READ);
	unlink(FIFO_WRITE);
	return EXIT_SUCCESS ;
} 