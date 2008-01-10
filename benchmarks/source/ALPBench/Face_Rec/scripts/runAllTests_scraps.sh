#!/bin/sh

########## PREAMBLE #########

if [ ! -f scripts/runAllTests_scraps.sh ]; then
    echo "Please run this command from the csuEvalFaceRec directory"
    exit 1
fi

run() {
    $@ || { echo "Command failed: $@"; echo; exit 1; }
}

###### End of PREAMBLE ######

run scripts/runPreprocessing_scraps.sh
run scripts/runPCA_scraps.sh
#run scripts/runLDA_scraps.sh
#run scripts/runBayesian_scraps.sh
