/*
 * 
 * This file is part of the ALPBench Benchmark Suite Version 1.0
 * 
 * Copyright (c) 2005 The Board of Trustees of the University of Illinois
 * 
 * All rights reserved.
 * 
 * ALPBench is a derivative of several codes, and restricted by licenses
 * for those codes, as indicated in the source files and the ALPBench
 * license at http://www.cs.uiuc.edu/alp/alpbench/alpbench-license.html
 * 
 * The multithreading and SSE2 modifications for SpeechRec, FaceRec,
 * MPEGenc, and MPEGdec were done by Man-Lap (Alex) Li and Ruchira
 * Sasanka as part of the ALP research project at the University of
 * Illinois at Urbana-Champaign (http://www.cs.uiuc.edu/alp/), directed
 * by Prof. Sarita V. Adve, Dr. Yen-Kuang Chen, and Dr. Eric Debes.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal with the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimers.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimers in the documentation and/or other materials provided
 *       with the distribution.
 * 
 *     * Neither the names of Professor Sarita Adve's research group, the
 *       University of Illinois at Urbana-Champaign, nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this Software without specific prior written permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
 * SOFTWARE.
 * 
 */


#ifdef RSIM

#define VEC_STRIDE_SET   0
#define VEC_CLEAR_BLK    1
#define VEC_SATURATE     2
#define VEC_IDCT         5

#define VEC_FLUSH           14

#define VEC_FORM_COMP_PRED  15
#define VEC_FORM_COMP_PRED2 30
#define VEC_FORM_COMP_PRED3 45

#define SIMD_SATURATE          60
#define SIMD_IDCT              64
#define SIMD_FORM_COMP_PRED    70

#define SIMD_ADD_BLK  80
#define VEC_ADD_BLK   90
#define SIMD_CLEAR_BLK 85

#define BEGIN_VEC1Arg(x)           BeginVec1Arg(x);
#define BEGIN_VEC2Arg(x,y)         BeginVec2Arg(x,y);
#define BEGIN_VEC3Arg(x,y,z)       BeginVec3Arg(x,y,z);
#define BEGIN_VEC4Arg(x,y,z,p)     BeginVec4Arg(x,y,z,p);
#define BEGIN_VEC5Arg(x,y,z,p,q)   BeginVec5Arg(x,y,z,p,q);
#define BEGIN_VEC6Arg(x,y,z,p,q,r) BeginVec5Arg(x,y,z,p,q,r);
#define END_VECNoRet()             EndVecNoRet();
#define END_VEC()                  EndVec();

#else /* NotRSIM */

#define BEGIN_VEC1Arg(x)           0
#define BEGIN_VEC2Arg(x,y)         0
#define BEGIN_VEC3Arg(x,y,z)       0
#define BEGIN_VEC4Arg(x,y,z,p)     0
#define BEGIN_VEC5Arg(x,y,z,p,q)   0
#define BEGIN_VEC6Arg(x,y,z,p,q,r) 0
#define END_VECNoRet()             (0)
#define END_VEC()                  (0)

#endif
