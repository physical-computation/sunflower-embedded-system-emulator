#!/bin/sh

run() {
    echo "Exicuting command: $@"
    $@ || { echo Command failed: $@; echo; exit 1; }
}

WAVELET="ebgmdata/GaborMaskWiskott.wavelet"
EBGM_SFI_DIR="data/FERET/images/processed/gaborSep2002sfi/"
EBGM_PGM_DIR="data/ebgmJan2003pgm"
MODEL_GRAPH_DIR="ebgmdata/graph122902"
BUNCH_LIST="imagelists/rand122902_70.list"
NOVEL_IMAGE_LIST="imagelists/list640.srt"

EXP_NAME="localizationconfig"

# NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD"
# FACE_GRAPH_DIR="data/${EXP_NAME}/FG"
# PHASE2_DIR="distances/${EXP_NAME}/Phase02"
# GEO1_DIR="distances/${EXP_NAME}/GEO1"
# GEO2_DIR="distances/${EXP_NAME}/GEO2"

for DE in DEPredictiveStep DEPredictiveIter DEFixedLocalSearch DENarrowingLocalSearch
do

    NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD_${DE}"
    FACE_GRAPH_DIR="data/${EXP_NAME}/FG_${DE}"
    PHASE2_DIR="distances/${EXP_NAME}/Phase02_${DE}"
    GEO1_DIR="distances/${EXP_NAME}/GEO1_${DE}"
    GEO2_DIR="distances/${EXP_NAME}/GEO2_${DE}"

    run mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $PHASE2_DIR $GEO1_DIR $GEO2_DIR


    echo "Fitting face graphs to novel images"

    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR ${DE} $NOVEL_GRAPH_DIR


    echo "Extracting face graphs from images"

    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR


    echo "Creating distance matrix"

    run bin/`arch`/csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR JetSimPhase02 $PHASE2_DIR GeoSimL2 $GEO1_DIR  GeoSimLeastSquares $GEO2_DIR


    echo "Removing old face graph files"

    # rm -r $FACE_GRAPH_DIR


    echo "Finished running EBGM algorithm $DE"
done

