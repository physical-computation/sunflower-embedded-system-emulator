#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFB_Bayesian_MAP imagelists/fafb.srt imagelists/feret_gallery.srt distances/feret/Bayesian_MAP

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFC_Bayesian_MAP imagelists/fafc.srt imagelists/feret_gallery.srt distances/feret/Bayesian_MAP

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP1_Bayesian_MAP imagelists/dup1.srt imagelists/feret_gallery.srt distances/feret/Bayesian_MAP

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP2_Bayesian_MAP imagelists/dup2.srt imagelists/feret_gallery.srt distances/feret/Bayesian_MAP

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFB_Bayesian_ML imagelists/fafb.srt imagelists/feret_gallery.srt distances/feret/Bayesian_ML

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFC_Bayesian_ML imagelists/fafc.srt imagelists/feret_gallery.srt distances/feret/Bayesian_ML

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP1_Bayesian_ML imagelists/dup1.srt imagelists/feret_gallery.srt distances/feret/Bayesian_ML

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP2_Bayesian_ML imagelists/dup2.srt imagelists/feret_gallery.srt distances/feret/Bayesian_ML

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFB_PCA_Euclidean imagelists/fafb.srt imagelists/feret_gallery.srt distances/feret/PCA_Euclidean

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFC_PCA_Euclidean imagelists/fafc.srt imagelists/feret_gallery.srt distances/feret/PCA_Euclidean

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP1_PCA_Euclidean imagelists/dup1.srt imagelists/feret_gallery.srt distances/feret/PCA_Euclidean

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP2_PCA_Euclidean imagelists/dup2.srt imagelists/feret_gallery.srt distances/feret/PCA_Euclidean

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFB_PCA_MahAngle imagelists/fafb.srt imagelists/feret_gallery.srt distances/feret/PCA_MahAngle

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFC_PCA_MahAngle imagelists/fafc.srt imagelists/feret_gallery.srt distances/feret/PCA_MahAngle

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP1_PCA_MahAngle imagelists/dup1.srt imagelists/feret_gallery.srt distances/feret/PCA_MahAngle

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP2_PCA_MahAngle imagelists/dup2.srt imagelists/feret_gallery.srt distances/feret/PCA_MahAngle

# Calculating the rank curves for fafb

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFB_LDA_ldaSoft imagelists/fafb.srt imagelists/feret_gallery.srt distances/feret/LDA_ldaSoft

# Calculating the rank curves for fafc

run csuAnalyzeRankCurve -outDir results -filePrefix feretFAFC_LDA_ldaSoft imagelists/fafc.srt imagelists/feret_gallery.srt distances/feret/LDA_ldaSoft

# Calculating the rank curves for dup1

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP1_LDA_ldaSoft imagelists/dup1.srt imagelists/feret_gallery.srt distances/feret/LDA_ldaSoft

# Calculating the rank curves for dup2

run csuAnalyzeRankCurve -outDir results -filePrefix feretDUP2_LDA_ldaSoft imagelists/dup2.srt imagelists/feret_gallery.srt distances/feret/LDA_ldaSoft
