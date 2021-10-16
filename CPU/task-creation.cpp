#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <iostream>
#include "time.h"
#define TIMES 10000

void processCreate_overhead() {
    unsigned long start, end;
    start = rdtsc_start();
    for (int i = 0; i < TIMES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            exit(0);
        } else {
            wait(NULL);
        }
    }
    end = rdtsc_end();
    std::cout << "The average overhead to create a user process is " 
    << (double)(end - start) / (double)TIMES << " cycles." << std::endl;
}

void* runThread(void *) {
    pthread_exit(NULL);
}

void threadCreate_overhead() {
    pthread_t tidp;
    unsigned long start, end;
    start = rdtsc_start();
    for (int i = 0; i < TIMES; ++i) {
        pthread_create(&tidp, NULL, runThread, NULL);
        pthread_join(tidp, NULL);
    }
    end = rdtsc_end();
    std::cout << "The average overhead to create a kernel thread is " 
    << (double)(end - start) / (double)TIMES << " cycles." << std::endl;
}

int main() {
    // try to accelerate iostream
    std::ios_base::sync_with_stdio(false);

    processCreate_overhead();
    threadCreate_overhead();

}
