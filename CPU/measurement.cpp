#include "time.h"
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#define TIMES 10000

void reading_time_overhead() {
    double all_overhead = 0;
    uint64_t start = 0, end = 0;
    for (uint64_t i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        end = rdtsc_end();
        all_overhead += (end - start);
    }
    cout << "The average overhead of reading in " << TIMES << " cycles is "
         << all_overhead / (double)TIMES << endl;
}

void loop_overhead() {
    uint64_t start = rdtsc_start();
    for (uint64_t i = 0; i < 100000; i++)
        ;
    uint64_t end = rdtsc_end();
    cout << "The average overhead time of one loop is "
         << (double)(end - start) / 100000 << " cycles." << endl;
}

void cpu_cycle() {
    uint64_t start = rdtsc_start();
    sleep(10);
    uint64_t end = rdtsc_end();
    cout << end - start << " cycles in 10 seconds" << endl;
}

int main() {
    // try to accelerate iostream
    ios_base::sync_with_stdio(false);

    // cpu_cycle();

    for (int i = 0; i < 10; i++) {
        reading_time_overhead();
        loop_overhead();
    }
    return 0;
}