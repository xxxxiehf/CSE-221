#include <unistd.h>
#include <iostream>
#include "time.h"

void systemCall_overhead() {
    unsigned long start, end;
    start = rdtsc_start();
    getpid();
    end = rdtsc_end();
    std::cout << "The average overhead of one system call is " 
        << (double)(end - start) << " cycles." << std::endl;
}

int main() {
    // try to accelerate iostream
    std::ios_base::sync_with_stdio(false);

    systemCall_overhead();
}
