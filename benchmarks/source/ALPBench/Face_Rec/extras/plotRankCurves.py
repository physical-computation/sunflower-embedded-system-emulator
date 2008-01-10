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
	print "Usage: " + sys.argv[0] + " [-html dir]"
	print
	print "By default, this program will create a new directory for the plots"
	print "and create a postscript plot for each of the distance measures"
	print "recorded in the rank curve file."
	print
	print "If the -html option is used, the program will create PNG images"
	print "and write out an HTML page that shows all the plots on one page."
	print
	sys.exit(1)

# Uses "gnuplot" to plot a set of (x,y) pairs, based on Kai She's
# original perl script. The x and y vectors must be of the
# same length.

def plot (data, title, xLabel, yLabel, outputFile, options):
	p = os.popen ("gnuplot",'w')
	p.write ("set xlabel '" + xLabel + "'\n")
	p.write ("set ylabel '" + yLabel + "'\n")
	p.write ("set xtics 5\n")
	p.write ("set ytics 0.1\n")
	p.write ("set term " + options["graphFormat"] + "\n")
	p.write ("set output '" + outputFile + "'\n")
	if (options["hideLegend"]):
		p.write ("set nokey\n")
	else:
		p.write ("set key right Right bottom\n")
		p.write ("set key left Left bottom\n")
	#p.write ("set grid ytics\n")
	if (options["logx"]):
	    p.write ("set logscale x\n")
        #p.write ("set term postscript 24\n")
        p.write ("set pointsize 1.00\n")
	p.write ("set title '" + title + "' +9,-17 'helvetica,42'\n")
	
        first = 0;

        keys = data.keys ()
	keys.sort()
	
	for file in keys:
        	if first == 0:
                	p.write ("plot [1:50] [0:1] '-' title \'" + file + "\' with linespoints ")
                        first = 1
                else:
                	p.write (" ,                '-' title \'" + file + "\' with linespoints ")
                        
        p.write("\n");
                    
	for file in keys:
		v = data[file]
		for pair in v:
			p.write( `pair[0]` + " " + `pair[1]` + "\n")
		
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

def writeLogChoice (fh, log, link):
	fh.write ("<BR><STRONG>Axis choice:</STRONG>\n")
	if log != "":
		fh.write ("<A HREF='index.html#" + link + "'>[Linear]</A>")
		fh.write ("[Log]")
	else:
		fh.write ("[Linear]")
		fh.write ("<A HREF='logarithmic.html#" + link + "'>[Log]</A>")
	fh.write ("<P>\n")
	
# Writes out an HTML page

def writeHtmlPage (pageName, measures, log = ""):
	print "Saving HTML document to " + pageName
	fh = open (pageName,'w')
	writePageHead (fh, "makeRankCurves results")
	writeLinkList (fh,measures)

	for m in measures:
		writeSectionHeading (fh,m,"Rank curve for " + m + " dataset")
		writeImgTag (fh,m + log + ".png")
		writeLogChoice(fh,log,m)
		writeParagraph (fh)
		
	writePageTail (fh)
	fh.close ()

# This function reads a rank curve file

def readRankCurve (filename, y={}):
	fileHandle = open (filename)
	
	# The first line of the file is a header that tells which tests were run
	# The first word of that file out to be "Rank"

	headers = string.split (fileHandle.readline ())

	if headers[0] != "Rank":
		print "This does not appear to be a valid results file!"
		sys.exit (1)
	
	# Read in the data file. As we read through the
	# fields, we append values to the appropriate list
	
	while 1:
	   	line = fileHandle.readline();
		if not line: break

		fields = string.split (line);
		
		rank = float (fields[0])

		# We only look at every other column, because the odd number
		# columns only contain values we don't need.
		
		for fieldNo in range (2, len (fields), 2):
			column = headers[fieldNo]
                        measure = column[(string.rfind (column, '/')+1):]
			value = float (fields[fieldNo])

			if not y.has_key(measure):
				y[measure] = []
				
			y[measure].append ([rank,value])

        fileHandle.close ()

	return y

# Process the command line arguments

def processCommandLineArguments (args,options):
	
	# Print help if the number of arguments is not what we expect
	
	if len (args) < 1:
		printHelp()
		
	args = args[1:] # Pop the program name

	while len (args):
		if args[0] == "-html":
			options["htmlPage"]       = 1
			options["graphFormat"]    = "png color"
			options["graphExtension"] = ".png"
		else:
			options["outputDir"] = args[0]
		args = args[1:]

# MAIN PROGRAM

#datasets = ['feretFAFB', 'feretFAFC','feretDUP1', 'feretDUP2']
#measures = ['PCA_Euclidean', 'PCA_MahAngle', 'LDA_ldaSoft', 'BayesianML', 'BayesianMAP']

datasets = ['feretTinyFAFB', 'feretTinyFAFC','feretTinyDUP1', 'feretTinyDUP2']
measures = ['PCA_Euclidean', 'PCA_MahAngle', 'Bayesian_MAP', 'Bayesian_ML']

if __name__=="__main__":

	options = {
		"graphFormat"    : "postscript color 24",
		"graphExtension" : ".ps",
		"outputDir"      : os.path.join ("results", "html"),
		"hideLegend"     : 0
		}

	processCommandLineArguments (sys.argv, options)

	os.system("mkdir -p " + options["outputDir"])

	for dataset in datasets:
		data = {}
		
		for measure in measures:
			print "Reading " + dataset + "_" + measure + "_Curve.txt"
					
			inName = os.path.join ("results", dataset + "_" + measure + "_Curve.txt")
			readRankCurve (inName, data)

		#options["hideLegend"] = (dataset != "feretFAFB")

		options["logx"] = 0
		outName =  options["outputDir"] + "/" + dataset + options["graphExtension"];
		plot (data, dataset, "Rank", "Percentage of correct matches at rank", outName, options)
		print "Saving graph to " + outName

		options["logx"] = 1
		outName =  options["outputDir"] + "/" + dataset + "Log" + options["graphExtension"];
		plot (data, dataset, "Rank", "Percentage of correct matches at rank", outName, options)
		print "Saving graph to " + outName		

	if options.has_key("htmlPage"):
		writeHtmlPage (options["outputDir"] + "/index.html", datasets)
		writeHtmlPage (options["outputDir"] + "/logarithmic.html", datasets, "Log")
