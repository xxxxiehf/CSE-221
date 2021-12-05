#include "time.h"
#include <iostream>
#include <vector>
using namespace std;

#define ACCESS_TIMES 10000
#define REPEAT_TIMES 10

void ram_access_time_instance(long array_size, long stride) {
    cout << "Ram access test with array size: " << array_size
         << " and stride: " << stride << endl;
    // cout << array_size << "\t" << stride << "\t";
    double total_cycles = 0;
    for (int time = 0; time < REPEAT_TIMES; time++) {
        // init an array
        int *array = (int *)malloc(array_size * sizeof(int));
        if (array == nullptr) {
            cout << "Failed to allocate array." << endl;
            return;
        }
        for (int i = 0; i < array_size; i++) {
            array[i] = (i + stride) % array_size;
        }
        int index = 0;
        uint64_t start = rdtsc_start();
        for (int i = 0; i < ACCESS_TIMES; i++) {
            index = array[index];
        }
        uint64_t end = rdtsc_end();
        total_cycles += end - start;
        free(array);
    }
    cout << total_cycles / (double)(REPEAT_TIMES * ACCESS_TIMES) << endl;
}

void ram_access_time(int array_size_len, int stride_len) {
    vector<long> array_size_vec(array_size_len), stride_size_vec(stride_len);
    // array size starts from 2KB
    array_size_vec[0] = 512;
    for (int i = 1; i < array_size_len; i++) {
        array_size_vec[i] = array_size_vec[i - 1] * 2;
    }
    // stride len starts from 4B
    stride_size_vec[0] = 1;
    for (int i = 1; i < stride_len; i++) {
        stride_size_vec[i] = stride_size_vec[i - 1] * 2;
    }

    for (int i = 0; i < array_size_len; i++) {
        for (int j = 0; j < stride_len; j++) {
            ram_access_time_instance(array_size_vec[i], stride_size_vec[j]);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    ram_access_time(15, 20);
}