#!/bin/sh

PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}


# this script selects distance directories in the feret directory and then generates results for each.

RESULTS_DIR="results/CSUBaselineResults"
DIST_DIR="distances/feret"
VERSION="CSU Face Idenification Evaluation System 5.0"

mkdir -p $RESULTS_DIR

# Collect all directories that have the proper number of distance files. 
basenames=''
dists=''
for i in `ls $DIST_DIR`; do 
    if [ `ls $DIST_DIR/$i | wc -w` = 3368 ]; then 
        basenames="$basenames $i";
        dists="$dists $DIST_DIR/$i";
         
    fi;  
done

echo Generateing results for: $dists

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -filePrefix FAFB imagelists/fafb.srt imagelists/feret_gallery.srt -outDir $RESULTS_DIR $dists

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -filePrefix FAFC imagelists/fafc.srt imagelists/feret_gallery.srt -outDir $RESULTS_DIR $dists

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -filePrefix DUP1 imagelists/dup1.srt imagelists/feret_gallery.srt -outDir $RESULTS_DIR $dists

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -filePrefix DUP2 imagelists/dup2.srt imagelists/feret_gallery.srt -outDir $RESULTS_DIR $dists

run extras/compareRankCurves.py $RESULTS_DIR/DUP1_Curve.txt $RESULTS_DIR/DUP2_Curve.txt $RESULTS_DIR/FAFB_Curve.txt $RESULTS_DIR/FAFC_Curve.txt 


# Run a permutation comparison for all of the results
run csuAnalyzePermute imagelists/list640.srt $RESULTS_DIR/Perm $dists



echo "" > $RESULTS_DIR/index.html
echo "<HTML><HEAD><TITLE>CSU Baseline Results on the Feret Database</TITLE></HEAD><BODY>" >> $RESULTS_DIR/index.html
echo "<H1>CSU Baseline Results on the Feret Database</H1><BR>" >> $RESULTS_DIR/index.html
echo "$VERSION <BR>Result generated at: `date` <HR>" >> $RESULTS_DIR/index.html
probes="FAFB FAFC DUP1 DUP2"
for probeset in $probes; do
    echo $probeset
    echo "<P><H3>${probeset} Results:</H3><BR>" >> $RESULTS_DIR/index.html
    echo "Recognition curves in text format. (<A HREF=\"${probeset}_Curve.txt\">${probeset}_Curve.txt</A>)<BR>"  >> $RESULTS_DIR/index.html
    echo "Individual Recogntion Counts. (<A HREF=\"${probeset}_Images.txt\">${probeset}_Images.txt</A>)<BR>"  >> $RESULTS_DIR/index.html
    echo "Recongnition curves image. (<A HREF=\"${probeset}_Curve.png\">${probeset}_Curve.png</A>)<BR>"  >> $RESULTS_DIR/index.html
    echo "Postscript figure(<A HREF=\"${probeset}_Curve.eps\">${probeset}_Curve.eps</A>)<BR>"  >> $RESULTS_DIR/index.html
    echo "<IMG SRC=\"${probeset}_Curve.png\">"  >> $RESULTS_DIR/index.html
    echo "<HR>" >> $RESULTS_DIR/index.html
done

    echo "<P><H3>Permute Results:</H3><BR>" >> $RESULTS_DIR/index.html
    echo "Summary (<A HREF=\"Perm_Rank1.html\">Perm_Rank1.html</A>)<BR><BR>"  >> $RESULTS_DIR/index.html
for alg in $basenames; do
    echo "Recognition rate curves with error bounds. (<A HREF=\"Perm${alg}_CMCurve.txt\">Perm${alg}_CMCurve.txt</A>)<BR>"  >> $RESULTS_DIR/index.html
    echo "Histogram data of recognition rate counts. (<A HREF=\"Perm${alg}_HistCounts.txt\">Perm${alg}_HistCounts.txt</A>)<BR>"  >> $RESULTS_DIR/index.html
    echo "Histogram data of recognition rate probablities. (<A HREF=\"Perm${alg}_HistProbs.txt\">Perm${alg}_HistProbs.txt</A>)<BR><BR>"  >> $RESULTS_DIR/index.html
done

echo "</BODY></HTML>" >> $RESULTS_DIR/index.html

