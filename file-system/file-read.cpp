#include "file_system.h"
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
using namespace std;

void seq_read(const char *file_name, off_t file_size) {
    void *buffer = malloc(BLOCKSIZE);
    system("echo 3 > /proc/sys/vm/drop_caches");
    int fd = open(file_name, O_RDONLY | O_SYNC);
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED) < 0) {
        cout << "failed to set POSIX flag." << endl;
        return;
    }
    off_t bytes_read_overall = 0;
    uint64_t start = 0, end = 0, sum = 0;
    while (true) {
        start = rdtsc_start();
        ssize_t bytes_read = read(fd, buffer, BLOCKSIZE);
        end = rdtsc_end();
        if (bytes_read <= 0 || bytes_read_overall >= file_size) {
            break;
        }
        bytes_read_overall += bytes_read;
        sum += (end - start);
    }
    close(fd);
    free(buffer);
    // cout << (double)sum / (file_size / BLOCKSIZE) << endl;
    cout << "average cycles to sequentially read a block in size "
         << file_size / 1024 / 1024 << "MB is "
         << (double)sum / (file_size / BLOCKSIZE) << endl;
}

void ram_read(const char *file_name, off_t file_size) {
    void *buffer = malloc(BLOCKSIZE);
    system("echo 3 > /proc/sys/vm/drop_caches");
    int fd = open(file_name, O_RDONLY | O_SYNC);
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED) < 0) {
        cout << "failed to set POSIX flag." << endl;
        return;
    }
    uint64_t start = 0, end = 0, sum = 0;
    off_t block_num = file_size / BLOCKSIZE;
    srand((unsigned int)time(NULL));
    for (auto i = 0; i < block_num; i++) {
        int rand_index = rand() % block_num;
        start = rdtsc_start();
        lseek(fd, rand_index * BLOCKSIZE, SEEK_SET);
        read(fd, buffer, BLOCKSIZE);
        end = rdtsc_end();
        sum += (end - start);
    }
    close(fd);
    free(buffer);
    // cout << (double)sum / (file_size / BLOCKSIZE) << endl;
    cout << "average cycles to randomly read a block in size "
         << file_size / 1024 / 1024 << "MB is "
         << (double)sum / (file_size / BLOCKSIZE) << endl;
}

void file_read() {
    // file start from 2MB to 256MB
    for (off_t file_size = 2 * 1024 * 1024; file_size <= 256 * 1024 * 1024;
         file_size *= 2) {
        string file_name = "test-" + to_string(file_size / 1024 / 1024) + "-MB";
        create_file(file_name.c_str(), file_size);
        cout << file_size / 1024 / 1024 << endl;
        for (int i = 0; i < 10; i++) {
            seq_read(file_name.c_str(), file_size);
            // ram_read(file_name.c_str(), file_size);
        }
        unlink(file_name.c_str());
    }
}

int main() {
    file_read();
}