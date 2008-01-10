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

FERET_PGM_SOURCE="data/FERET/source/pgm"
EBGM_SFI_DIR="data/EBGMSep2002sfi"
EBGM_PGM_DIR="data/EBGMSep2002pgm"
EYE_COORDS="imagelists/coords.3368"
GABOROPTIONS="-size 128 128 -mask NO -preNorm YES -pixelNorm YES -hist NONE -eye-target 52 64 76 64 -postEdge 30 -preEdge 30 "

mkdir -p $EBGM_SFI_DIR $EBGM_PGM_DIR

msg "Running Preprocessing for EBGM images. Using EBGM Defaults for Optional Settings"
run csuPreprocessNormalize $GABOROPTIONS $EYE_COORDS $FERET_PGM_SOURCE -sfi  $EBGM_SFI_DIR -pgm $EBGM_PGM_DIR

    
