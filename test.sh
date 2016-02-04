#!/bin/bash

for (( i=1; i<=65536; i*=2 )); do
	ii=`printf %05d $i`
	./main $i > virtual_$ii.dat
done