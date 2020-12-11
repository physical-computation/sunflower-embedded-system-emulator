#!/bin/sh

run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

usage() {
    echo "This script runs the standard permute tests on multiple distance"
    echo "directories.  The results are all saved to one results directory"
    echo
    echo "usage ${1} RESULTS_DIR [LIST_OF_DISTANCE_DIRS]"
    exit 1
}

if [ $# -eq 0 ]
    then
        usage $0
fi

IMAGELIST="imagelists/list640.srt"
RESULTS_DIR="${1}"
shift

echo Making directory $RESULTS_DIR
run mkdir -p $RESULTS_DIR


while [ "$#" -gt "0" ]
do
    BASE_NAME=${RESULTS_DIR}/Perm`basename $1`
    echo "Geting new Variable"
    DIST_DIR=$1
    shift
    run bin/`arch`/csuAnalyzePermute ${DIST_DIR} ${IMAGELIST} ${BASE_NAME}
done

