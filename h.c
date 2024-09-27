#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


// h) ( (pr1 | pr2); pr3) | pr4

int main(int argc, char * argv[])

{
	int fd1[2], fd2[2];
	pipe(fd1);
	pipe(fd2);
	if(fork()==0)
	{
		close (fd2[0]);
		dup2(fd2[1],1);
		close(fd2[1]);
		if(fork()==0)
		{
			if(fork()==0) //pr1
			{
				close (fd1[0]);
				dup2(fd1[1],1);
				close(fd1[1]);
				execlp(argv[1],argv[1], NULL);
				perror("execlp");
        			exit(1);
			}
			
			if(fork()==0) //pr2
			{
				dup2(fd1[0], 0);
				close(fd1[0]);
				execlp(argv[2],argv[2], NULL);
				perror("execlp");
        			exit(1);
			}
		}
		
		wait(NULL);
		if (fork()==0) /* process 3 */ execlp(argv[3],argv[3], NULL);
		wait(NULL);
	}
	
	if(fork()==0)
	{
		dup2(fd2[0], 0);
		close(fd2[0]);
		execlp(argv[4],argv[4], NULL);
		perror("execlp");
		exit(1);
	}
	
	close(fd1[0]);
	close(fd2[0]);
 	wait(NULL);
 	wait(NULL);
}
