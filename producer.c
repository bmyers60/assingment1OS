#include "table.h"
#include "string.h"
#include "errno.h"

int main(int argc, char* argv[]) {
    srand(time(nullptr)); // Initialize random seed

    int sharedMemoryDescriptor = shm_open("/SharedMemory2", O_CREAT | O_RDWR, S_IXUSR | S_IRUSR | S_IWUSR);
    if(sharedMemoryDescriptor == -1)
        printf("error: shm_open()");

    if(ftruncate(sharedMemoryDescriptor, sizeof(struct table)) == -1)
        printf("error: resizef()");
    else
        ftruncate(sharedMemoryDescriptor, sizeof(struct table)); // Resize shared memory

    struct table *producer;
    producer = static_cast<table*>(mmap(0, sizeof(struct table), PROT_READ|PROT_WRITE, MAP_SHARED, sharedMemoryDescriptor, 0)); // Point to shared memory

    if (producer == MAP_FAILED)
        printf("error: mmap()");

    // Initialize semaphores
    sem_init(&(producer->emptySpots), 1, bufferSize);
    sem_init(&(producer->filledSpots), 1, 0);
    sem_init(&(producer->consumerFinished), 1, 0); // Initialize semaphore for consumer finished signal

    for(int i = 0; i < bufferSize; ++i) {
        producer->sharedBuffer[i] = 0;
    }

    int iterationCount = 0;
    while(iterationCount < numIterations) { // Perform 5 iterations

        // Wait for consumer to finish consuming
        sem_wait(&producer->consumerFinished);

        sleep(1);
        sem_wait(&producer->emptySpots);

        int filler;
        for(int i = 0; i < bufferSize; ++i) {
            filler = rand() % 100 + 1; // Generate filler to fill buffer
            if(filler == 0) {
                filler = 1;
            }
            producer->sharedBuffer[i] = filler;
            printf("Produced item %d\n", filler);
        }
        ++iterationCount;

        sem_post(&producer->filledSpots);
    }

    return 0;
}
