Authors
=======
Alex Li (manlapli@uiuc.edu)
Ruchira Sasanka (sasanka@uiuc.edu)


Modifications of MSSG MPEG2 encoder
===================================

This encoder is a modified version of the MSSG MPEG2 encoder. The
original version can be found at
http://www.mpeg.org/MSSG/#source. Details of the base encoder can be
found in README, ARCHITECTURE, and mpeg2enc.doc. This document focuses
on the modifications of the encoder.

The differences between the current version and the base version are

I. 3-step motion search instead of full search, 

II. butterfly algorithm for forward Discrete Cosine Transform (fDCT),

III. parallelized processing of the frames, and 

IV. SIMDifications of motion estimation, error predictions, fDCT/iDCT,
and quantization/inverse quantization.

To run MPEG2 encoder
1. make
2. cd verify
3. ../execs/mpeg2enc test.par output.m2v
4. use any mpeg2 player to view output.m2v

I. 3-step Motion Search
------------------------

The 3-step search algorithm is written by Eric Debes. Its
implementation can be found in the fullsearch function in motion.c. A
high-level description of the 3 steps taken is followed.

1) Calculate the distance of the center block.

2) If the result from step 1 exceeds the preset threshold, calculate
the distances between the matching block and the blocks in the search
window. Mark the one with minimum error. The blocks are 4 pels apart.

3) Search around the blocks that are next to the center block. More
specifically, the eight surrounding blocks that are 2 pels away from
the center block.

This search algorithm greatly reduces the computations when comparing
with traditional full search algorithm.

II. fDCT Butterfly Algorithm
----------------------------

Forward DCT in the original encoder is done with floating-point matrix
multiplies of the coefficient matrices. The fDCT, located in
fdctref.c, used in this encoder is based on Intel Application Notes
922 (AP-922). The scalar and the MMX version can be found in the FDCT
package at http://www.geocities.com/liaor2/mpeg2avi/mpeg2avi.htm

III. Parallelization
--------------------

POSIX threads are used to parallelize the MPEG2 encoder. The
parallelization uses a fork-join model for every frame, i.e., forking
out different threads at the beginning of the frame processing and
joining all threads before the frame is written to the bitstream.

The multithreading is initiated in the thread_work_dist() function in
putseq.c.  The frame is partitioned statically among different
threads, i.e., for 2 threads, thread 0 takes the top half and thread 1
takes the bottom half of the frame. The needed parameters are then
passed to individual threads so that motion estimation, DCT,
quantization, IDCT, inverse quantization, and entropy coding can be
done in parallel. The threaded functions are prefixed with pt, e.g.,
ptmotion_estimation.

While quantization depends on the bit rate in the original encoder,
the current encoder uses a fix quantization value to allow threads to
perform independent operations. This results in variable bit rate
bitstream. However, current implementations of MPEG2 decoder can
handle VBR encoding and thus it is not a big issue.

To create the bitstream, each thread is given a buffer to write its
encoded frame segment. After thread join is encountered, the main
thread combines all the segments into one bitstream.

IV. SIMDification
-----------------

Motion estimation, prediction error calculation, fDCT/iDCT, and
quantization/inverse quantization are enhanced with Intel SSE2
instructions. The SSE2 instructions are embedded as inline assembly
code using MSASM format.

Note: In order to execute the SSE2 version, please use Intel ICC
compiler.  The non-commercial version of ICC can be obtained from
http://www.intel.com/cd/software/products/asmo-na/eng/compilers/219771.htm

(1) Motion Estimation : Functions dist1, dist2, bdist1, bdist2, and
variance are enhanced with Intel SSE2 instructions. In motion
estimation, the error between two 16x16 blocks are calculated with Sum
of Absolute Differences (SAD) and pel-wise averages. As each row of
the 16x16 blocks contains 16 pels and each XMM register is 128-bit
(16-byte) wide, it takes only 16 SSE2 instructions to operate on 256
pels.

(2) Prediction Error Calculation : The errors are the differences
between the current 8x8 blocks and the reference 8x8 blocks. Before
fDCT, the errors are extracted by SIMD subtracting reference blocks
from current blocks. After iDCT, the current blocks are recovered by
SIMD adding the errors to the reference blocks so that future frames
can refer to these predicted blocks (in P frames). The 8-byte rows are
unpacked into 8 shorts (16-bit) and the SIMD additions and
subtractions are then applied (transfrm.c).

(3) fDCT/iDCT : The SSE2-enhanced fDCT and iDCT use a butterfly
algorithm based on Intel Application Notes 922 (AP-922). The SSE2
version of the fast iDCT (idct.c) is available on the Internet at
http://www.koders.com/cpp/fid284A1B63C25B61F528B1A110F8920EAEC6BFB69F.aspx
The MMX version of the fast fDCT is also available on the Internet as
described in II. This encoder contains the SSE2-ported version of the
MMX code of the fDCT (fdctref.c).

Both the forward and inverse transforms use a 2-step (row and column)
approach for the 2-dimensional 8x8 fDCT/iDCT.  The row step performs a
dot product of the input and the coefficient matrices. The
multiply-add and SIMD add/subtract instructions are used to
efficiently compute the row fDCT/iDCT.  The column step is less
regular. However, identical operations are performed on each
column. Thus, while scalar instructions perform column fDCT/iDCT
column by column, the SSE2-enhanced version is able to operate on 8
columns simultaneously.

(4) Quantization/Inverse Quantization : Both quantization and inverse
quantization involves clipping and scaling of the 8x8 blocks. Inverse
quantization involves SIMD multiply, SIMD right shift, and SIMD
min/max (iquant_* functions in quantize.c).  Quantization also
contains similar operations; however, it also needs a SIMD integer
division. Unfortunately, SSE2 does not support such a SIMD
operation. Consequently, the row of the 8x8 blocks are converted to
floating-point so that the SIMD floating-point divide can be used
(quant_* functions in quantize.c).

