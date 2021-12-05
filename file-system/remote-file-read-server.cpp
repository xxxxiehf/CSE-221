#include "file_system.h"
#include <string>
#include <iostream>
using namespace std;

int main() {
    for (off_t file_size = 2 * 1024 * 1024; file_size <= 256 * 1024 * 1024;
         file_size *= 2) {
        string file_name = "test-" + to_string(file_size / 1024 / 1024) + "-MB";
        create_file(file_name.c_str(), file_size);
    }
}