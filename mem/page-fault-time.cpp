#include "time.h"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
using namespace std;

uint64_t OFFSET = 4 * 1024 * 1024;
uint64_t FILE_SIZE = 512 * 1024 * 1024;
uint64_t PAGE_FAULT_TIME = 100;
uint64_t REPEAT_TIMES = 100;

string FILE_NAME = "RANDOM";

bool isFileExist(string &fileName) {
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

void checkAndCreateFile(string &fileName) {
    if (isFileExist(fileName)) {
        cout << fileName + " file has been found..." << endl;
        return;
    }
    cout << "generating file..." << endl;
    ofstream file(fileName.c_str());
    bool finished = false;
    struct stat info;
    while (!finished) {
        for (uint64_t i = 0; i < 1024; ++i) {
            char c = (rand() % 26) + 'a';
            file << c;
        }
        stat(fileName.c_str(), &info);
        uint64_t size = info.st_size;
        if (size >= FILE_SIZE) {
            finished = true;
        }
    }
    file.close();
    checkAndCreateFile(fileName);
}

void page_fault_service() {
    uint64_t start = 0, end = 0, total_cycles = 0;
    checkAndCreateFile(FILE_NAME);
    struct stat info;
    int f = open(FILE_NAME.c_str(), O_RDWR);

    for (uint64_t i = 0; i < REPEAT_TIMES; ++i) {
        stat(FILE_NAME.c_str(), &info);
        uint64_t size = info.st_size;

        volatile char ch = 0;
        char *map =
            (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);

        for (uint64_t j = 0; j < PAGE_FAULT_TIME; ++j) {
            uint64_t idx = (j + 1) * OFFSET % (size - 1);
            start = rdtsc_start();
            ch = map[idx];
            end = rdtsc_end();
            total_cycles += end - start;
        }
        munmap(map, size);
    }
    double average_cycle =
        (double)(total_cycles) / (double)(REPEAT_TIMES * PAGE_FAULT_TIME);
    cout << average_cycle << endl;
}

int main() { page_fault_service(); }
