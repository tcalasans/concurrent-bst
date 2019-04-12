#!/bin/bash

#ELEMENTS=1000000
#MIN_THREADS=1
#MAX_THREADS=32
#PACKAGE_NAME=pa3        

RATIOS_ADD=(70 15 15)
RATIOS_REMOVE=(15 70 15)
RATIOS_CONTAINS=(15 15 70)
NUM_THREADS=(1 2 4 8)
TRANSACTION_SIZES=(1)
ELEMENTS=500000

OUTPUT_FILENAME=paper-output.csv
EXECUTABLE_FILENAME=a.out
echo Compiling...

g++ *.cpp -lpthread -std=c++11 -w
if [ $? -eq 0 ]; then
    echo Compilation succeeeded.
else
    echo Compilation failed
    exit
fi

echo Starting testing...

#rm $OUTPUT_FILENAME
echo "duration(ms), elements, numThreads, percAdd, percRemove, percContains, transaction_size\n" > $OUTPUT_FILENAME

for i in 0 1 2;
do
    for threads in ${NUM_THREADS[*]};
    do
        for transaction_size in ${TRANSACTION_SIZES};
        do
            echo **TESTCASE $threads threads and $ELEMENTS elements, ${RATIOS_ADD[$i]} %add ratio, ${RATIOS_REMOVE[$i]} %remove, ${RATIOS_CONTAINS[$i]} %contains, filename $OUTPUT_FILENAME, trans. size $transaction_size
            #java $PACKAGE_NAME.TestUtil $ELEMENTS $threads $ratio $OUTPUT_FILENAME
            ./$EXECUTABLE_FILENAME $ELEMENTS $threads ${RATIOS_ADD[$i]} ${RATIOS_REMOVE[$i]} ${RATIOS_CONTAINS[$i]} $transaction_size $OUTPUT_FILENAME 
        done
    done
done