#include <err.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_data
{
  pthread_t t;
  int i, n, k, mod;
  unsigned long long *res;
  pthread_mutex_t *mutex;
};

void *
run (void *arg)
{
  int l, h;
  unsigned long long temp = 1;
  struct thread_data *td = arg;

  l = td->i * td->k / td->n;
  h = (td->i == td->n - 1
       ? td->k
       : (td->i + 1) * td->k / td->n);

  while (l < h)
    temp = temp * (1 + l++) % td->mod;

  pthread_mutex_lock (td->mutex);
  *td->res = *td->res * temp % td->mod;
  pthread_mutex_unlock (td->mutex);

  return NULL;
}

int
main (int argc, char *argv[])
{
  bool flag = false;
  unsigned long long res = 1;
  int pnum = -1, mod = -1, k = -1;
  pthread_mutex_t mutex;
  struct thread_data *tds;

  for (;;)
    {
      int c, option_index;
      struct option options[] = {
        {"pnum", required_argument, 0, 'p'},
        {"mod", required_argument, 0, 'm'},
        {0, 0, 0, 0}
      };

      c = getopt_long (argc, argv, "p:m:k:", options, &option_index);
      if (c == -1)
        break;

      switch (c)
        {
        case  'p':
          pnum = atoi (optarg);
          break;
        case 'm':
          mod = atoi (optarg);
          break;
        case  'k':
          k = atoi (optarg);
          break;
        case '?':
          printf("Unknown option\n");
          break;
        case ':':
          printf("Option %s requires an argument\n", options[option_index].name);
          break;
        default:
          printf("Unexpected behavior\n");
          break;
        }
    }

  if (pnum < 1 || k < 0 || mod < 1)
  {
    printf("Usage: -p/--pnum <number of procs> -m/--mod <mod> -k <number>\n");
    return EXIT_FAILURE;
  }

  while (optind < argc)
  {
    printf("Wrong positional argument: %s\n", argv[optind++]);
    flag = true;
  }
  if (flag)
  {
    printf("Wrong arguments\n");
    return EXIT_FAILURE;
  }

  tds = malloc(pnum * sizeof(*tds));

  if (tds == NULL)
  {
    printf("Unable to allocate memory\n");
    return EXIT_FAILURE;
  }

  pthread_mutex_init (&mutex, NULL);
  for (int i = 0; i < pnum; i++)
  {
    struct thread_data *td = &tds[i];

    td->i = i;
    td->n = pnum;
    td->k = k;
    td->mod = mod;
    td->res = &res;
    td->mutex = &mutex;

    if(pthread_create (&td->t, NULL, run, td) != 0)
    {
      printf("Failed to create thread\n");
      return EXIT_FAILURE;
    }
  }

  for (int i = 0; i < pnum; i++)
  {
    if(pthread_join (tds[i].t, NULL) != 0)
    {
      printf("Failed to join thread\n");
      return EXIT_FAILURE;
    }
  }
  pthread_mutex_destroy (&mutex);

  printf ("res: %llu\n", res);

  free(tds);
  return EXIT_SUCCESS;
}
