#!/bin/bash

## Command line paramethers
ORIGINAL=$1
OPTIMIZED=$2
NEXEC=$3


## Input and output files to prove the correctness of optimized program over the original program
OUTPUT_REF="output_ref.txt"
INPUT_REF="input_ref.txt"


## Input file with a considerable input data used to measure execution time of both programs
INPUT_TIME="input_time.txt"


## Generate the outputs for the original and optimized program
./$ORIGINAL < $INPUT_REF > /tmp/output_original.txt
./$OPTIMIZED < $INPUT_REF > /tmp/output_optimized.txt


## Compare the previous outputs with original (and correct) outputs
if [ -n "$(diff /tmp/output_original.txt $OUTPUT_REF)" ]
then
	echo "No correct results for $ORIGINAL program compared to reference output $OUTPUT_REF"
	echo "ERROR: The script execution failed"
	rm /tmp/output_original.txt
	exit
fi

if [ -n "$(diff /tmp/output_original.txt $OUTPUT_REF)" ]
then
	echo "No correct results for $OPTIMIZED program compared to reference output $OUTPUT_REF"
	echo "ERROR: The script execution failed"
	rm /tmp/output_original.txt /tmp/output_optimized.txt
	exit
fi
rm /tmp/output_original.txt /tmp/output_optimized.txt


## Measure elapsed time of original and optimized programs during NEXEC executions
j=0
sum_original=0
sum_optimized=0
while [ $j -lt $NEXEC ]; do
	time_output=$(/usr/bin/time -f "%e %P" ./$ORIGINAL < $INPUT_TIME 2>&1 1>/dev/null)
	elapsed_original=$(echo "$time_output" | cut -d' ' -f1)
	percent_cpu=$(echo "$time_output" | cut -d' ' -f2 | cut -d'%' -f1)
	if [ $percent_cpu -ge 98 ]
	then
		sum_original=$(echo "$sum_original + $elapsed_original" | bc -l)
		VECTOR_TIMES_ORIG[$j]=$elapsed_original
	else
		echo "%CPU was not significant enough (${percent_cpu}%) for the execution number $j of ${ORIGINAL} program."
		echo "Try again"
		continue
	fi

	time_output=$(/usr/bin/time -f "%e %P" ./${OPTIMIZED} < $INPUT_TIME 2>&1 1>/dev/null)
	elapsed_optimized=$(echo "$time_output" | cut -d' ' -f1)
	percent_cpu=$(echo "$time_output" | cut -d' ' -f2 | cut -d'%' -f1)
	if [ $percent_cpu -ge 98 ]
	then
		sum_optimized=$(echo "$sum_optimized + $elapsed_optimized" | bc -l)
		VECTOR_TIMES_OPT[$j]=$elapsed_optimized
	else
		echo "%CPU was not significant enough (${percent_cpu}%) for the execution number $j of ${OPTIMIZED} program."
		echo "Try again"
		continue
	fi
	let j=$j+1
done


## Compute the average time for each program and compute the speedup
avg_elapsed_original=$(echo "$sum_original/$NEXEC" | bc -l | awk '{printf "%.2f", $0}')
avg_elapsed_optimized=$(echo "$sum_optimized/$NEXEC" | bc -l | awk '{printf "%.2f", $0}')
speedup=$(echo "$avg_elapsed_original/$avg_elapsed_optimized" | bc -l | awk '{printf "%.2f", $0}')
speedup_percent=$(echo "($speedup - 1) * 100" | bc -l)


## Compute standard deviation for the two previous populations
acum=0
for i in "${VECTOR_TIMES_ORIG[@]}"; do
	acum=$(echo "$acum + (($i - $avg_elapsed_original)^2)" | bc -l)
done
stddev_orig=$(echo "sqrt(($acum/($NEXEC - 1)))" | bc -l | awk '{printf "%.2f", $0}')


acum=0
for i in "${VECTOR_TIMES_OPT[@]}"; do
	acum=$(echo "$acum + (($i - $avg_elapsed_optimized)^2)" | bc -l)
done
stddev_opt=$(echo "sqrt(($acum/($NEXEC - 1)))" | bc -l | awk '{printf "%.2f", $0}')


## Write the final results into a file for each program version
echo "*** SUCCESS ***: The script has finished successfully"

echo "${VECTOR_TIMES_ORIG[@]}" > time_results_original.txt
echo "${VECTOR_TIMES_OPT[@]}" > time_results_optimized.txt

echo "Average time: $avg_elapsed_original" >> time_results_original.txt
echo "Average time: $avg_elapsed_optimized" >> time_results_optimized.txt

echo "Standard deviation: $stddev_orig" >> time_results_original.txt
echo "Standard deviation: $stddev_opt" >> time_results_optimized.txt

echo ""
echo "The average elapsed time of $ORIGINAL program is: ${avg_elapsed_original} seconds."
echo "The average elapsed time of $OPTIMIZED program is: ${avg_elapsed_optimized} seconds."

echo ""
echo "The speedup of $OPTIMIZED program over the $ORIGINAL program is: ${speedup}X ($speedup_percent%)"
echo "(all of versions have been executed using the same input file (${INPUT_TIME})."

echo ""
echo "The standard deviation of $ORIGINAL program with $NEXEC executions is: ${stddev_orig}"
echo "The standard deviation of $OPTIMIZED program with $NEXEC executions is: ${stddev_opt}"

if [ ! -d "execution_time_results" ]
then
	mkdir execution_time_results
fi

mv time_results_original.txt execution_time_results/${ORIGINAL}_time_results.txt
mv time_results_optimized.txt execution_time_results/${OPTIMIZED}_time_results.txt

echo ""
echo "Final results of this test can be found in \"execution_time_results\" folder created inside $PWD"

