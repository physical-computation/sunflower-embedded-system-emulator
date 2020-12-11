#!/bin/sh

run() {
    echo "Exicuting command: $@"
    $@ || { echo Command failed: $@; echo; exit 1; }
}

WAVELET="ebgmdata/GaborMaskWiskott.wavelet"
EBGM_SFI_DIR="data/gaborSep2002sfi/"
EBGM_PGM_DIR="data/ebgmJan2003pgm"
MODEL_GRAPH_DIR="ebgmdata/graph122902"
NOVEL_IMAGE_LIST="imagelists/list640.srt"
NOVEL_IMAGE_LIST_LOW="imagelists/list640low.srt"
NOVEL_IMAGE_LIST_HIGH="imagelists/list640high.srt"
DE="DEPredictiveStep"
BUNCH_LIST="imagelists/ebgm70.srt"

EXP_NAME="FGSimilarityConfig"

# NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD"
# FACE_GRAPH_DIR="data/${EXP_NAME}/FG"
# PHASE2_DIR="distances/${EXP_NAME}/Phase02"
# GEO1_DIR="distances/${EXP_NAME}/GEO1"
# GEO2_DIR="distances/${EXP_NAME}/GEO2"


    BASENAME="EBGM"
    NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD_${BASENAME}"
    FACE_GRAPH_DIR="data/${EXP_NAME}/FG_${BASENAME}"
    Magnitude_DIR="distances/${EXP_NAME}/JetSimMagnitude_${BASENAME}"
    Phase_DIR="distances/${EXP_NAME}/JetSimPhase_${BASENAME}"
    PredictiveStep_DIR="distances/${EXP_NAME}/JetSimPredictiveStep_${BASENAME}"
    PredictiveIter_DIR="distances/${EXP_NAME}/JetSimPredictiveIter_${BASENAME}"
    FixedLocalSearch_DIR="distances/${EXP_NAME}/JetSimFixedLocalSearch_${BASENAME}"
    NarrowingLocalSearch_DIR="distances/${EXP_NAME}/JetSimNarrowingLocalSearch_${BASENAME}"
    GEO_L2_DIR="distances/${EXP_NAME}/GeoSim_L2_${BASENAME}"
    GEO_LeastSquares_DIR="distances/${EXP_NAME}/GeoSim_LeastSquares_${BASENAME}"
    GEO_LeastSquaresPS_DIR="distances/${EXP_NAME}/GeoSim_LeastSquaresPS_${BASENAME}"
    GEO_LeastSquaresPI_DIR="distances/${EXP_NAME}/GeoSim_LeastSquaresPI_${BASENAME}"
    GEO_LeastSquaresFLS_DIR="distances/${EXP_NAME}/GeoSim_LeastSquaresFLS_${BASENAME}"
    GEO_LeastSquaresNLS_DIR="distances/${EXP_NAME}/GeoSim_LeastSquaresNLS_${BASENAME}"

    run mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $Magnitude_DIR $Phase_DIR $PredictiveStep_DIR $PredictiveIter_DIR $FixedLocalSearch_DIR $NarrowingLocalSearch_DIR $GEO_L2_DIR $GEO_LeastSquares_DIR $GEO_LeastSquaresPS_DIR $GEO_LeastSquaresPI_DIR $GEO_LeastSquaresFLS_DIR $GEO_LeastSquaresNLS_DIR


    echo "Fitting face graphs to novel images"

#    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST  $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR
#    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST_LOW  $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR &
#    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST_HIGH $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR &
    wait

    echo "Extracting face graphs from images"

#    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST  $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR
#    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST_LOW  $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR &
#    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST_HIGH $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR &
    wait

    echo "Creating distance matrix"

#    run bin/`arch`/csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR \
        #JetSimMag $Magnitude_DIR \
        #JetSimPhase $Phase_DIR \
        #JetSimPredictiveStep $PredictiveStep_DIR \
        #JetSimPredictiveIter $PredictiveIter_DIR \
        #JetSimNarrowingLocalSearch $NarrowingLocalSearch_DIR \
        #JetSimFixedLocalSearch $FixedLocalSearch_DIR \
        #GeoSimL2 $GEO_L2_DIR \
        #GeoSimLeastSquares $GEO_LeastSquares_DIR \
    run bin/`arch`/csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR \
        GeoSimLeastSquaresPS $GEO_LeastSquaresPS_DIR \
        GeoSimLeastSquaresPI $GEO_LeastSquaresPI_DIR \
        GeoSimLeastSquaresFLS $GEO_LeastSquaresFLS_DIR \
        GeoSimLeastSquaresNLS $GEO_LeastSquaresNLS_DIR


    echo "Removing old face graph files because they take up too much space"

    #run rm -r $FACE_GRAPH_DIR


    echo "Finished running EBGM algorithm for $BUNCH_LIST"
done

# grep ^1\\\> *CM* | cut --bytes=11,12,13,33-

