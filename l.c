#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//e)  pr1 < f.dat | pr2 arg2 ; pr3 > f.res

int main(int argc, char * argv[])

{
	int fd[2], f, r;
	pipe(fd);
	
	r = open(argv[2], O_RDONLY | O_CREAT, 0776);
	f=open(argv[6], O_WRONLY|O_CREAT|O_TRUNC, 0776);
	if(fork()==0)
	{
		close (fd[0]);
		dup2(fd[1], 1);	
		dup2(r, 0);
		close(fd[1]);
        	close(r);
        	execlp(argv[1],argv[1], NULL);
        	perror("execlp");
        	exit(1);	
	}
	
	if (fork()==0)
	{
		dup2(fd[0], 0);
		close(fd[0]);
		execlp(argv[3],argv[3], argv[4], NULL);
        	perror("execlp");
        	exit(1);	
	}
	
	wait(NULL);
	
	if (fork()==0)
	{
		dup2(f,1);
 		close(f);
		execlp(argv[5],argv[5], NULL);
        	perror("execlp");
        	exit(1);	
	}
	
	close(fd[0]);
 	wait(NULL);
 	wait(NULL);
 	return 0;
}

