#!/bin/bash

for ((i=0;i<10;i++))
do
taskset -c 7 ./bin/system-call
done