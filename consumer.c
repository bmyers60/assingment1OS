#include "table.h"

int main() {
    // Producer initializes shared memory

    int sharedMemoryDescriptor = shm_open("/SharedMemory2", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(sharedMemoryDescriptor == -1)
        printf("error: shm_open()");

    struct table *consumer;
    consumer = static_cast<table*>(mmap(0, sizeof(struct table), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryDescriptor, 0));
    if(consumer == MAP_FAILED)
        printf("error: mmap()");

    int iterationCount = 0;
    while ( iterationCount < numIterations) { // Perform 5 iterations

        // Wait for producer to finish producing
        sem_wait(&consumer->filledSpots);

        for (int i = 0; i < bufferSize; ++i) {
            printf("Consumed item %d\n", consumer->sharedBuffer[i]);
            consumer->sharedBuffer[i] = 0; // Clear buffer
        }

        ++iterationCount;
        printf("Turn: %i\n", iterationCount);
        sem_post(&consumer->emptySpots);

        // Signal producer that consumer finished
        sem_post(&consumer->consumerFinished);
    }

    return 0;
}
