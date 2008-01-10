#!/bin/sh

########## PREAMBLE #########
PATH=$PATH:bin/`arch`

export PATH

run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

msg(){
    echo $@
}

###### End of PREAMBLE ######

WAVELET="ebgmdata/GaborMaskWiskott.wavelet"
EBGM_SFI_DIR="data/ebgmJan2003sfi"
EBGM_PGM_DIR="data/ebgmJan2003pgm"
MODEL_GRAPH_DIR="ebgmdata/graph122902"
MODEL_LIST="imagelists/rand122902_70.list"
NOVEL_GRAPH_DIR="data/novelGraphsWiskott"
NOVEL_IMAGE_LIST="imagelists/all.srt"
NOVEL_IMAGE_LIST_LO="imagelists/all.lo.srt"
NOVEL_IMAGE_LIST_HI="imagelists/all.hi.srt"
FACE_GRAPH_DIR="data/faceGraphsWiskott"
PHASE2_DIR="distances/BetaPhase02/"
MAG_DIR="distances/BetaMag/"

mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $PHASE2_DIR $MAG_DIR 

msg "Fitting face graphs to novel images"

# Optimized for duel processor machines.  This command can be replaced with by a single command using all.srt.

csuEBGMGraphFit $MODEL_LIST $NOVEL_IMAGE_LIST_LO $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR PHASE02 $NOVEL_GRAPH_DIR > graphfit.lo &
csuEBGMGraphFit $MODEL_LIST $NOVEL_IMAGE_LIST_HI $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR PHASE02 $NOVEL_GRAPH_DIR > graphfit.hi &
wait

msg "Extracting face graphs from images"
csuEBGMFaceGraph $NOVEL_IMAGE_LIST_LO $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR > facegraph.lo &
csuEBGMFaceGraph $NOVEL_IMAGE_LIST_HI $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR > facegraph.hi &
wait

msg "Creating distance matrix"
csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR JetSimPhase02 $PHASE2_DIR JetSimMag $MAG_DIR
msg "Finished running EBGM algorithm"

