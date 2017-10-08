#!/bin/bash

RESULT_FILE="cppcheck_results.txt"
if [ -f "$RESULT_FILE" ]
then
    rm -f "$RESULT_FILE~"
    mv $RESULT_FILE "$RESULT_FILE~"
elif [ ! -f "$RESULT_FILE~" ]
then
    > "$RESULT_FILE~"
fi


./startStaticAnalysis.sh $RESULT_FILE &&
cmp $RESULT_FILE "$RESULT_FILE~" || gvimdiff $RESULT_FILE "$RESULT_FILE~" 
