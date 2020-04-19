Authors
=======
Alex Li (manlapli@uiuc.edu)
Ruchira Sasanka (sasanka@uiuc.edu)


Modifications of CMU Sphinx 3-0.1 Speech Recognizer
===================================================

The current recognizer is a subset of the CMU Sphinx 3-0.1. The original version
can be found at http://cmusphinx.sourceforge.net/html/cmusphinx.php. Details of
the base version can be found in README and the doc directory. This document 
focuses on the modifications made to the base version.

This release is a stripped down version of the original version and the 
directory hierarchy has also been changed. This version produces the livepretend
binaries that takes in an audio sample and attempts to translate the speech 
sample into text. 

To compile, type 'make' at the main directory. The makefile includes comments
that explain the different options in this release.

To test the program, first modify $HOME in sphinx3-test to point to the main 
directory. Then, at the main directory, do './scripts/sphinx3-test' and the
sample P I T T S B U R G H should be recognized. If a big endian machine is
used to run the program, change the file name in model/lm/an4/an4.ctl to
pittsburgh.bigendian.

The modifications of Sphinx-3 are

I. Parallelization of Gaussian scoring, lextree node evaluation, and
lextree node de/activations.

II. SIMDification of vector quantization and Gaussian scoring.

I. Parallelization
------------------

Three phases are parallelized in the Sphinx 3-0.1 program. The threading.c file 
contains all the multithreaded code used in the parallelization. The three
phases are

1) Gaussian model scoring,
2) Lexical tree node evaluation, and
3) Lexical tree node propagation.

POSIX threads are used to divide the workload. The multithreading initiation is
called at the utt_decode_block function in utt.c. The actual thread creations 
are carried out at thrd_scoring_phase function in threading.c. The threading
model adopts the fork-barrier-join model. Threads encounter a barrier after 
each parallel phase and wait until the next parallel phase starts. After the
last parallel phase, the threads are joined.

In Gaussian model scoring phase, threads evenly divide the large number of 
senones and calculate the Mahalanobis distance between the feature vector and 
the given Gaussian model.

Lextree nodes are activated based on the senone scores generated from phase one.
In lextree node evaluation, the active nodes in the lextree are evenly divided
among threads so that HMM scores are evaluated in all the active HMM models.

In lextree node propagation, the active nodes are evenly divided among threads
so that the next iteration's active nodes are collectively generated based on
the scores generated from the last phase.

All these parallelized code can be found in threading.c

II. SIMDification
-----------------

Vector quantization and Gaussian scoring are enhanced with Intel SSE2
instructions. The SSE2 instructions are embedded as inline assembly
code using MSASM format.

Note: In order to execute the SSE2 version, please use Intel ICC
compiler. The non-commercial version of ICC can be obtained from
http://www.intel.com/cd/software/products/asmo-na/eng/compilers/219771.htm

(1) Vector Quantization : The Mahalanobis distance is calculated
during vector quantization in vector_gautbl_eval_logs3_inner in
vector.c. Single-precision floating-point SSE2 multiply, subtract, and
add instructions are used. 4 vector elements can be processed by each
SSE2 instruction.

(2) Gaussian Scoring : During Gaussian scoring, the Mahalanobis
distances between the input feature vector (contains 39 elements) and
the continuous Gaussian models are calculated in mgau_eval_inner
function in cont_mgau.c. Similar operations are done as in vector
quantization described previously, except with a input that contains
more elements.

