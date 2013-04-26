#!/bin/bash

array=(0 0.1 0.25 0.5 0.75 0.875 0.9 0.925 0.95 0.98 0.99 1);
#list=(basic quick race);
list=($1);
qval=$2

for item in ${array[*]}
do
	for type in ${list[*]}
	do
		name=$type-$item
		cp $type.dat $name.dat
		echo $name;
		time ./race-track -f $name -g 10000 -p 0 -q $qval -l $item -d $name
		rm $name.dat
		mv *.tsv results/
		mv *.log results/
	done
done 
