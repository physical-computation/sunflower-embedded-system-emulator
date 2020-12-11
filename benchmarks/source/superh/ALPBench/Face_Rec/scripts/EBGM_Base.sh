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
msg "Running EBGM algorithm with the Bochum/USC setings"

# The settings in this file are based on the Bochum/USC
# EBGM algorithm as described in:
#
# Laurenz Wiskott and Jean-Marc Fellous and Norbert Kruger and Christoph von der Malsburg,
# "Face Recognition by Elastic Bunch Graph Matching", no. 96-08, 1996

WAVELET="ebgmdata/GaborMaskWiskott.wavelet"
EBGM_SFI_DIR="data/EBGMSep2002sfi"
MODEL_GRAPH_DIR="ebgmdata/ManualLandmarkLocations"
MODEL_LIST="imagelists/rand122902_70.list"
NOVEL_GRAPH_DIR="data/EBGM_Base_NG"
NOVEL_IMAGE_LIST="imagelists/all.srt"
NOVEL_IMAGE_LIST_LO="imagelists/all.lo.srt"
NOVEL_IMAGE_LIST_HI="imagelists/all.hi.srt"
FACE_GRAPH_DIR="data/EBGM_Base_FG"
FGPS_DIR="distances/EBGM_Base_FGPredictiveStep"
FGMAG_DIR="distances/EBGM_Base_FGMagnitude/"

mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $FGPS_DIR $FGMAG_DIR

msg "Fitting face graphs to novel images"

# Optimized for duel processor machines.  This command can be replaced with by a single command using all.srt.

csuEBGMGraphFit $MODEL_LIST $NOVEL_IMAGE_LIST_LO $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR DEPredictiveStep $NOVEL_GRAPH_DIR  &
csuEBGMGraphFit $MODEL_LIST $NOVEL_IMAGE_LIST_HI $WAVELET $EBGM_SFI_DIR  $MODEL_GRAPH_DIR DEPredictiveStep $NOVEL_GRAPH_DIR  &
wait

msg "Extracting face graphs from images"
csuEBGMFaceGraph $NOVEL_IMAGE_LIST_LO $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR  &
csuEBGMFaceGraph $NOVEL_IMAGE_LIST_HI $EBGM_SFI_DIR $NOVEL_GRAPH_DIR $WAVELET $FACE_GRAPH_DIR  &
wait

msg "Creating distance matrix"
csuEBGMMeasure $NOVEL_IMAGE_LIST $FACE_GRAPH_DIR FGPredictiveStep $FGPS_DIR FGMagnitude $FGMAG_DIR
msg "Finished running EBGM algorithm"

