#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h> 
#define FIFO_READ "b_fifo"
#define FIFO_WRITE "a_fifo"
#define STR_LEN 100

void* translate(void* arg)
{
	FILE *fdw;
	char* translateWord = (char*)arg; 
	//Creates new fifo for writing
	if (mkfifo(FIFO_WRITE, S_IFIFO | 0777) == -1 &&
	errno != EEXIST) 
	{
		perror("cannot create fifo file") ;
		exit(EXIT_FAILURE) ;
	}
	if (!(fdw = fopen(FIFO_WRITE, "w"))) 
	{
		perror("cannot open fifo file for w") ;
		exit(EXIT_FAILURE) ;
	}
	//doing translationing 
	if (strcmp(translateWord, "water") == 0)
	{
		fprintf(fdw, " %s", "wasser") ;
		fflush(fdw) ; // <== important
		printf("Translation sent...\n");
	}
	else if (strcmp(translateWord, "woman") == 0)
	{
		fprintf(fdw, " %s", "Frau") ;
		fflush(fdw) ; // <== important
		printf("Translation sent...\n");
	}
	else if (strcmp(translateWord, "man") == 0)
	{
		fprintf(fdw, " %s", "Mann") ;
		fflush(fdw) ; // <== important
		printf("Translation sent...\n");
	}
	else if (strcmp(translateWord, "exit") == 0)
	{
		fprintf(fdw, " %s", "Bye") ;
		fflush(fdw) ; // <== important
	}
	else
	{//no in our dicitionary
		fprintf(fdw, " %s", "Undefined") ;
		fflush(fdw) ; // <== important
		printf("Translation sent...\n");
	}
	
	fclose(fdw);
}




int main()
{
	 char s[STR_LEN];
	 pthread_t thread;
	 FILE *fdr;
	 if (mknod(FIFO_READ, S_IFIFO | 0666, 0) == -1 &&
		 errno != EEXIST) {
		 perror("cannot create fifo file") ;
		 exit(EXIT_FAILURE) ;
	 }

	 if (!(fdr = fopen(FIFO_READ, "r"))) {
		 perror("cannot open fifo file for r") ;
		 exit(EXIT_FAILURE) ;
	 }

	printf("Translator is waiting for students.\n");
	printf("Translator is ready\n");


	 while ( fscanf(fdr, " %s", s) != EOF && strcmp(s,"exit")!=0)
	 {
		fclose(fdr);
		printf("Got: %s\n", s) ;   
        if(pthread_create(&thread, NULL, &translate, s))
        {
            perror("pthread_create failed in main") ;
		    exit(EXIT_FAILURE) ;
		}
		if(pthread_join(thread, NULL)!=0)
        {
            perror("pthread_join failed in main") ;
		    exit(EXIT_FAILURE) ;
        }
		if (mknod(FIFO_READ, S_IFIFO | 0666, 0) == -1 &&
		 errno != EEXIST) 
		 {
			perror("cannot create fifo file") ;
			exit(EXIT_FAILURE) ;
		 }

		if (!(fdr = fopen(FIFO_READ, "r"))) 
		{
			perror("cannot open fifo file for r") ;
			exit(EXIT_FAILURE) ;
		}     	
	}
	if(strcmp(s,"exit")==0)// to send exit = bye to student
	{
		fclose(fdr);
		if(pthread_create(&thread, NULL, &translate, s))
        {
            perror("pthread_create failed in main") ;
		    exit(EXIT_FAILURE) ;
		}
		if(pthread_join(thread, NULL)!=0)
        {
            perror("pthread_join failed in main") ;
		    exit(EXIT_FAILURE) ;
        }

	}
	printf("Translator shut down.\n");
	//fclose(fdr); - no need because always exit in exit
	unlink(FIFO_READ);
	unlink(FIFO_WRITE);
	return EXIT_SUCCESS ;
} 

