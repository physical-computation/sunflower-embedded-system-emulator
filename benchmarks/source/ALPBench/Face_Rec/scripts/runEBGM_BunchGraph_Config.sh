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

EXP_NAME="BunchGraphConfig"

# NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD"
# FACE_GRAPH_DIR="data/${EXP_NAME}/FG"
# PHASE2_DIR="distances/${EXP_NAME}/Phase02"
# GEO1_DIR="distances/${EXP_NAME}/GEO1"
# GEO2_DIR="distances/${EXP_NAME}/GEO2"

for BASE in bunchgraph001A.list bunchgraph001B.list bunchgraph001C.list bunchgraph001D.list \
            bunchgraph001E.list bunchgraph001F.list bunchgraph001G.list bunchgraph001H.list \
            bunchgraph001I.list bunchgraph001J.list bunchgraph002A.list bunchgraph002B.list \
            bunchgraph002C.list bunchgraph002D.list bunchgraph002E.list bunchgraph002F.list \
            bunchgraph002G.list bunchgraph002H.list bunchgraph002I.list bunchgraph002J.list \
            bunchgraph004A.list bunchgraph004B.list bunchgraph004C.list bunchgraph004D.list \
            bunchgraph004E.list bunchgraph004F.list bunchgraph004G.list bunchgraph004H.list \
            bunchgraph004I.list bunchgraph004J.list bunchgraph008A.list bunchgraph008B.list \
            bunchgraph008C.list bunchgraph008D.list bunchgraph008E.list bunchgraph008F.list \
            bunchgraph008G.list bunchgraph008H.list bunchgraph008I.list bunchgraph008J.list \
            bunchgraph016A.list bunchgraph016B.list bunchgraph016C.list bunchgraph016D.list \
            bunchgraph016E.list bunchgraph016F.list bunchgraph016G.list bunchgraph016H.list \
            bunchgraph016I.list bunchgraph016J.list bunchgraph032A.list bunchgraph032B.list \
            bunchgraph032C.list bunchgraph032D.list bunchgraph032E.list bunchgraph032F.list \
            bunchgraph032G.list bunchgraph032H.list bunchgraph032I.list bunchgraph032J.list \
            bunchgraph064A.list bunchgraph064B.list bunchgraph064C.list bunchgraph064D.list \
            bunchgraph064E.list bunchgraph064F.list bunchgraph064G.list bunchgraph064H.list \
            bunchgraph064I.list bunchgraph064J.list bunchgraph128A.list bunchgraph128B.list \
            bunchgraph128C.list bunchgraph128D.list bunchgraph128E.list bunchgraph128F.list \
            bunchgraph128G.list bunchgraph128H.list bunchgraph128I.list bunchgraph128J.list
do

    BUNCH_LIST="ebgmdata/BunchGraphConfig/${BASE}"
    BASENAME=`basename ${BASE} .list`
    NOVEL_GRAPH_DIR="data/${EXP_NAME}/GD_${BASENAME}"
    FACE_GRAPH_DIR="data/${EXP_NAME}/FG_${BASENAME}"
    PHASE2_DIR="distances/${EXP_NAME}/Phase02_${BASENAME}"
    GEO1_DIR="distances/${EXP_NAME}/GEO1_${BASENAME}"
    GEO2_DIR="distances/${EXP_NAME}/GEO2_${BASENAME}"

    run mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $PHASE2_DIR $GEO1_DIR $GEO2_DIR


    echo "Fitting face graphs to novel images"

    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST_LOW  $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR &
    run bin/`arch`/csuEBGMGraphFit $BUNCH_LIST $NOVEL_IMAGE_LIST_HIGH $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR $DE $NOVEL_GRAPH_DIR &
    wait

    echo "Extracting face graphs from images"

    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST_LOW  $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR &
    run bin/`arch`/csuEBGMFaceGraph $NOVEL_IMAGE_LIST_HIGH $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR &
    wait

    echo "Creating distance matrix"

    run bin/`arch`/csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR JetSimPhase02 $PHASE2_DIR GeoSimL2 $GEO1_DIR  GeoSimLeastSquares $GEO2_DIR


    echo "Removing old face graph files because they take up too much space"

    run rm -r $FACE_GRAPH_DIR


    echo "Finished running EBGM algorithm for $BUNCH_LIST"
done

# grep ^1\\\> *CM* | cut --bytes=11,12,13,33-
