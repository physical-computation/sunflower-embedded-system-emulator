Authors
=======
Alex Li (manlapli@uiuc.edu)
Ruchira Sasanka (sasanka@uiuc.edu)


Modifications of CSU Face Recognition System
============================================

The current recognizer is a subset of the CSU face recognition algorithm 
evaluation system. The original version can be found at 
http://www.cs.colostate.edu/evalfacerec/. Details of the base version can be 
found in README and faceIdUsersGuide.pdf. This document decribes the 
modifications made to the base version.

This version focuses on the Eigen face PCA algorithm. To compile the necessary
binaries, do

1) make preprocess
2) make subspace
3) make -f makefile_sp

In the main directory, run './scripts/runPCA_PP_Train_scraps.sh' and the scrap
images included in this package are preprocessed and trained. One can also 
download the FERET database to test on larger number of images.

To compute distances, run './scripts/PCAproject.sh' which the 2nd pass compute
distances in MahCosine between the probe image with the given image list.

The modifications made to the Face Recognition application are

I. Parallelization of subspace projection and distance calculation

II. SIMDification of subspace projection and distance calculation

I. Parallelization
-------------------

POSIX threads are used to parallelize the application in 3 different
occasions

1) projecting the gallery images onto the trained subspace,
2) projecting the probe image onto the subspace, and
3) distance calculation of the probe iamge against gallery images.

The first event can be done once offline before the application is
run. Images are divided evenly among threads to project onto the
subspace. Threads are created in readAndProjectImages function in
csuCommonSubspace.c.

The second event occurs when a probe image is given and to be aligned
to the trained subspace. Threads divide up the large subspace during
projection of the probe image, which is essentially a matrix
multiply. The threading is initiated in thrd_centerthenproject
function in threading.c.

The third event occurs when the distances between the probe image and
the gallery images are to be calculated. Threads evenly divide up the
gallery images and perform distance computations. In the current
version, only MahCosine is implemented since it gives good accuracy
and performance trade-off among the other algorithms. The thread
creation is done in computeOneDistances function in
csuSubspaceProject.c.

II. SIMDification
-----------------

Subspace normalization, projection, and distance calculations are
enhanced with Intel SSE2 instructions. The SSE2 instructions are
embedded as inline assembly code using MSASM format.

Note: In order to execute the SSE2 version, please use Intel ICC
compiler. The non-commercial version of ICC can be obtained from
http://www.intel.com/cd/software/products/asmo-na/eng/compilers/219771.htm

(1) Subspace Normalization : Normalization is carried out in
thread_mean_subtract_images in thread.c by subtracting the subspace
mean vector from the image vector. Double-precision floating-point
SSE2 instructions are used.

(2) Subspace Projection : Projection is done in
thrd_transposeMultiplyMatrixL in threading.c. Double-precision
floating-point SSE2 multiply and add instructions are used.

(3) MahCosine Distance Calculation : distanceMahCosine in
csuSubspaceProject.c shows how the Mahalanobis angle is calculated
with SSE2 Double-precision floating-point multiply and add
instructions.



