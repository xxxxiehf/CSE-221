#!/bin/bash

for ((i=1;i<=10;i++))
do
    ./bin/measurement | awk '{print $1}'
done