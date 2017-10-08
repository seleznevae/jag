#!/bin/bash
JOBS_NUMBER=2

DIRS_AND_FILES_TO_IGNORE='../src/core/qcustomplot.cpp'
DIRS_AND_FILES_TO_IGNORE=$DIRS_AND_FILES_TO_IGNORE' ../src/qsint/'
DIRS_AND_FILES_TO_IGNORE=$DIRS_AND_FILES_TO_IGNORE' ../src/fullScreen/'
DIRS_AND_FILES_TO_IGNORE=$DIRS_AND_FILES_TO_IGNORE' ../src/simple_fft/'


if [ "$#" -ge 1 ]
then
    RESULT_FILE="$1"
else
    RESULT_FILE="cppcheck_results.txt"
fi
###################################################################

IGNORE_STRING=`echo $DIRS_AND_FILES_TO_IGNORE | awk '
{ for (i = 1; i <= NF; ++i) {
    printf " -i  "$i
  }
}
'`
cppcheck -q -j $JOBS_NUMBER --enable=all --inconclusive  $IGNORE_STRING ../src 2>$RESULT_FILE &&
    echo  "static analysis is finished"
