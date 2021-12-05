#!/bin/sh
sudo mkdir -p /share/test
sudo chown 777 /share/test

echo "/share/test -alldirs" >> /etc/exports

sudo exportfs -a
sudo systemctl restart nfs-kernel-server