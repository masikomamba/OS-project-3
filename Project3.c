// Masiko Mamba

#define _REENTRANT
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#define BUFFER_SIZE 15

int main() {
    // Circular buffer and its indices
    char buffer[BUFFER_SIZE];
    int in = 0;
    int out = 0;

    // Semaphores
    sem_t empty;
    sem_t full;
    sem_t mutex;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE); // BUFFER_SIZE empty slots initially
    sem_init(&full, 0, 0);           // 0 filled slots initially
    sem_init(&mutex, 0, 1);          // Mutex for critical section

    int producer_complete = 0; // Flag to indicate producer has finished

    pthread_t producer_thread;
    pthread_t consumer_thread;

    // Producer thread
    pthread_create(&producer_thread, NULL, (void *(*)(void *)) ({
        void *producer(void *arg) {
            FILE *fp = fopen("mytest.dat", "r");
            if (fp == NULL) {
                perror("Failed to open file");
                pthread_exit((void *)1); // Exit thread with error code
            }
            char newChar;

            while (fscanf(fp, "%c", &newChar) != EOF) {
                sem_wait(&empty);           // Wait for empty slot
                sem_wait(&mutex);           // Enter critical section

                buffer[in] = newChar;        // Add character to buffer
                in = (in + 1) % BUFFER_SIZE; // Move to next position

                sem_post(&mutex);            // Exit critical section
                sem_post(&full);             // Increment filled slots
            }

            // Signal end of file with special character
            sem_wait(&empty);
            sem_wait(&mutex);
            buffer[in] = '*';                // Write termination signal
            in = (in + 1) % BUFFER_SIZE;
            sem_post(&mutex);
            sem_post(&full);

            producer_complete = 1; // Indicate producer is done
            fclose(fp);
            return NULL;
        }
        producer;
    }), NULL);

    // Consumer thread
    pthread_create(&consumer_thread, NULL, (void *(*)(void *)) ({
        void *consumer(void *arg) {
            char readChar;
            while (1) {
                sem_wait(&full);             // Wait for filled slot
                sem_wait(&mutex);            // Enter critical section

                readChar = buffer[out];      // Read character from buffer
                out = (out + 1) % BUFFER_SIZE;

                sem_post(&mutex);            // Exit critical section
                sem_post(&empty);            // Increment empty slots

                if (readChar == '*') break;  // End if special character is read
                putchar(readChar);           // Print character
                fflush(stdout);
            }
            return NULL;
        }
        consumer;
    }),
     NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
