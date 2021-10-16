#include "time.h"
#include <iostream>
#define TIMES 100000

static __attribute__((noinline)) void param_0() {}
static __attribute__((noinline)) void param_1(int p1) {}
static __attribute__((noinline)) void param_2(int p1, int p2) {}
static __attribute__((noinline)) void param_3(int p1, int p2, int p3) {}
static __attribute__((noinline)) void param_4(int p1, int p2, int p3, int p4) {}
static __attribute__((noinline)) void param_5(int p1, int p2, int p3, int p4,
                                              int p5) {}
static __attribute__((noinline)) void param_6(int p1, int p2, int p3, int p4,
                                              int p5, int p6) {}
static __attribute__((noinline)) void param_7(int p1, int p2, int p3, int p4,
                                              int p5, int p6, int p7) {}

void procedure_call_overhead() {
    unsigned long start, end, sum;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_0();
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 0 parameter takes "
              << (double)sum / TIMES << std::endl;
    
    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_1(i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 1 parameter takes "
              << (double)sum / TIMES << std::endl;
    
    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_2(i, i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 2 parameters takes "
              << (double)sum / TIMES << std::endl;

    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_3(i, i, i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 3 parameters takes "
              << (double)sum / TIMES << std::endl;

    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_4(i, i, i, i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 4 parameters takes "
              << (double)sum / TIMES << std::endl;

    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_5(i, i, i, i, i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 5 parameters takes "
              << (double)sum / TIMES << std::endl;
    
    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_6(i, i, i, i, i, i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 6 parameters takes "
              << (double)sum / TIMES << std::endl;

    sum = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc_start();
        param_7(i, i, i, i, i, i, i);
        end = rdtsc_end();
        sum += end - start;
    }
    std::cout << "Function calling with 7 parameters takes "
              << (double)sum / TIMES << std::endl;
}

int main() {
    // try to accelerate iostream
    std::ios_base::sync_with_stdio(false);

    procedure_call_overhead();
    return 0;
}