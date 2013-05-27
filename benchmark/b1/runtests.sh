#!/bin/bash

testpattern="testXXX"
outfile="results.txt"
outfile_tmp="results.txt.tmp"

rm $outfile
gcc -o testgen testgen.c

i=1000
while [ $i -le 1000000 ]
do

	testname=${testpattern//XXX/$i}
	echo $testname

	# generate the test
	./testgen $i > $testname

	# run the tests
	j=0
	while [ $j -lt 1 ]
	do
		/usr/bin/time -a -o $outfile -p ~/jobs/benchmark/r3 < $testname > /dev/null
		head -n -2 $outfile > $outfile_tmp
		mv $outfile_tmp $outfile

		j=$(($j+1))
	done
	rm $testname

	i=$(($i+1000))

done

cat $outfile | sed 's/^.....//' > $outfile_tmp
mv $outfile_tmp $outfile

rm testgen

