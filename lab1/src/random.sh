#!/bin/bash

for ((i=0; i<150; i++)); do
    echo $(od -An -N2 -i /dev/random) >> numbers.txt
done
