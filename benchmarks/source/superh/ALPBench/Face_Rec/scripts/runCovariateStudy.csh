#!/bin/sh

# PLEASE RUN THIS SCRIPT FROM THE TOP LEVEL DIRECTORY
#
# This script runs a single covariate training experiment and
# generates covariate datafile for processing with statistical
# software.  The script expects one argument which is the
# basename WITHOUT the ".sfi" extention for an image list in
# the imagelist directory.  The script will train on the imagelist
# "imagelist/$1.srt" and produce distance files based on the list.
# To build csuCovariateStudy run "make extra"  this builds cpp
# codes in the extras directory need to run the experiment.
# You also may need csuToolAddSFI to add the sfi extention
# to the back of the imagelists.  This is used by the command:
# "csuToolAddSFI < base.list > new.list"



PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

if [ -z $1 ]; then
    echo Please specify at least one argument: example:
    echo    runCovariateStudy feret_training
    exit 1
fi

IMAGEDIR=data/FERET/images/processed/norm26x30Sep2002sfi/
TRAINING_SRT=imagelists/$1.srt 

run mkdir -p results/CovStudy train/CovStudy \
    distances/CovStudy/$1_Cityblock          \
    distances/CovStudy/$1_Euclidean          \
    distances/CovStudy/$1_Correlation        \
    distances/CovStudy/$1_Covariance         \
    distances/CovStudy/$1_MahCosine          \
    distances/CovStudy/$1_Euclidean          \
    distances/CovStudy/$1_Bayesian_ML        \
    distances/CovStudy/$1_Bayesian_MAP

# Run nearest-neighbor classifier

#run csuSubspaceTrain -imDir $IMAGEDIR -cutOffMode ENERGY $TRAINING_SRT train/CovStudy/$1.train

#run csuSubspaceProject train/CovStudy/$1.train imagelists/all.srt -imDir $IMAGEDIR \
#        distances/CovStudy/$1_MahCosine/ MahCosine \
#        distances/CovStudy/$1_Euclidean/ Euclidean

# Run Bayesian classifier

#run csuBayesianTrain -imDir $IMAGEDIR $TRAINING_SRT train/CovStudy/$1.train

#run csuBayesianProject -imDir $IMAGEDIR train/CovStudy/$1.train.intra train/CovStudy/$1.train.extra imagelists/all.srt \
#    distances/CovStudy/$1_Bayesian_ML distances/CovStudy/$1_Bayesian_MAP

# Generate the covariates

run csuCovariateStudy imagelists/list2144.srt $TRAINING_SRT results/CovStudy/$1.covStudy \
    -cov imagelists/facecovariates.cov      \
    -cov imagelists/imagecovariates.cov     \
    -dist distances/CovStudy/$1_Euclidean   \
    -dist distances/CovStudy/$1_MahCosine   \
    -dist distances/CovStudy/$1_Bayesian_ML \
    -dist distances/CovStudy/$1_Bayesian_MAP

run csuCovariateStudy imagelists/list2144.srt $TRAINING_SRT results/CovStudy/$1.rank5.covStudy \
    -nrank 5                                \
    -cov imagelists/facecovariates.cov      \
    -cov imagelists/imagecovariates.cov     \
    -dist distances/CovStudy/$1_Euclidean   \
    -dist distances/CovStudy/$1_MahCosine   \
    -dist distances/CovStudy/$1_Bayesian_ML \
    -dist distances/CovStudy/$1_Bayesian_MAP