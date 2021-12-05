#!/bin/sh
echo "GRUB_CMDLINE_LINUX="isolcpus=7" >> /etc/default/grub

sudo update-grub
