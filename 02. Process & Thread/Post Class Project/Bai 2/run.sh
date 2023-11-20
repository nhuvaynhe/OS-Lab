#!/bin/bash

make clean
make sum_serial
make sum_multi_threads

echo "##########################################"


# Run the math.c file.
time ./sum_serial 1000000
time ./sum_multi_threads 100 1000000