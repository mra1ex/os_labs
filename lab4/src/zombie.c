
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

pid_t child;

void
sighan (int sig)
{
  (void) sig;
  if (child > 0)
    if (child != waitpid (child, NULL, WNOHANG))
      {
        printf("Child is still alive, SIGKILLing...\n");
        kill (child, SIGKILL);
        waitpid (child, NULL, 0);
      }
  printf("Terminated\n");
}

int
main (void)
{
  child = fork ();
  if (child < 0)
    printf("Unable to fork");
  else if (child > 0)
    {
      signal (SIGINT, sighan);
      signal (SIGTERM, sighan);
      printf("Parent pid = %d", getpid ());
      for (;;)
        {
          /* Do nothing. */
        }
    }
  else
    printf("From child = %d: terminated\n", getpid ());

  printf("UNREACHED\n");
  return 0;
}
