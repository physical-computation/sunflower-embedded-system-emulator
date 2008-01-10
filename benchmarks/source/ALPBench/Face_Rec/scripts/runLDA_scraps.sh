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

run csuSubspaceTrain -lda -imDir data/csuScrapShots/normSep2002sfi imagelists/scrap_all.srt train/scraps/feretLDA.train

# Clearing out old distances and/or creating new distances directory

rm -rf distances/scraps/LDA_ldaSoft
run mkdir -p distances/scraps/LDA_ldaSoft

# Projecting face images onto PCA basis and computing distances

run csuSubspaceProject -imDir data/csuScrapShots/normSep2002sfi train/scraps/feretPCA.train imagelists/scrap_all_x4.srt distances/scraps/LDA_ldaSoft ldaSoft
run echo 'Indicator file used by the scripts to know whether the data in this directory is valid' > distances/scraps/ldaDataExists.txt
