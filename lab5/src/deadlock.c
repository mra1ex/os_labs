#include <stdio.h>
#include <err.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

pthread_t thread1, thread2;
pthread_barrier_t barrier;
pthread_mutex_t source1, source2;

void
sighan (int sig)
{
  pthread_cancel (thread1);
  pthread_cancel (thread2);

  pthread_mutex_destroy (&source1);
  pthread_mutex_destroy (&source2);
  pthread_barrier_destroy (&barrier);

  printf("Exit on signal");
}

void *
run1 (void *arg)
{
  (void) arg;

  printf("1: trying to lock 1");
  pthread_mutex_lock (&source1);
  printf("1: after locking 1");

  pthread_barrier_wait (&barrier);

  printf("1: trying to lock 2");
  pthread_mutex_lock (&source2);
  printf("1: after locking 2");

  pthread_mutex_unlock (&source2);
  pthread_mutex_unlock (&source1);

  return NULL;
}

void *
run2 (void *arg)
{
  (void) arg;

  printf("2: trying to lock 2");
  pthread_mutex_lock (&source2);
  printf("2: after locking 2");

  pthread_barrier_wait (&barrier);

  printf("2: trying to lock 1");
  pthread_mutex_lock (&source1);
  printf("2: after locking 1");

  pthread_mutex_unlock (&source1);
  pthread_mutex_unlock (&source2);

  return NULL;
}

int
main (void)
{
  signal (SIGINT, sighan);
  signal (SIGTERM, sighan);

  pthread_mutex_init (&source1, NULL);
  pthread_mutex_init (&source2, NULL);
  pthread_barrier_init (&barrier, NULL, 2);

  pthread_create (&thread1, NULL, run1, NULL);
  pthread_create (&thread2, NULL, run2, NULL);
  pthread_join (thread1, NULL);
  pthread_join (thread2, NULL);

  pthread_mutex_destroy (&source1);
  pthread_mutex_destroy (&source2);
  pthread_barrier_destroy (&barrier);

  return EXIT_SUCCESS;
}
