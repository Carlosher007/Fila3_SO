
/**
 * This program calculates the fibonnaci number asociated with a position in a vector. 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <pthread.h>
#define println printf("\n")
#define CONSTANTE 5

int max_random(int min, int max) {
    return min + rand() % (max + 1 - min);
}

void print_vector(int* vector, int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", vector[i]);
    }
}

void* fibonacci_over_vector(void *);

int *vector;
int nprocessors;
int step;

int main(int argc, char *argv[]) {
    int vl, mr, i; // vl: vector_length, mr: max_random, i : index used in fors
    pthread_t *threads;
    int *threads_id;

  // Initializing variables
    if(argc != 3) {
        printf("Ejecutar de la siguiente manera: \n");
        printf("\t %s <vector_length> <max_random>\n",argv[0]);
        exit(1);
    }

    vl = atoi(argv[1]);
    mr = atoi(argv[2]);
    nprocessors = get_nprocs();
    step = (int) (vl / nprocessors);

    // Initializing vector variables
    vector = (int*) malloc(sizeof(int) * vl);
    assert(vector != NULL); // In case of a zero elements vector
    threads = (pthread_t*) malloc(sizeof(pthread_t) * nprocessors);
    assert(threads != NULL);
    threads_id = (int*) malloc(sizeof(int) * nprocessors);
    srand(time(NULL));

    for(i = 0 ; i < vl; i++) {
        vector[i] = max_random(mr - CONSTANTE, mr);
    }

    printf("Vector initialized\n");
    print_vector(vector, vl);
    println;
    printf("Vector size: %d\n", vl);
    printf("Max random value: %d\n", mr);
    printf("Processors number: %d\n", nprocessors);
    printf("Step: %d\n", step);

    for(i = 0; i < nprocessors; i++) {
        threads_id[i] = i;
        pthread_create(&threads[i], NULL, fibonacci_over_vector, (void*) threads_id[i]);
    }

    for(i = 0; i < nprocessors; i++) {
        pthread_join(threads[i], NULL);
    }

    print_vector(vector, vl);
    println;

    return 0;
}

int fibonacci(int p) {
    if(p == 0) {
        return 0;
    } else if(p == 1) {
        return 1;
    } else {
        return fibonacci(p - 1) + fibonacci(p - 2);
    }
}

void* fibonacci_over_vector(void *pos) {
    int thread_id = (int) pos;

    for (int i = 0; i < step; i++) {
        i = i * nprocessors + thread_id;
        vector[i] = fibonacci(vector[i]);
        printf("Thread: %d, posicion: %i \n", thread_id, i);

    }
    return NULL;
}