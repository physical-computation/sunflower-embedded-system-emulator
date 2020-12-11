#!/usr/bin/python

#
# A script for reading the results file from csuRankCurve
# and generating plots using "gnuplot"
#
# Author: Marcio Luis Teixeira
# Date:   5/28/2002
#

import sys
import string
import os
import math

# This function prints usage information

def printHelp ():
    print "Usage: " + sys.argv[0] + " [-html] rankcurve1 [rankcurve2 [... rankcurveN]]"
    print
    print "  rankcurve1 ... rankcurveN - name of the rank curve file(s) from csuRankCurve"
    print
    print "By default, this program will create a new directory for the plots"
    print "and create a postscript plot for each of the distance measures"
    print "recorded in the rank curve file(s)."
    print
    print "If the -html option is used, the program will create PNG images"
    print "and write out an HTML page that shows all the plots on one page."
    print
    sys.exit(1)

# Uses "gnuplot" to plot a set of (x,y) pairs, based on Kai She's
# original perl script. The x and y vectors must be of the
# same length.

def ploteps (data, options, filename):
    xLabel = "Rank"
    yLabel = "Rate"
    format = "postscript eps color"
    outputFile = filename[:(string.rfind (filename, '.')+1)] + "eps"

    p = os.popen ("gnuplot",'w')
    p.write ("set xlabel '" + xLabel + "'\n")
    p.write ("set ylabel '" + yLabel + "'\n")
    p.write ("set xtics 5\n")
    p.write ("set key bottom\n")
    p.write ("set term " + format + "\n")
    p.write ("set output '" + outputFile + "'\n")
    p.write ("set size 0.7,0.7\n")

    first = 0;

    for measure in data.keys ():
        if measure != "Rank":
            if first == 0:
                p.write ("plot [0:50] '-' title \'" + measure + "\' with points ")
                first = 1
            else:
                p.write (", '-' title \'" + measure + "\' with points ")

    p.write("\n");

    for measure in data.keys ():
        if measure != "Rank":
            for n in range (len(data["Rank"])):
                p.write( `data["Rank"][n]` + " " + `data[measure][n]` + "\n")
            p.write ("e\n")

    p.write ("quit\n")
    p.close ()

def plotpng (data, options, filename):
    xLabel = "Rank"
    yLabel = "Rate"
    format = "png color"
    outputFile = filename[:(string.rfind (filename, '.')+1)] + "png"

    p = os.popen ("gnuplot",'w')
    p.write ("set xlabel '" + xLabel + "'\n")
    p.write ("set ylabel '" + yLabel + "'\n")
    p.write ("set xtics 5\n")
    p.write ("set key bottom\n")
    p.write ("set term " + format + "\n")
    p.write ("set output '" + outputFile + "'\n")
    # p.write ("set size 0.7,0.7\n")

    first = 0;

    for measure in data.keys ():
        if measure != "Rank":
            if first == 0:
                p.write ("plot [0:50] '-' title \'" + measure + "\' with points ")
                first = 1
            else:
                p.write (", '-' title \'" + measure + "\' with points ")

    p.write("\n");

    for measure in data.keys ():
        if measure != "Rank":
            for n in range (len(data["Rank"])):
                p.write( `data["Rank"][n]` + " " + `data[measure][n]` + "\n")
            p.write ("e\n")

    p.write ("quit\n")
    p.close ()


# Helper function for generating HTML tags

def writePageHead (fh, pageTitle):
    fh.write ("<HTML>\n")
    fh.write ("<HEAD>\n")
    fh.write ("<TITLE>")
    fh.write (pageTitle)
    fh.write ("</TITLE>\n")
    fh.write ("</HEAD>\n")
    fh.write ("<BODY>\n")
    fh.write ("<H1>")
    fh.write (pageTitle)
    fh.write ("</H1>\n")

def writePageTail (fh):
    fh.write ("</BODY>\n")
    fh.write ("</HTML>\n")

def writeParagraph (fh):
    fh.write ("<P>\n")

def writeSectionHeading (fh,anchor,text):
    fh.write ("<A NAME=")
    fh.write (anchor)
    fh.write (">\n")
    fh.write ("<H2>")
    fh.write (text)
    fh.write ("</H2>\n")
    fh.write ("</A>")

def writeImgTag (fh,imageName):
    fh.write ("<IMG SRC=\"")
    fh.write (imageName)
    fh.write ("\">\n")

def writeLinkList (fh,links):
    fh.write ("<UL>\n")
    for l in links:
        fh.write ("<LI><A HREF=#")
        fh.write (l)
        fh.write (">" + l + "</A></LI>\n")
    fh.write ("</UL>\n")

# Writes out an HTML page

def writeHtmlPage (options):
    pageName = options["outputDir"] + "/index.html"
    experimentName = options["outputDir"]            
    #for filename in options["fileName"]:
    measures = options["fileName"]


    print "Saving HTML document to " + pageName
    fh = open (pageName,'w')
    writePageHead (fh, "csuRankCurve output for " + experimentName)
    writeLinkList (fh,measures)

    for m in measures:
        writeSectionHeading (fh,m,"Rank curve for " + m + " measure")
        writeImgTag (fh,m + ".png")
        writeParagraph (fh)

    writePageTail (fh)
    fh.close ()

# This function reads a rank curve file and generates graphs
# for each of the measurement metrics in that file

def plotRankCurvesForResult (options,data):

    measures = []
    for column in data[data.keys()[0]].keys ():
        if column != "Rank":
            measure = column[(string.rfind (column, '/')+1):]
            measures.append (measure)
            fName =  options["outputDir"] + "/" + measure + options["graphExtension"];
            ploteps (data, column, "Rank",
                  "Percentage of correct matches at rank", fName, options["graphFormat"])
            plotpng (data, column, "Rank",
                  "Percentage of correct matches at rank", fName, options["graphFormat"])
            print "Saving graph to " + fName

    if options.has_key("htmlPage"):
        writeHtmlPage (options["outputDir"] + "/index.html", options["outputDir"], measures)


# Process the command line arguments

def processCommandLineArguments (args,options):

    # Print help if the number of arguments is not what we expect

    if len (sys.argv) < 2:
        printHelp()

    idx = 1;

    # Parse additional command line arguments

    if args[idx] == "-html":
        options["htmlPage"]       = 1
        options["graphFormat"]    = "png"
        options["graphExtension"] = ".png"
        idx = idx + 1
        options["outputDir"] = sys.argv[idx]
        tmp = len(options["outputDir"])
        os.system("mkdir -p " + options["outputDir"])
        idx = idx + 1

    options["fileName"] = []
    while idx <  len (sys.argv):
        options["fileName"].append( sys.argv[idx] )
        idx = idx + 1


def readRankCurve( filename ):
    fileHandle = open (filename)

    # The first line of the file is a header that tells which tests were run
    # The first word of that file out to be "Rank"

    headers = string.split (fileHandle.readline ())

    if headers[0] != "Rank":
        print "This does not appear to be a valid results file!"
        sys.exit (1)

    # Make sure the directory exists

    fName = headers[1];

    # y is a map from column names to lists.

    y = {}
    for column in headers:
        tmp = column
        if column[-1] == '/':
            tmp = column[:-1]
        measure = tmp[(string.rfind (tmp, '/')+1):]
        y[measure] = []
        print "Using measure" + measure

    # Read in the data file. As we read through the
    # fields, we append values to the appropriate list

    while 1:
        line = fileHandle.readline();
        if not line: break

        fields = string.split (line);

        # We only look at every other column, because the odd number
        # columns only contain values we don't need.

        for fieldNo in range (0, len (fields), 2):
            column = headers[fieldNo]
            tmp = column
            if column[-1] == '/':
                tmp = column[:-1]
            measure = tmp[(string.rfind (tmp, '/')+1):]
            #measure = column[(string.rfind (column, '/')+1):]
            #measure = column
            value = float (fields[fieldNo])
            # print measure
            y[measure].append (value)

    fileHandle.close ()

    return y


# MAIN PROGRAM

if __name__=="__main__":

    options = {
        "graphFormat"    : "postscript",
        "graphExtension" : ".ps"
        }
    processCommandLineArguments (sys.argv, options)

    # Open the file and do it

    try:
            for filename in options["fileName"]:
                print "Filename: <" + filename + ">"
                data = readRankCurve (filename)
                ploteps(data, options, filename)
                plotpng(data, options, filename)
                



    except IOError:
           sys.stderr.write ("File not found: %s\n" + options["fileName"])
