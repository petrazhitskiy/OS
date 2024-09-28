#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

//e)  pr1 arg1 < if.dat | pr2 | pr3 > f.res &

int main(int argc, char * argv[])

{
	int fd1[2], fd2[2], f, r;
	pipe(fd1);
	pipe(fd2);
	r = open(argv[3], O_RDONLY | O_CREAT, 0776);
	f=open(argv[6], O_WRONLY|O_CREAT|O_TRUNC, 0776);
	if(fork()==0)
	{
		close (fd1[0]);
		dup2(fd1[1], 1);	
		dup2(r, 0);
		close(fd1[1]);
        	close(r);
        	execlp(argv[1],argv[1], argv[2], NULL);
        	perror("execlp");
        	exit(1);	
	}
	
	if (fork()==0)
	{
		dup2(fd1[0], 0);
		close(fd1[0]);
		close (fd1[0]);
		dup2(fd1[1], 1);
		close(fd1[1]);
		execlp(argv[4],argv[4], NULL);
        	perror("execlp");
        	exit(1);	
	}
	
	if (fork()==0)
	{
		dup2(fd1[0], 0);
		close(fd1[1]);
		dup2(f,1);
 		close(f);
		execlp(argv[5],argv[5], NULL);
        	perror("execlp");
        	exit(1);	
	}
	
	close(fd1[0]);
	close(fd2[0]);
 	wait(NULL);
 	wait(NULL);
 	return 0;
}
