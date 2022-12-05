/**
 * Este programa calcula sobre cada uno de los enteros de un vector su
 * fibonacci.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <pthread.h>
#define max_random(min, max) min + rand() % (max + 1 - min)
#define printf_vector(v, n) \
  for (i = 0; i < n; i++)   \
  {                         \
    printf("%d ", v[i]);    \
  }
#define println printf("\n")
#define CONSTANTE 5

int fibonacci(int);
void *fibonacci_over_vector(void *);

int *vector;
int nprocesadores;
int stride;
int vl;
int main(int argc, char *argv[])
{
  // int vl, // vl: vector_length
  int mr; // mr: max_random
  int i;
  pthread_t *hilos;
  int *hilos_id;
  // Inicializacion de variables
  if (argc != 3)
  {
    printf("Ejecutar de la siguiente manera: \n");
    printf("\t %s <vector_length> <max_random>\n", argv[0]);
    exit(1);
  }
  vl = atoi(argv[1]);
  mr = atoi(argv[2]);
  nprocesadores = get_nprocs();
  stride = (int)(vl / nprocesadores);
  // Inicializacion de variables tipo vector
  vector = (int *)malloc(sizeof(int) * vl);
  assert(vector != NULL);
  hilos = (pthread_t *)malloc(sizeof(pthread_t) * nprocesadores);
  assert(hilos != NULL);
  hilos_id = (int *)malloc(sizeof(int) * nprocesadores);
  srand(time(NULL));
  for (i = 0; i < vl; i++)
  {
    vector[i] = max_random(mr - CONSTANTE, mr);
  }
  printf("Vector inicializado\n");
  printf_vector(vector, vl);
  println;
  printf("Vector de hilos inicializado\n");
  printf("Tamano vector: %d\n", vl);
  printf("Maximo valor aleatorio: %d\n", mr);
  printf("Num procesadores: %d\n", nprocesadores);
  printf("Stride: %d\n", stride);

  for (i = 0; i < nprocesadores; i++)
  {
    hilos_id[i] = i;
    pthread_create(&hilos[i], NULL, fibonacci_over_vector, (void *)hilos_id[i]);
    // fibonacci_over_vector no se le pasa ningun parametro, esto es porque se le pasa el id del hilo y se usa como indice para calcular el inicio y el fin del vector
  }

  for (i = 0; i < nprocesadores; i++)
    pthread_join(hilos[i], NULL);

  printf_vector(vector, vl);
  println;

  return 0;
}

int fibonacci(int m)
{
  if (m == 0)
    return 0;
  if (m == 1)
    return 1;
  return fibonacci(m - 1) + fibonacci(m - 2);
}

void *fibonacci_over_vector(void *pos)
{
  int thread_id = (int)pos;
  int i;
  for (i = thread_id; i < vl; i += nprocesadores)
  {
    printf("Soy el hilo %d y voy a calcular en la posicion %d\n", thread_id, i);
    vector[i] = fibonacci(vector[i]);
  }
  return NULL;
}
