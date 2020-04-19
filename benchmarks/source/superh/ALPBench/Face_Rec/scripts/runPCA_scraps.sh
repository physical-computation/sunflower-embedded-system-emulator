#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Using normalized raster images from directory <data/csuScrapShots/normSep2002sfi>


# Creating directory for training data.

run mkdir -p train/scraps

# Running standard feret PCA training.

run csuSubspaceTrain -imDir data/csuScrapShots/normSep2002sfi imagelists/scrap_all.srt train/scraps/feretPCA.train

# Clearing out old distances and/or creating new distances directory

rm -rf distances/scraps/PCA_Euclidean distances/scraps/PCA_MahCosine
run mkdir -p distances/scraps/PCA_Euclidean distances/scraps/PCA_MahCosine

# Projecting face images onto PCA basis and computing distances

run csuSubspaceProject -imDir data/csuScrapShots/normSep2002sfi train/scraps/feretPCA.train imagelists/scrap_all_x4.srt distances/scraps/PCA_Euclidean Euclidean distances/scraps/PCA_MahCosine MahCosine
run echo 'Indicator file used by the scripts to know whether the data in this directory is valid' > distances/scraps/pcaDataExists.txt
