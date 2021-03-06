#include "time.h"
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define TIMES 10000
using namespace std;

uint64_t single_process_switch_time() {
    uint64_t start = 0, end = 0;
    uint64_t diff = 0;
    pid_t pid;
    int fd[2];
    pipe(fd);
    if ((pid = fork()) != 0) {
        start = rdtsc_start();
        wait(NULL);
        read(fd[0], (void *)&end, sizeof(uint64_t));
    } else {
        end = rdtsc_end();
        write(fd[1], (void *)&end, sizeof(uint64_t));
        exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    if (end > start)
        diff = end - start;
    return diff;
}

void process_switch_overhead() {
    double ans = 0.0;
    for (uint64_t i = 0; i < TIMES; ++i) {
        ans += single_process_switch_time();
    }
    cout << "The average context switch overhead for user process is "
         << ans / (double)TIMES << " cycles." << endl;
}

void *run_thread(void *fd) {
    uint64_t end = rdtsc_end();
    write(*(int *)fd, (void *)&end, sizeof(uint64_t));
    pthread_exit(NULL);
}

uint64_t single_thread_switch_time() {
    uint64_t start = 0, end = 0;
    uint64_t diff = 0;
    pthread_t tidp;
    int fd[2];
    pipe(fd);
    pthread_create(&tidp, NULL, run_thread, &fd[1]);
    start = rdtsc_start();
    pthread_join(tidp, NULL);
    read(fd[0], (void *)&end, sizeof(uint64_t));
    close(fd[0]);
    close(fd[1]);
    diff = end - start;
    return diff;
}

void thread_switch_overhead() {
    double ans = 0.0;
    for (uint64_t i = 0; i < TIMES; ++i) {
        ans += single_thread_switch_time();
    }
    cout << "The average context switch overhead for kernel thread is "
         << ans / (double)TIMES << " cycles." << endl;
}

int main() {
    // try to accelerate iostream
    ios_base::sync_with_stdio(false);

    for (int i = 0; i < 10; i++) {
        process_switch_overhead();
        thread_switch_overhead();
    }
}