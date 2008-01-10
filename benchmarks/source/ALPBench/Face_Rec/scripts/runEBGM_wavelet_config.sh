#!/bin/sh

run() {
    echo "Exicuting command: $@"
    $@ || { echo Command failed: $@; echo; exit 1; }
}

WAVELETS="ebgmdata/GaborMaskNestares.wavelet ebgmdata/GaborMaskBolme3.wavelet ebgmdata/GaborMaskWiskott.wavelet"
WAVELET="ebgmdata/GaborMaskWiskott.wavelet"
EBGM_SFI_DIR="data/gaborSep2002sfi/"
EBGM_PGM_DIR="data/ebgmJan2003pgm"
MODEL_GRAPH_DIR="ebgmdata/graph122902"
NOVEL_IMAGE_LIST="imagelists/list640.srt"
NOVEL_IMAGE_LIST_LOW="imagelists/list640low.srt"
NOVEL_IMAGE_LIST_HIGH="imagelists/list640high.srt"
DE="DEPredictiveStep"
BUNCH_LIST="imagelists/ebgm70.srt"

EXP_NAME="WaveletConfig"

# NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD"
# FACE_GRAPH_DIR="data/${EXP_NAME}/FG"
# PHASE2_DIR="distances/${EXP_NAME}/Phase02"
# GEO1_DIR="distances/${EXP_NAME}/GEO1"
# GEO2_DIR="distances/${EXP_NAME}/GEO2"

for BASE in $WAVELETS
do

    #BUNCH_LIST="ebgmdata/BunchGraphConfig/${BASE}"
    WAVELET=${BASE}
    BASENAME=`basename ${BASE} .wavelet`
    NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD_${BASENAME}"
    FACE_GRAPH_DIR="data/${EXP_NAME}/FG_${BASENAME}"
    PHASE2_DIR="distances/${EXP_NAME}/Phase02_${BASENAME}"
    GEO1_DIR="distances/${EXP_NAME}/GEO1_${BASENAME}"
    GEO2_DIR="distances/${EXP_NAME}/GEO2_${BASENAME}"

    run mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $PHASE2_DIR $GEO1_DIR $GEO2_DIR


    echo "Fitting face graphs to novel images"

    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST  $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR
#    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST_LOW  $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR &
#    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST_HIGH $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR &
    wait

    echo "Extracting face graphs from images"

    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST  $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR
#    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST_LOW  $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR &
#    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST_HIGH $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR &
    wait

    echo "Creating distance matrix"

    run bin/`arch`/csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR JetSimPhase02 $PHASE2_DIR GeoSimL2 $GEO1_DIR  GeoSimLeastSquares $GEO2_DIR


    echo "Removing old face graph files because they take up too much space"

    #run rm -r $FACE_GRAPH_DIR


    echo "Finished running EBGM algorithm for $BUNCH_LIST"
done

# grep ^1\\\> *CM* | cut --bytes=11,12,13,33-

