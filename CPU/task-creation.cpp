#include "time.h"
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

#define TIMES 10000

void process_create_overhead() {
    uint64_t start, end;
    start = rdtsc_start();
    for (uint64_t i = 0; i < TIMES; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            exit(0);
        } else {
            wait(NULL);
        }
    }
    end = rdtsc_end();
    cout << "The average overhead to create a user process is "
         << (double)(end - start) / (double)TIMES << " cycles." << endl;
}

void thread_create_overhead() {
    pthread_t tidp;
    uint64_t start, end;
    start = rdtsc_start();
    for (uint64_t i = 0; i < TIMES; ++i) {
        pthread_create(
            &tidp, NULL,
            [](void *) -> void * {
                pthread_exit(NULL);
                return NULL;
            },
            NULL);
        pthread_join(tidp, NULL);
    }
    end = rdtsc_end();
    cout << "The average overhead to create a kernel thread is "
         << (double)(end - start) / (double)TIMES << " cycles." << endl;
}

int main() {
    // try to accelerate iostream
    ios_base::sync_with_stdio(false);

    process_create_overhead();
    thread_create_overhead();
}
