#!/bin/sh
for i in `seq 1 100` ; do ./timeout3 -t 70 java Client 130.237.218.85 10001 $i ; done
