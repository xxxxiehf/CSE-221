#!/bin/sh
sudo mkdir -p /mnt/test
sudo mount -t nfs {serverIP}:/share/test /mnt/test

make all
mv test-* /mnt/test/