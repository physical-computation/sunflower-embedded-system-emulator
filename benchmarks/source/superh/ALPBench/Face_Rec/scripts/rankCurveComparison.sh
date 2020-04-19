#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

mkdir -p $1

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -filePrefix FAFB imagelists/fafb.srt imagelists/feret_gallery.srt -outDir $@

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -filePrefix FAFC imagelists/fafc.srt imagelists/feret_gallery.srt -outDir $@

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -filePrefix DUP1 imagelists/dup1.srt imagelists/feret_gallery.srt -outDir $@

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -filePrefix DUP2 imagelists/dup2.srt imagelists/feret_gallery.srt -outDir $@


