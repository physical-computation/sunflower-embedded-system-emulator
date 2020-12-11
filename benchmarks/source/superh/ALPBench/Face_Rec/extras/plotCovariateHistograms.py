#!/usr/bin/python

#
# A script for reading the results file from csuCovariateStudy
# and generating plots using "gnuplot"
#
# Author: Marcio Luis Teixeira
# Date:   1/11/2003
#

import sys
import string
import os
import math

from HtmlReport import *
from Statistics import *

# This function prints usage information

def printHelp ():
    print "Usage: " + sys.argv[0] + " [-html dir]"
    print
    print "By default, this program will create a new directory for the plots"
    print "and create a postscript plot for each of the covariates"
    print
    print "If the -html option is used, the program will create PNG images"
    print "and write out an HTML page that shows all the plots on one page."
    print
    sys.exit(1)

# Process the command line arguments

def processCommandLineArguments (args,options):
	
    # Print help if the number of arguments is not what we expect
	
    if len (args) < 1:
        printHelp()
		
    args = args[1:] # Pop the program name

    while len (args):
            
        # Pop the topmost argument into arg

        arg = args[0]
        args = args[1:]
            
        if arg == "-html":
            options["htmlPage"]       = 1
            options["graphFormat"]    = "png color"
            options["graphExtension"] = ".png"
        elif arg == "-input":
            options["inputFile"] = args[0]
            args = args[1:]
        else:
            options["outputDir"] = arg

def writeCovariateHistograms (values, trials, requestedTrials, label, categoryMaps = 0):

    if categoryMaps != 0:
        buckets = getHistogramOrdinalBuckets (max(values))

        report.setXRange (min(values)-1, max(values)+1)
        report.setBoxWidth (0.9)
        report.setCategoryMap (categoryMaps[1])
    else:
        buckets = getHistogramBucketParameters (min (values), max (values), 20)
                
        report.setXRange ()
        report.setBoxWidth ()
        report.setCategoryMap ()

    hist = {}
    for t in requestedTrials:
                                
        # Now we extract the values corresponding
        # to trial
            
        subset = vectorExtract (values, trials[t])         

        # Generate a histogram and add it to the
        # report

        hist[t] = makeHistogram (subset, buckets)
                
    report.setPlotSize (1.0, 0.40)
    report.setPlotStyle ("boxes")

    report.writePlot (hist, label, "value", "count")

# Find the minimum and maximum values of a group of sequences.
# The argument is a hash table that maps from a sequence label
# to a vector of numbers.
def findMinMax (sequences):
    keys = sequences.keys ()
    minVal = min (sequences[keys[0]])
    maxVal = max (sequences[keys[0]])
    for k in keys[1:]:
        minVal = min (minVal, min (sequences[k]))
        maxVal = max (maxVal, max (sequences[k]))
    return minVal, maxVal
    
def writeHistogram (data, categoryMaps = 0):
    histMin, histMax = findMinMax (data)

    if categoryMaps != 0:
        buckets = getHistogramOrdinalBuckets (histMax)

        report.setXRange (histMin-1, histMax+1)
        report.setBoxWidth (0.9)
        report.setCategoryMap (categoryMaps[1])
    else:
        buckets = getHistogramBucketParameters (histMin, histMax, 20)
                
        report.setXRange ()
        report.setBoxWidth ()
        report.setCategoryMap ()
        
    report.setPlotStyle ("boxes")

    report.writePlot (hist, label, "value", "count")
    

# MAIN PROGRAM

measure1 = "feret_training_Euclidean_RANK"
measure2 = "feret_training_Bayesian_ML_RANK"

if __name__=="__main__":

    options = {
        "inputFile"      : "../results/CovStudy/feret_training.covStudy",
        "graphFormat"    : "postscript color 24",
        "graphExtension" : ".ps",
        "outputDir"      : os.path.join ("results", "CovStudy", "html"),
        "hideLegend"     : 0
        }

    processCommandLineArguments (sys.argv, options)

    os.system("mkdir -p " + options["outputDir"])

    print "Reading " + options["inputFile"] + "\n"
    data = readDelimitedFile (options["inputFile"], ",")

    labels     = data.keys ()
    nColumns   = len (labels)
    nTrials    = len (data[labels[0]])

    print "There are " + str(nColumns) + " columns in the data"
    print "There are " + str(nTrials)  + " trials in the data"

    # Find the subset of data points corresponding to the
    # four possible scenarios for successes and failures of
    # the two algorithms: FF, FS, SF, and SS. We create four
    # vectors of indices into the data array, which can later
    # be used to extract the appropriate values from any
    # given data vector

    trials = {}
    trials["FF"] = []
    trials["FS"] = []
    trials["SF"] = []
    trials["SS"] = []
    trials["ML_F"] = []
    trials["ML_S"] = []
    trials["Euc_F"] = []
    trials["Euc_S"] = []
    trials["all"] = []
    
    for i in range (nTrials):
        m1DidOk = data[measure1][i] == "0"
        m2DidOk = data[measure2][i] == "0"

        trials["all"].append(i)
        if m1DidOk and m2DidOk:
            trials["SS"].append (i)
            trials["Euc_S"].append (i)
            trials["ML_S"].append (i)
        elif m1DidOk:
            trials["SF"].append (i)
            trials["Euc_S"].append (i)
            trials["ML_F"].append (i)
        elif m2DidOk:
            trials["FS"].append (i)
            trials["Euc_F"].append (i)
            trials["ML_S"].append (i)
        else:
            trials["FF"].append (i)
            trials["Euc_F"].append (i)
            trials["ML_F"].append (i)

    # Save the HTML report

    report = HtmlReport (options["outputDir"])
    
    report.writePageHead ("plotCovariateHistograms results")

##    report.writeParagraph (
##        "This covariate study is designed to show under which conditions the Euclidean "
##        "classifier outperforms the Maximum Likelihood classifer, or vice-versa. For "
##        "each trial, we define a success as a correct rank one classification by that algorithm."
##        "We discard all the trials for which both algorithms either both failed "
##        "or both suceeded (FF and SS). For the remaining trials (FS and SF), we go through "
##        "each of the covariates and generate two superimposed histograms which show the "
##        "distributions of the covariates for the FS and SF trials (a histogram of the all "
##        "the trials are provided for comparison). If a covariate has the potential to "
##        "act as a predictor of success of one algorithm over the other, then we expect the "
##        "two superimposed distributions to be offset relative to each other. If algorithm "
##        "performances are independent of the the covariates, we expect that the FS and SF "
##        "distributions will mirror the overall distribution for all trials."
##        )

    report.writeParagraph (
        "This covariate study is designed to show the conditions under which the Euclidean "
        "and ML algorithms succeed or fail in doing a correct classification. "
        "We define a success as a correct rank one classification by that algorithm. "
        "For each covariate, we generate a histogram of the successful trials and another for "
        "the unsuccessful trials."
        "Each histogram shows both the Bayesian and Euclidean algorithms to allow comparison of the two."
        "An additional histogram shows the overall distribution of all the trials."
        ) 
    
    report.writeSectionHeading ("param", "Parameters:")
    report.writeParameter ("S0", measure1 + " = 0")
    report.writeParameter ("S1", measure2 + " = 0")
    report.writeParagraph ()
    report.writeParameter ("Trials Euc_F", len (trials["Euc_F"]))
    report.writeParameter ("Trials Euc_S", len (trials["Euc_S"]))
    report.writeParameter ("Trials ML_F", len (trials["ML_F"]))
    report.writeParameter ("Trials ML_S", len (trials["ML_S"]))


    # Show the individual statistics

    subjectCovariates = [
        "AGE", "AGE2", "GENDER", "RACE", "SKIN", "SKIN2"
       ]

    imageCovariates = [
        "BANGS", "EXPRESSION", "EYES", "FACIAL_HAIR", "GLASSES", "MAKEUP", "MOUTH", "MOUTH2",
        
        "MEAN_WHOLE",     "MED_WHOLE",     "STD_WHOLE",     "MAD_WHOLE",
        "EYE_DIST",       "EYE_ANGLE",
        "MEAN_EYES",      "MED_EYES",      "STD_EYES",      "MAD_EYES",
        "MEAN_FH",        "MED_FH",        "STD_FH",        "MAD_FH",
        "MEAN_CENTR",     "MED_CENTR",     "STD_CENTR",     "MAD_CENTR",
        "MEAN_MC",        "MED_MC",        "STD_MC",        "MAD_MC",
        "DMEAN_LR_SIDES", "DMED_LR_SIDES", "RSTD_LR_SIDES", "RMAD_LR_SIDES", "TSTAT_LR_SIDES",
        "DMEAN_LR_EYES",  "DMED_LR_EYES",  "RSTD_LR_EYES",  "RMAD_LR_EYES",  "TSTAT_LR_EYES",
        "DMEAN_FH_CENTR", "DMED_FH_CENTR", "RSTD_FH_CENTR", "RMAD_FH_CENTR", "TSTAT_FH_CENTR",
        "DMEAN_FH_MC",    "DMED_FH_MC",    "RSTD_FH_MC",    "RMAD_FH_MC",    "TSTAT_FH_MC",
        "DMEAN_CENTR_MC", "DMED_CENTR_MC", "RSTD_CENTR_MC", "RMAD_CENTR_MC", "TSTAT_CENTR_MC",
        "DMEAN_LR_CHEEK", "DMED_LR_CHEEK", "RSTD_LR_CHEEK", "RMAD_LR_CHEEK", "TSTAT_LR_CHEEK"
        ]

    report.writeSectionHeading ("gloss", "Glossary:")
    
    report.writeList(
        [ "WHOLE: Whole face",
          "EYES: Combined left and right eye regions",
          "FH: Forehead strip (top 1/3 of image)",
          "CENTR: Center strip (middle 1/3 of image)",
          "MC: Mouth/Chin strip (bottom 1/3 of image)",
          "MEAN: Mean pixel value of region",
          "MED: Median pixel value of region",
          "MAD: Median absolute deviation pixel value of region",
          "STD: Standard Deviation of pixels in region",
          "DMEAN: Difference between the mean of the two regions",
          "DMED: Difference between the median of the two regions",
          "RSTD: Ratio between the standard deviations of the two regions",
          "RMAD: Ratio between the MAD of the two regions",
          "TSTAT: T-Statistic computed between the two regions"
          ]);
    
    report.writeSectionHeading ("SubjCovIndex", "Subject covariates:")
    report.writeLinkList (subjectCovariates)
    report.writeSectionHeading ("ImageCovIndex", "Image covariates:")
    report.writeLinkList (imageCovariates)
    
    # Generate histograms for the subject covariates

    report.writeSectionHeading ("param", "Subject covariates:")
    
    for cov in subjectCovariates:
        print "Generating histogram for " + cov
        report.writeSectionHeading (cov, cov)

        categoryMaps = getCategoryToOrdinalMaps (data ["SUBJ_" + cov])
        values = convertCategoricalToOrdinalData (data ["SUBJ_" + cov], categoryMaps)
        
        # Write the histograms
        
        writeCovariateHistograms (values, trials, ["Euc_S","ML_S"], cov, categoryMaps)
        writeCovariateHistograms (values, trials, ["Euc_F","ML_F"], cov, categoryMaps)
        writeCovariateHistograms (values, trials, ["all"], cov, categoryMaps)

        report.writeSeparator ()

    # Generate histograms for the image covariates

    report.writeSectionHeading ("param", "Image covariates:")
        
    for cov in imageCovariates:
        print "Generating histogram for " + cov
        report.writeSectionHeading (cov, cov)

        try:
            values1 = vectorToFloat (data ["IMAGE_1_" + cov])
            values2 = vectorToFloat (data ["IMAGE_2_" + cov])
            categoryMaps = 0
        except ValueError:
            combinedData = data ["IMAGE_1_" + cov] + data ["IMAGE_2_" + cov]
            categoryMaps = getCategoryToOrdinalMaps (combinedData)
            values1 = convertCategoricalToOrdinalData (data ["IMAGE_1_" + cov], categoryMaps)
            values2 = convertCategoricalToOrdinalData (data ["IMAGE_2_" + cov], categoryMaps)

        # Write the histograms
            
        writeCovariateHistograms (values1, trials, ["Euc_S", "ML_S"], "IMAGE_1:" + cov, categoryMaps)
        writeCovariateHistograms (values1, trials, ["Euc_F", "ML_F"], "IMAGE_1:" + cov, categoryMaps)
        writeCovariateHistograms (values1, trials, ["all"],  "IMAGE_1:" + cov, categoryMaps)

        writeCovariateHistograms (values2, trials, ["Euc_S", "ML_S"], "IMAGE_2:" + cov, categoryMaps)
        writeCovariateHistograms (values2, trials, ["Euc_F", "ML_F"], "IMAGE_2:" + cov, categoryMaps)
        writeCovariateHistograms (values2, trials, ["all"],  "IMAGE_2:" + cov, categoryMaps)

        if (categoryMaps == 0):
            valuesD = vectorSubtract (values1, values2)

            writeCovariateHistograms (valuesD, trials, ["Euc_S", "ML_S"], "DIFF:" + cov, categoryMaps)
            writeCovariateHistograms (valuesD, trials, ["Euc_F", "ML_F"], "DIFF:" + cov, categoryMaps)
            writeCovariateHistograms (valuesD, trials, ["all"],     "DIFF:" + cov, categoryMaps)

        report.writeSeparator ()

    report.writePageTail ()
    report.close ()

