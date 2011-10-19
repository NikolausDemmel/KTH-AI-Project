#!/bin/sh
for i in `seq 1 100` ; do echo "TESTING BOARD $i" && ./timeout3 -t 70 ./client-opt 130.237.218.85 10001 $i ; done
