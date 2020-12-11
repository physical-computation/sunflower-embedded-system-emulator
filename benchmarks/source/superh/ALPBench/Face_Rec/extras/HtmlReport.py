#
# This Python module contains functions which generate HTML
# reports of experiments
#
# Author: Marcio Luis Teixeira
# Date:   1/11/2003
#

import sys
import string
import os
import math

class HtmlReport:
	def __init__(self, dir):
		self.dir = dir
		self.pageName = os.path.join (dir, "index.html")
		self.uniqueLabels = 0
		self.plotOptions = {}
		self.plotOptions = {}
		self.plotOptions["graphFormat"] = "png color"
		self.plotOptions["hideLegend"] = 0
		self.plotOptions["logx"] = 0
		self.plotOptions["size"] = "1,1"
		self.plotOptions["axis"] = ""
		self.plotOptions["titlePos"] = ""
		self.plotOptions["plotStyle"] = "linespoints"
		self.plotOptions["plotXRange"] = ""
		self.plotOptions["plotBoxWidth"] = ""
		self.queuedPlots = []
    
		print "Saving HTML document to " + self.pageName
		self.fh = open (self.pageName,'w')

	def plot_aux (self, p, title, xLabel, yLabel, outputFile, options):
		p.write ("set xlabel '" + xLabel + "'\n")
		p.write ("set ylabel '" + yLabel + "'\n")
		#p.write ("set xtics 5\n")
		#p.write ("set ytics 0.1\n")
		p.write ("set term " + options["graphFormat"] + "\n")
		p.write ("set output '" + outputFile + "'\n")
		p.write ("set boxwidth "  + options["plotBoxWidth"] + "\n")
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
		p.write ("set size " + options["size"] + "\n")
		p.write ("set title '" + title + "' " + options["titlePos"] + " 'helvetica,42'\n")

		# If the options specify a category map, then we have
		# to convert the hash into something that looks like
		# this:
		#
		#    set xtics ("April" 1, "May" 2, "June" 3, "July" 4)
		#
		if options.has_key("plotCategoryMap"):
			map = options["plotCategoryMap"]
			strs = []
			for key in map.keys():
				strs.append("\"" + map[key] + "\" " + str(key))
				
				p.write (
					"set xtics (" +
					string.join (strs, ", ") +
					")\n"
					)

		# Send the plot data
		
		first = 1;
	
		for pair in self.queuedPlots:
			if first:
				p.write ("plot '-' " + options["plotXRange"] + " " + pair[0])
				first = 0
			else:
				p.write (" ,                '-' " + pair[0])
				
		p.write("\n");
		
		for pair in self.queuedPlots:
			p.write (pair[1])
                        
		p.write("\n");
                    
		p.write ("quit\n")

		# Clear the queued data
		
		self.queuedPlots = []

	def plot (self, title, xLabel, yLabel, outputFile, options):
		p = os.popen ("gnuplot",'w')
		self.plot_aux (p, title, xLabel, yLabel, outputFile, options)
		p.close ()

		fname = outputFile + ".txt";
		p = open (fname, 'w')
		self.plot_aux (p, title, xLabel, yLabel, outputFile, options)
		p.close ()

	# Adds a data series to the list of data to be plotted. The data
	# series is a collection of pairs in the form (X,Y). To actually
	# generate a plot of the accumulated data, you must call "plot()"
	
	def addDataSeries (self, label, data, plotStyle = "linespoints"):
		formatStr = "title \'" + label + "\' with " + plotStyle
		dataStr = ""
		for pair in data:
			dataStr = dataStr + `pair[0]` + " " + `pair[1]` + "\n"
		dataStr = dataStr + "e\n"
		self.queuedPlots.append ((formatStr,dataStr))
		
	def close (self):
		self.fh.close ()
	
	def getUniqueLabel (self):
		self.uniqueLabels = self.uniqueLabels + 1
		return "l" + str(self.uniqueLabels)

	def getUniqueFilename (self, extension):
		return self.getUniqueLabel () + extension

	def writePageHead (self, pageTitle):
		self.fh.write ("<HTML>\n")
		self.fh.write ("<HEAD>\n")
		self.fh.write ("<TITLE>")
		self.fh.write (pageTitle)
		self.fh.write ("</TITLE>\n")	
		self.fh.write ("</HEAD>\n")
		self.fh.write ("<BODY>\n")
		self.fh.write ("<H1>")
		self.fh.write (pageTitle)
		self.fh.write ("</H1>\n")

	def writePageTail (self):
		self.fh.write ("</BODY>\n")
		self.fh.write ("</HTML>\n")

	def writeSeparator (self):
		self.fh.write ("<HR>\n")

	def writeParagraph (self, text = 0):
		self.fh.write ("<P>\n")
		if text != 0:
			self.fh.write (text)
			self.fh.write("</P>\n")

	def writeSectionHeading (self, anchor, text):
		self.fh.write ("<P><A NAME=\"")
		self.fh.write (anchor)
		self.fh.write ("\">\n")
		self.fh.write ("<H2>")
		self.fh.write (text)
		self.fh.write ("</H2>\n")
		self.fh.write ("</A><P>")
	
	def writeImgTag (self, imageName):
		self.fh.write ("<IMG SRC=\"")
		self.fh.write (imageName)
		self.fh.write ("\">\n")

	def writeLink (self, name, link):
		self.fh.write ("<A HREF=\"" + link + "\">")
		self.fh.write (name)
		self.fh.write ("</A>\n")
	
	def writeLinkList (self, links):
		self.fh.write ("<UL>\n")
		for l in links:
			self.fh.write ("<LI><A HREF=#")
			self.fh.write (l)
			self.fh.write (">" + l + "</A></LI>\n")
		self.fh.write ("</UL>\n")

	def writeList (self, items):
		self.fh.write ("<UL>\n")
		for i in items:
			self.fh.write ("<LI>")
			self.fh.write (i)
			self.fh.write ("</LI>\n")
		self.fh.write ("</UL>\n")

	def writeLogChoice (self, log, link):
		self.fh.write ("<BR><STRONG>Axis choice:</STRONG>\n")
		if log != "":
			self.fh.write ("<A HREF='index.html#" + link + "'>[Linear]</A>")
			self.fh.write ("[Log]")
		else:
			self.fh.write ("[Linear]")
			self.fh.write ("<A HREF='logarithmic.html#" + link + "'>[Log]</A>")
			self.fh.write ("<P>\n")

	def writeParameter (self, label, data):
		self.fh.write ("<STRONG>" + label + ":</STRONG> " + str(data) + "<BR>\n")
	
	def writePlot (self, title, xLabel, yLabel):
		outputFile = self.getUniqueFilename (".png")
		self.plot (title, xLabel, yLabel,
		      os.path.join (self.dir, outputFile),
		      self.plotOptions)
		self.writeImgTag (outputFile)

		self.writeLink ("[txt]", outputFile + ".txt")
		self.writeParagraph ()
		
	# Sets the box width for box plots
	
	def setBoxWidth (self, width = ""):
		self.plotOptions["plotBoxWidth"] = str(width)
		
	# Sets the plot style (e.g. linespoints, boxes).
	
	def setPlotStyle (self, style = "linespoints"):
		self.plotOptions["plotStyle"] = style

        # Sets the plot size, relative to the default size. For example,
        # setPlotSize(0.5) scales the plot by one-half

        def setPlotSize (self, x = [], y = []):
                if x == []:
                        x = 1
                if y == []:
                        y = x
                self.plotOptions["size"] = str(x) + "," + str(y)

	# Applies a map to values on the x-axis. This is used to label
	# categorical data, for example.

	def setCategoryMap (self, map = 0):
		if map == 0:
			try:
				del self.plotOptions["plotCategoryMap"]
			except:
				pass
		else:
			self.plotOptions["plotCategoryMap"] = map

	# Sets the x-axis range

	def setXRange (self, min = [], max = []):
		if min == []:
			self.plotOptions["plotXRange"] = ""
		else:
			self.plotOptions["plotXRange"] = "[" + str(min) + ":" + str(max) + "]"


