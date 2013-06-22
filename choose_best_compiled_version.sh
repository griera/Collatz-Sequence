#!/bin/bash
# Type the number of versions, each one compiled with different optimization flags
N=$1

# Put in the array all arguments referred to each version of program
# Convention: Put at first position the version compiled without any optimization flags
ARRAY=($2 $3 $4 $5 $6 $7)

# ORIGINAL is the program version compiled without any optimization flags
ORIGINAL=${ARRAY[0]}

# OPTIMIZED is each of the program version compiling with optimization flags
# Its value changes for each program version inside the array
OPTIMIZED=${ARRAY[1]}

# NEXEC specifies the number of executions of each program version to compute the average of elapsed time
NEXEC=$8

OUTPUT_REF="output_ref.txt"
INPUT_REF="input_ref.txt"
INPUT_TIME="input_time.txt"

i=1
best_time=0
best_version=$ORIGINAL

sum_original=0
elapsed_original=0

elapsed_optimized=0

while [ $i -lt $N ]; do
	OPTIMIZED=${ARRAY[${i}]}
	./${OPTIMIZED} < $INPUT_REF > /tmp/output_${OPTIMIZED}.txt
	if [ -n "$(diff $OUTPUT_REF /tmp/output_${OPTIMIZED}.txt)" ]
	then
		echo "No correct results for version ${OPTIMIZED} with the input file called $INPUT_REF"
		echo "There are differences between $OUTPUT_REF file (correct output) and /tmp/output_${OPTIMIZED}.txt)output file generated"
		echo "ERROR: The script execution failed"
		rm /tmp/output_*
		exit
	fi

	j=0
	sum_optimized=0
	while [ $j -lt $NEXEC ]; do
		if [ $i -eq 1 ]
		then
			time_output=$(/usr/bin/time -f "%e %P" ./$ORIGINAL < $INPUT_TIME 2>&1 1>/dev/null)
			elapsed_original=$(echo "$time_output" | cut -d' ' -f1)
			percent_cpu=$(echo "$time_output" | cut -d' ' -f2 | cut -d'%' -f1)
			if [ $percent_cpu -ge 98 ]
			then
				sum_original=$(echo $sum_original + $elapsed_original | bc -l)

			else
				echo "%CPU was not significant enough (${percent_cpu}%) for the execution number $j of ${ORIGINAL} program."
				echo "Try again"
				continue
			fi
		fi
		time_output=$(/usr/bin/time -f "%e %P" ./${OPTIMIZED} < $INPUT_TIME 2>&1 1>/dev/null)
		elapsed_optimized=$(echo "$time_output" | cut -d' ' -f1)
		percent_cpu=$(echo "$time_output" | cut -d' ' -f2 | cut -d'%' -f1)
		
		if [ $percent_cpu -ge 98 ]
		then
			sum_optimized=$(echo $sum_optimized + $elapsed_optimized | bc -l)
		else
			echo "%CPU was not significant enough (${percent_cpu}%) for the execution number $j of ${OPTIMIZED} program."
			echo "Try again"
			continue
		fi
		let j=$j+1
	done

	if [ $i -eq 1 ]
	then
		avg_elapsed_original=$(echo $sum_original/$NEXEC | bc -l | awk '{printf "%.4f", $0}')
		best_time=$avg_elapsed_original
	fi

	avg_elapsed_optimized=$(echo $sum_optimized/$NEXEC | bc -l | awk '{printf "%.4f", $0}')


	if [[ $(echo "if (${avg_elapsed_optimized} <= ${best_time}) 1 else 0" | bc) -eq 1 ]]
	then
		best_time=$avg_elapsed_optimized
		best_version=${OPTIMIZED};
	fi

	if [ ! -d "execution_time_results" ]
	then
		mkdir execution_time_results
	fi

	if [ $i -eq 1 ]
	then
		echo "${INPUT_TIME} $avg_elapsed_original" >> execution_time_results/time_results_${ORIGINAL}.txt
	fi

	echo "${INPUT_TIME} $avg_elapsed_optimized" >> execution_time_results/time_results_${OPTIMIZED}.txt
	let i=$i+1
done

rm /tmp/output_*

echo "SUCCESS: The script has finished successfully"
echo ""
echo "The best version of program is $best_version, with an elpased time of $best_time seconds"
echo "Time results of this test can be found in \"execution_time_results\" folder created inside $PWD"

speedup=$(echo $avg_elapsed_original/$best_time | bc -l | awk '{printf "%.2f", $0}')
speedup_percent=$(echo "($speedup - 1) * 100" | bc -l | awk '{printf "%.2f", $0}')

echo ""
echo "The speedup of $best_version program over the $ORIGINAL program (without compiler optimizations) is ${speedup}X (${speedup_percent}%)"
echo ""
echo "All of versions have been executed using the same input file (${INPUT_TIME})."


