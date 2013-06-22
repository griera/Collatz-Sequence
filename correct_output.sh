#!/bin/bash

PROGRAM=$1

OUTPUT_REF="output_ref.txt"
INPUT_REF="input_ref.txt"
OUTPUT_PROG="/tmp/output_program.txt"

./$PROGRAM < $INPUT_REF > $OUTPUT_PROG

if [ -n "$(diff $OUTPUT_PROG $OUTPUT_REF)" ]
then
	echo "No correct results for $PROGRAM program compared to reference output $OUTPUT_REF"
	echo "ERROR: The script execution failed"
	rm $OUTPUT_PROG
	exit
fi

rm $OUTPUT_PROG

echo "SUCCESS: The script has finished successfully"
echo "The correctness of $PROGRAM program has been proved!"

