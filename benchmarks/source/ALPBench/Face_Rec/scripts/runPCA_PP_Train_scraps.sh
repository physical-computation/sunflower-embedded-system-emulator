#!/bin/sh

########## PREAMBLE #########

if [ ! -f scripts/runPreprocessing_scraps.sh ]; then
    echo "Please run this command from the csuEvalFaceRec directory"
    exit 1
fi

run() {
    $@ || { echo "Command failed: $@"; echo; exit 1; }
}

###### End of PREAMBLE ######

PATH=$PATH:bin/`arch`
export PATH

FERET_PGM_SOURCE="data/csuScrapShots/source/pgm"
FERET_SFI_DIR="data/csuScrapShots/normSep2002sfi"
EYE_COORDS="imagelists/coords.scraps"

mkdir -p $FERET_SFI_DIR 

echo "Running Preprocessing on scrapshots images. Using CSU Defaults for Optional Settings"
run csuPreprocessNormalize $EYE_COORDS $FERET_PGM_SOURCE -sfi  $FERET_SFI_DIR 

# Using normalized raster images from directory <data/csuScrapShots/normSep2002sfi>


# Creating directory for training data.

run mkdir -p train/scraps

# Running standard feret PCA training.

run csuSubspaceTrain -imDir data/csuScrapShots/normSep2002sfi imagelists/scrap_all.srt train/scraps/feretPCA.train

