#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
void timeout_catcher (int status)
{
    printf("mrbayes timeout %d\n",status);
}

int main ()
{
  int fdsout[2];
  int fdsin[2];
  pid_t pid;

  if(pipe (fdsout) != 0) {
    perror("pipe failed\n");
    exit(0);
  }
  if(pipe (fdsin) != 0) {
    perror("pipe failed\n");
    exit(0);
  }

  pid = fork (); /* Fork a child process.  */
  if (pid == (pid_t) 0) {
    // This is the child process.  
    close(fdsout[0]);
    dup2 (fdsout[1], STDOUT_FILENO); // Dup fds[1] to stdout 
    close(fdsin[1]);
    dup2 (fdsin[0], STDIN_FILENO); // Connect the read end of the pipe to standard input.
    execlp ("./mb", "./mb", 0);      // Run MrBayes
  } else {
    /* This is the parent process.  */
    FILE* output;
    FILE* input;

    /* Convert the write file descriptor to a FILE object, and write to it.  */
    if((output = fdopen (fdsin[1], "w")) == NULL) {
        perror("output fdopen failed\n");
        exit(0);
    }
    close(fdsin[0]);
    if((input = fdopen (fdsout[0], "r")) == NULL) {
        perror("input fdopen failed\n");
        exit(0);
    }
    close(fdsout[1]);
printf("outputing\n");
    fprintf (output, "execute primates.nex \n");
    fprintf (output, "set nowarn=yes \n");
    fprintf (output, "lset nst=6 rates=invgamma\n");
    fprintf (output, "mcmcp savebrlens=yes\n");

#define MAXLINE 1024
    char buf[MAXLINE];
		float prob = 1.0;
		
		setbuf(stdout,0);
		setbuf(output,0);
		fprintf (output, "mcmc ngen=100\n");
		fflush (output);
		int count = 0;
		int countbelow = 0;
// We want to keep going until we have at least 6 trees and at least 4 below .10 probability
		while((countbelow < 4) || (count < 6)) {
printf("reading count = %d, countbelow = %d\n", count, countbelow);
			while(fgets(buf, MAXLINE, input) != NULL) {
				fputs(buf, stdout);
				if(sscanf(buf,"  Average standard deviation of split frequencies: %f\n",&prob) > 0) {
					printf("Current Probability %f\n",prob);
					fprintf (output, "yes\n");
					fprintf (output, "100\n");
					break;
				}
			}
			count++;
			if(prob < 0.10) {
				countbelow++;
			}
		}
		fprintf (output, "no\n");
		printf ("executing count %d, sumt contpe=halfcompat burnin=%d\n",count, (count-4));
		fprintf (output, "sumt contype=halfcompat burnin=%d\n", count-4);
		fprintf (output, "quit\n");
		fflush (output);
		// Wait for the child to exit
		signal(SIGALRM, timeout_catcher);
		alarm(10);
		int retpid=waitpid(pid, NULL,0);
		printf("waitpid(%d) returned %d\n",pid,retpid);
    fclose(output);
	} // End of Parent
}

