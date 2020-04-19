#!/bin/sh

########## PREAMBLE #########

if [ ! -f scripts/runPreprocessing_feret.sh ]; then
    echo "Please run this command from the csuEvalFaceRec directory"
    exit 1
fi

run() {
    $@ || { echo "Command failed: $@"; echo; exit 1; }
}

###### End of PREAMBLE ######

PATH=$PATH:bin/`arch`
export PATH

FERET_PGM_SOURCE="data/FERET/source/pgm"
FERET_SFI_DIR="data/FERET/normSep2002sfi"
FERET_NRM_DIR="data/FERET/normSep2002nrm"
FERET_PGM_DIR="data/FERET/normSep2002pgm"
EYE_COORDS="imagelists/coords.3368"

mkdir -p $FERET_SFI_DIR $FERET_NRM_DIR $FERET_PGM_DIR

echo "Running Preprocessing on FERET images. Using CSU Defaults for Optional Settings"
run csuPreprocessNormalize $EYE_COORDS $FERET_PGM_SOURCE -sfi  $FERET_SFI_DIR -nrm $FERET_NRM_DIR -pgm $FERET_PGM_DIR 

    
