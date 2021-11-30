#include "file_system.h"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

void file_cache_measurement() {
    // create a buffer with size of exactly one block
    void *buffer = malloc(BLOCKSIZE);

    // start from 4GB, end at 16GB, stripe is 1GB
    for (off_t block_number = 1024 * 1024; block_number <= 4 * 1024 * 1024;
         block_number += 256 * 1024) {
        // repeat for 10 times
        for (int i = 0; i < 10; i++) {
            off_t file_size = block_number * BLOCKSIZE;
            string file_name =
                "test-" + to_string(block_number * 4 / 1024 / 1024) + "-GB";
            create_file(file_name.c_str(), file_size);

            int fd = open(file_name.c_str(), O_RDONLY | O_SYNC);
            off_t bytes_read_overall = 0;

            // first, seek to the last block in the file
            lseek(fd, file_size - 1, SEEK_SET);

            // read the file from end to start
            // put the file into cache of file system
            while (true) {
                lseek(fd, -2 * BLOCKSIZE, SEEK_CUR);
                ssize_t bytes_read = read(fd, buffer, BLOCKSIZE);
                if (bytes_read <= 0 || bytes_read_overall >= file_size) {
                    break;
                }
                bytes_read_overall += bytes_read;
            }

            // close the file and read again
            close(fd);
            fd = open(file_name.c_str(), O_RDONLY | O_SYNC);
            lseek(fd, file_size - 1, SEEK_SET);
            bytes_read_overall = 0;
            uint64_t start = 0, end = 0, sum = 0;

            // while from end to start to avoid block prefetching
            while (true) {
                lseek(fd, -2 * BLOCKSIZE, SEEK_CUR);
                start = rdtsc_start();
                ssize_t bytes_read = read(fd, buffer, BLOCKSIZE);
                end = rdtsc_end();
                sum += end - start;
                if (bytes_read <= 0 || bytes_read_overall >= file_size) {
                    break;
                }
                bytes_read_overall += bytes_read;
            }

            close(fd);
            unlink(file_name.c_str());
            cout << file_size / 1024 / 1024 / 1024 << "\t"
                 << (double)sum / (double)block_number << endl;
            // cout << "Average cycles for reading a 4k block for a " <<
            // file_size / 1024 / 1024 / 1024 << "GB file is " << (double)sum /
            // (double)count << endl;
        }
    }
    free(buffer);
}

int main() { file_cache_measurement(); }