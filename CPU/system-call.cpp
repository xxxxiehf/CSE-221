#include <unistd.h>
#include <iostream>
#include "time.h"

void system_call_overhead() {
    uint64_t start, end;
    start = rdtsc_start();
    getpid();
    end = rdtsc_end();
    std::cout << "The average overhead of one system call is " 
        << (double)(end - start) << " cycles." << std::endl;
}

int main() {
    // try to accelerate iostream
    std::ios_base::sync_with_stdio(false);

    system_call_overhead();
}
