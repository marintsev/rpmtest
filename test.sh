#!/bin/bash

NAME=real
for (( i=1; i<=65536; i*=2 )); do
	ii=`printf %05d $i`
	./main $i > $NAME""_$ii.dat
done