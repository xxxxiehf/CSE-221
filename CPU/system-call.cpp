#include <iostream>
#include <stdint.h>
#include <unistd.h>
using namespace std;

#include "time.h"

void system_call_overhead() {
    uint64_t start, end;
    start = rdtsc_start();
    getpid();
    end = rdtsc_end();
    cout << "The average overhead of one system call is "
         << (double)(end - start) << " cycles." << endl;
}

int main() {
    // try to accelerate iostream
    ios_base::sync_with_stdio(false);

    for (int i = 0; i < 10; i++)
        system_call_overhead();
}
