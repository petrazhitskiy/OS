#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


// g) pr1; pr2 | pr3 > f.res

int main(int argc, char * argv[])
{
	int fd[2], f;
	pipe(fd);
	f=open(argv[4], O_WRONLY|O_CREAT|O_TRUNC, 0776);
	if (fork()==0) /* process 1 */ 
	{
		execlp(argv[1],argv[1], NULL);
	}
	wait(NULL);
	if (fork()==0) /* process 2 */ 
	{
		close (fd[0]);
		dup2(fd[1],1);
		close(fd[1]);
		execlp(argv[2],argv[2], NULL);
	}
	if (fork()==0)
 	{

 		dup2(fd[0],0);
 		close(fd[0]);
 		dup2(f,1);
 		close(f);
 		execlp(argv[3],argv[3], NULL);/* process 3 */
 	}
	close(fd[0]);
 	wait(NULL);
 	wait(NULL);
	return 0;
}
