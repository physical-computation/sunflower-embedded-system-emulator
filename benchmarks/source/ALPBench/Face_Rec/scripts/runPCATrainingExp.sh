#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    echo Running command: $@
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Using normalized raster images from directory <data/FERET/normSep2002sfi>


echo Creating directory for training data.

run mkdir -p train/feret results

echo Running standard feret PCA training on file $1

run nice 20 csuSubspaceTrain -dropNVectors 1 -cutOffMode ENERGY -cutOff 90 -imDir data/FERET/normSep2002sfi $1 train/feret/PCA$1.train

echo Clearing out old distances and/or creating new distances directory


mkdir -p distances/feret/PCA$1_MahAngle

echo Projecting face images onto PCA basis and computing distances

run nice 20 csuSubspaceProject -imDir data/FERET/normSep2002sfi train/feret/PCA$1.train imagelists/all.srt distances/feret/PCA$1_MahAngle MahAngle

run echo 'Indicator file used by the scripts to know whether the data in this directory is valid' > distances/feret/pcaDataExists.txt

echo Doing permutation tests for PCA$1_MahAngle

run nice 20 csuAnalyzePermute distances/feret/PCA$1_MahAngle imagelists/list1024_all_subset.srt results/PCA$1_MahAngle_

echo Computing feret curves for PCA$1_MahAngle

run nice 20 csuAnalyzeRankCurve imagelists/fafb.srt imagelists/feret_gallery.srt distances/feret/PCA$1_MahAngle -filePrefix results/PCA$1_MahAngle_FAFB
run nice 20 csuAnalyzeRankCurve imagelists/fafc.srt imagelists/feret_gallery.srt distances/feret/PCA$1_MahAngle -filePrefix results/PCA$1_MahAngle_FAFC
run nice 20 csuAnalyzeRankCurve imagelists/dup1.srt imagelists/feret_gallery.srt distances/feret/PCA$1_MahAngle -filePrefix results/PCA$1_MahAngle_DUP1
run nice 20 csuAnalyzeRankCurve imagelists/dup2.srt imagelists/feret_gallery.srt distances/feret/PCA$1_MahAngle -filePrefix results/PCA$1_MahAngle_DUP2

rm -rf distances/feret/PCA$1_MahAngle
