#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H
#include <cstdio>

// size of a block is 4K
const off_t BLOCKSIZE = 4 * 1024;

void create_file(const char *file_name, off_t file_size) {
    FILE *fd = fopen(file_name, "w");
    fseek(fd, file_size - 1, SEEK_SET);
    fputc('\0', fd);
    fclose(fd);
}

// rdtsc_start serialize code and read the clock
// call before entering code to be measured
// return current timestamp
static unsigned long inline rdtsc_start(void) {
    unsigned cycles_high, cycles_low;

    asm volatile("cpuid\n\t" /* serialize */
                 "rdtsc\n\t" /* read the clock */
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 : "=r"(cycles_high), "=r"(cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");

    // we need 64bits to represent current timestamp
    return (((unsigned long)cycles_high << 32) | cycles_low);
}

// rdtsc_end serialize code and read the clock
// call after exiting code to be measured
// return current timestamp
static unsigned long inline rdtsc_end(void) {
    unsigned cycles_high, cycles_low;

    asm volatile("rdtscp\n\t"
                 "mov %%edx,     %0\n\t"
                 "mov %%eax,     %1\n\t"
                 "cpuid\n\t"
                 : "=r"(cycles_high), "=r"(cycles_low)::"%rax", "%rbx", "%rcx", "%rdx");

    // we need 64bits to represent current timestamp
    return (((unsigned long)cycles_high << 32) | cycles_low);
}

#endif