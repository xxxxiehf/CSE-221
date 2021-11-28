#include "file_system.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

vector<string> file_names;
// fixed file size, 1GB
const off_t file_size = 1024 * 1024 * 1024;

void read_file(int index) {
    void *buffer = malloc(BLOCKSIZE);
    system("echo 3 > /proc/sys/vm/drop_caches");
    int fd = open(file_names[index].c_str(), O_RDONLY | O_SYNC);
    // disabled file cache for it
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED) < 0) {
        cout << "failed to set POSIX flag." << endl;
        return;
    }
    uint64_t start = 0, end = 0, sum = 0;
    off_t number_of_blocks = file_size / BLOCKSIZE;
    while (true) {
        start = rdtsc_start();
        ssize_t bytes_read = read(fd, buffer, BLOCKSIZE);
        end = rdtsc_end();
        if (bytes_read <= 0)
            break;
        sum += (end - start);
    }
    close(fd);
    free(buffer);
    cout << (double)sum / (double)number_of_blocks << endl;
}

// create given number of process
// each corresponding to reading one file
void contention(int number_of_process) {
    pid_t pids[number_of_process];
    for (int i = 0; i < number_of_process; i++) {
        pids[i] = fork();
        // if failed to fork
        if (pids[i] < 0) {
            cout << "failed to fork " << i << "th process." << endl;
            abort();
        } else if (pids[i] == 0) {
            // the i-th process will read the i-th test file
            read_file(i);
            exit(0);
        }
    }
    // wait until all child finish
    pid_t wid;
    int status = 0;
    while ((wid = wait(&status)) > 0);
}

int main() {
    string file_name_prefix = "test-";
    // create 8 test files
    for (int i = 0; i < 12; i++) {
        string file_name = file_name_prefix + to_string(i);
        file_names.push_back(file_name);
        create_file(file_name.c_str(), file_size);
    }

    // test with different number of process
    for (int i = 1; i <= 12; i++) {
        // repeat to calculate mean and sd
        cout << i << endl;
        // for (int j = 0; j < 10; j++)
        contention(i);
    }

    for (int i = 0; i < 12; i++) {
        unlink(file_names[i].c_str());
    }
}