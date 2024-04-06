#ifndef TABLE
#define TABLE

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int bufferSize = 2;
int numIterations = 5;

struct table {
    sem_t filledSpots;
    sem_t emptySpots;
    sem_t consumerFinished;
    int sharedBuffer[2];
};

#endif
