#!/bin/sh
PATH=$PATH:execs
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Using normalized raster images from directory <data/csuScrapShots/normSep2002sfi>


# Creating directory for training data.

run mkdir -p train/scraps

rm -rf distances/scraps/PCA_Euclidean distances/scraps/PCA_MahCosine
run mkdir -p distances/scraps/PCA_Euclidean distances/scraps/PCA_MahCosine

# Projecting face images onto PCA basis and computing distances

run face_project -imDir data/csuScrapShots/normSep2002sfi train/scraps/feretPCA.train imagelists/scrap_all.srt distances/scraps/PCA_MahCosine MahCosine
run echo 'Indicator file used by the scripts to know whether the data in this directory is valid' > distances/scraps/pcaDataExists.txt

run face_project -imDir data/csuScrapShots/normSep2002sfi -lastphase -infile 00001aa.sfi -outfile distances.dat train/scraps/feretPCA.train imagelists/scrap_all.srt distances/scraps/PCA_MahCosine MahCosine
