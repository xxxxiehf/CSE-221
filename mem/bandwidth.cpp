#include "time.h"
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

#define REPEAT_TIMES 10

// read, 4 byte read, 32 byte stride
void ram_read_bandwidth() {
    double overall_cycles = 0;
    for (int time = 0; time < REPEAT_TIMES; time++) {
        long long sum = 0;
        // 256MB
        int arr_size = pow(2, 26);
        register int *array = (int *)malloc(arr_size * sizeof(int));
        memset(array, 1, arr_size * sizeof(int));
        register int cur = 0;
        uint64_t start = rdtsc_start();
        while (cur < arr_size) {
            sum +=
#define     DOIT(i) array[cur + i] +
            DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
            DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48)
            DOIT(52) DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72)
            DOIT(76) DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) 
            DOIT(100) DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) 
            array[124];
            cur += 128;
        }
        uint64_t end = rdtsc_end();
        overall_cycles += end - start;
        free(array);
    }
    cout << overall_cycles / (double)REPEAT_TIMES << endl;
}
#undef DOIT

// write, 4 byte write, 32 byte stride
void ram_write_bandwidth() {
    double overall_cycles = 0;
    for (int time = 0; time < REPEAT_TIMES; time++) {
        // 256MB
        int arr_size = pow(2, 26);
        int *array = (int *)malloc(arr_size * sizeof(int));
        memset(array, 1, arr_size * sizeof(int));
        register int cur = 0;
        uint64_t start = rdtsc_start();
        while (cur < arr_size) {
#define     DOIT(i) array[cur + i] = 1;
            DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
            DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52) 
            DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76) DOIT(80) 
            DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100) DOIT(104)
            DOIT(108) DOIT(112) DOIT(116) DOIT(120) DOIT(124);
            cur += 128;
        }
        uint64_t end = rdtsc_end();
        overall_cycles += end - start;
        free(array);
    }
    cout << overall_cycles / (double)REPEAT_TIMES << endl;
}
#undef DOIT

int main() {
    ram_read_bandwidth();
    ram_write_bandwidth();
}