Authors
=======
Alex Li (manlapli@uiuc.edu)
Ruchira Sasanka (sasanka@uiuc.edu)


Modifications of MPEG2 decoder
==============================

This decoder is a modified version of the MSSG MPEG2 decoder. The
original version can be found at
http://www.mpeg.org/MSSG/#source. Details of the base decoder can be
found in README, ARCHITECTURE, and m2d_old.doc. This document focuses
on the modifications of the decoder.

The modifications made to MSSG MPEG2 decoder are

I. Parallelization of frame decoding

II. SIMDifications of motion compensation and iDCT.

To run MPEG2 decoder

1. make
2. cd verify
3. ../execs/mpeg2dec -b test.m2v -o0 out%d

I. Parallelization
------------------

POSIX threads are used to parallelize the MPEG2 decoder. The parallelization 
uses a fork-join model for every frame, i.e., forking out different threads at 
the beginning of the bitstream processing and joining all threads after segments
of the frame are reconstructed from the bitstream.

Since the bitstream is inherently serial, the main thread first indentifies the 
slices (a line of macroblocks in a frame that can be decoded independently) from
the bitstream and then distributes equal number of slices to different threads.
Thus, each thread is responsible for a specific part of the frame (e.g. top half
and bottom half). The main thread copies the slices into the private buffers of 
the threads so that decoding can be carried out independently. The slice 
distribution and multithreading initiation are done in new_slice function in 
getpic.c. After the slice is processed (i.e. Entropy decoding, IDCT, and 
iQuant), each thread writes to the corresponding part in the frame buffer. 
Then, all threads are joined and the main thread outputs the reconstructed 
frame.

As the original version is single-thread, most of the functions are written 
assuming a single buffer. In order to enable parallel processing of the 
bitstream, many of the functions are modified to access the private buffers of
different threads. These functions are prefixed with Thrd_, e.g. 
Thrd_decode_macroblock.

II. SIMDification 
-----------------

Motion compensation and inverse DCT are enhanced with Intel SSE2
instructions. The SSE2 instructions are embedded as inline assembly
code using MSASM format.

Note: In order to execute the SSE2 version, please use Intel ICC
compiler. The non-commercial version of ICC can be obtained from
http://www.intel.com/cd/software/products/asmo-na/eng/compilers/219771.htm

(1) Motion Compensation : The prediction for the current block is
formed in form_predictions function in recon.c. The SSE2 instructions
are used to do packed average computation. The Saturate_Block function
in getpic.c is SSE2-enchanced to perform clipping. Lastly, the error
is added to the reference block to get the current block, which is
done in Add_Block function in getpic.c.

(2) Inverse DCT : The SSE2-enhanced iDCT uses a butterfly algorithm
based on Intel Application Notes 922 (AP-922). The SSE2 version of the
fast iDCT (idct.c) is available on the Internet at
http://www.koders.com/cpp/fid284A1B63C25B61F528B1A110F8920EAEC6BFB69F.aspx

The inverse transform uses a 2-step (row and column) approach for the
2-dimensional 8x8 iDCT.  The row step performs a dot product of the
input and the coefficient matrices. The multiply-add and SIMD
add/subtract instructions are used to efficiently compute the row
iDCT.  The column step is less regular. However, identical operations
are performed on each column. Thus, while scalar instructions perform
column iDCT column by column, the SSE2-enhanced version is able to
operate on 8 columns simultaneously.



