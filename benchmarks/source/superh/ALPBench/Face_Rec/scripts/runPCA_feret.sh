#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Using normalized raster images from directory <data/FERET/normSep2002sfi>


# Creating directory for training data.

run mkdir -p train/feret

# Running standard feret PCA training.

run csuSubspaceTrain -imDir data/FERET/normSep2002sfi imagelists/feret_training.srt train/feret/feretPCA.train

# Clearing out old distances and/or creating new distances directory

rm -rf distances/feret/PCA_Euclidean distances/feret/PCA_MahCosine
run mkdir -p distances/feret/PCA_Euclidean distances/feret/PCA_MahCosine

# Projecting face images onto PCA basis and computing distances

run csuSubspaceProject -imDir data/FERET/normSep2002sfi train/feret/feretPCA.train imagelists/all.srt distances/feret/PCA_Euclidean Euclidean distances/feret/PCA_MahCosine MahCosine
run echo 'Indicator file used by the scripts to know whether the data in this directory is valid' > distances/feret/pcaDataExists.txt
