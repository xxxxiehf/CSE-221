# CSE-221
Project for UC San Diego CSE 221, 2021 Fall.

## Environment

We used Linux (g++) as our environment, all files pass our compilation, but we can NOT guarantee that they can pass compilation in any other environment.

## Build & Run

There is a `Makefile` file in each of sub directory. Use `make all` to compile all files in the same directory. All the executable file will under the sub directory `bin`, just type 
```shell
./bin/{name of the test}
```
to run the test.

### CPU (/CPU)

In our experiment, we bound tests on one particular CPU using 
```shell
taskset -c 7 ./bin/{name of task}
```
You can try this, but this works better with excluding this CPU from OS's scheduler. You can find the script (`exclude-CPU.sh`) to exclude it, but **no recommendation of executing that**. If you insist, restart to finish the exclusion.

Just running those files without specifying (and excluding) CPU is fine.

### Memory (/mem)

Like in the CPU section, you can bind tests to one partcular CPU or just type `make all` and run executable files.

### Network (/network)

In the code, there is a variable names `local_ip`. Here we use `127.0.0.1`, so the code will test loopback. If you want to test between two machines, change this variable to the IP of server, recompile (`make all`) and rerun them.

For all tests, run the files of server first, then run the files of client.

### File System (/file-system)

In the test *remote file read*, we use two machines to test. Run `NFS-{client/server}-setup.sh` to setup the client or server, then run the corresponding executable files (run the file of server first). 