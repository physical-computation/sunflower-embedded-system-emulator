#
# This Python module contains functions which are useful for
# doing statistical studies on data
#
# Author: Marcio Luis Teixeira
# Date:   1/11/2003
#

import sys
import string
import os
import math

##############################################################
# I/O Functions
##############################################################

# This function reads in a table of data which is
# stored as a comma delimited file. The first
# line is assumed to be the column names. The
# return of this function is a hash-table.
# There is a hash-table entry for each column
# name and the corresponding entry is a vector
# of values from that column (stored as strings).

def readDelimitedFile (filename, delim=","):
    
    fileHandle = open (filename)

    # The first line is the list of field titles

    headers = string.split (string.strip (fileHandle.readline ()), delim)

    # Now initialize the data structure for the data.
    # We will have a hash table which maps field names
    # to vectors

    data = {}
    for h in headers:
        data[h] = []

    # Now read in the data

    while 1:
        line = string.strip (fileHandle.readline());
        if not line: break

        fields = string.split (line, delim)
        i = 0;
        for h in headers:        
            data[h].append(fields[i])
            i = i + 1

    return data

##############################################################
# Operations on vectors
##############################################################

# Converts all the elements of a string vector
# into a floating point number and returns it
# as a new vector

def vectorToFloat (v1):
    v = []
    for i in range (len (v1)):
        v.append (float (v1[i]))
    return v

# Performs an elementwise subtraction on two
# vectors of numbers and returns the result
# as a new vector

def vectorSubtract (v1, v2):
    v = []
    for i in range (len (v1)):
        v.append (v1[i] - v2[i])
    return v

# Given a vector and a list of indices,
# returns a new vector containing the
# specified elements of the original vector

def vectorExtract (v1, indices):
    v = []
    for i in indices:
        v.append (v1[i])
    return v

##############################################################
# Histogram Functions
##############################################################

# Returns a set of nBins histogram buckets spanning the space
# from valMin to valMax. This function does not work well
# for ordinal data.

def getHistogramBucketParameters (histMin, histMax, nBins = 10):
    bucketSize = float(histMax - histMin) / nBins
    return (histMin, bucketSize, nBins)

# This function works well for ordinal data

def getHistogramOrdinalBuckets (max):
    return getHistogramBucketParameters(1, max+1, max)

# Computes the histogram of a vector of numbers. The first
# parameter is the data, the second one is either the number
# of buckets to use, or the data which is returned from
# getHistogramBucketParameters. The latter case is useful
# when one wishes to have multiple histograms plotted on
# the same set of buckets. This histogram function works
# only on ordinal or interval data. When dealing with
# categorical data, it is important to use first
# convert the data into ordinal using the function
# convertCategoricalToOrdinal

def makeHistogram (data, buckets = 10):

    # If we passed in a number of buckets, rather
    # than actual buckets, then compute the buckets
    # based on the data
    
    if type(buckets) == type(0):
        buckets = getHistogramBucketParameters (min(data), max(data), buckets)
            
    hist       = []
    histMin    = buckets[0]
    bucketSize = buckets[1]
    nBins      = buckets[2]
    
    # Create the histogram buckets
    
    for i in range (nBins):
        hist.append (0)

    # Count the number of values which
    # fall into each bucket
    
    for i in range (len (data)):
        bucket = int((data[i] - histMin)/bucketSize)
        if bucket == nBins: # Special case: data[i]=max(data)
            bucket = nBins - 1
        hist[bucket] = hist[bucket] + 1

    # Convert the buckets into pairs
    
    pairs = []
    for i in range (nBins):
        pairs.append ((histMin + i*bucketSize, hist[i]))

    return pairs

# Returns maps which can be used to convert
# a categorical data into ordinal data and
# back. This function returns both the forward
# and backward map

def getCategoryToOrdinalMaps (data):
	forwardMap = {}
	backwardMap = {}
	ordinal = 1
        for category in data:
		if not forwardMap.has_key(category):
			forwardMap[category] = ordinal
			backwardMap[ordinal] = category
			ordinal = ordinal + 1
	return forwardMap, backwardMap
    
# Converts categorical data into into ordinal data.
# This function takes two arguments. The first is
# the data, the second is the pair of maps which is
# obtained from getCategoryToOrdinalMaps

def convertCategoricalToOrdinalData (data, maps = 0):
    res = []
    if maps == 0:
        maps = getCategoryToOrdinalMaps (data)
    for category in data:
        res.append (maps[0][category])
    return res

# Undoes the conversion from categorical to ordinal
# data that was performed by convertCategoricalToOrdinalData
# This function takes two arguments. The first is
# the data, the second is the pair of maps which
# were used to do the inital conversion.

def convertOrdinalToCategoricalData (data, maps):
    res = []
    for ordinal in data:
        res.append (maps[1][ordinal])
    return res


