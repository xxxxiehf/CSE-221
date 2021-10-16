#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <functional>
#include <iostream>
#include "time.h"
#define TIMES 10000

void reading_time_overhead() {
    double all_overhead = 0;
    unsigned long start, end;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        end = rdtsc_end();
        all_overhead += (end - start);
    }
    std::cout << "The average overhead of reading in " 
        << TIMES << " cycles is " << all_overhead / (double)TIMES << std::endl;
}

void systemCall_overhead() {
    unsigned long start, end;
    start = rdtsc_start();
    getpid();
    end = rdtsc_end();
    std::cout << "The average overhead of one system call is " 
        << (double)(end - start) << " cycles." << std::endl;
}

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

void loop_overhead() {
    unsigned long start = rdtsc_start();
    for (int i = 0;i < 100000;i++) ;
    unsigned long end = rdtsc_end();
    std::cout << "The average overhead time of one loop is "
        << (double)(end - start) / 100000 << " cycles." << std::endl;
}

void cpu_cycle() {
    unsigned long start = rdtsc_start();
    sleep(10);
    unsigned long end = rdtsc_end();
    std::cout << end - start << " cycles in 10 seconds" << std::endl;
}

int main() {
    // try to accelerate iostream
    std::ios_base::sync_with_stdio(false);

    // cpu_cycle();
    reading_time_overhead();
    loop_overhead();
    systemCall_overhead();
    return 0;
}