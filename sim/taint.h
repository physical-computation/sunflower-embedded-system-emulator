/*
	Copyright (c) 2019, Hein Alexander Mante (author)

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

typedef enum
{
	kSunflowerTaintMemTypeMemory,
	kSunflowerTaintMemTypeRegister,
	kSunflowerTaintMemTypefltRegister,
	kSunflowerTaintMemTypeInstruction,

	/*
	*	This means that: 0 = memory,
	*	1 = register, 2 = fregister,
	*	3 = instruction
	*/
} SunflowerTaintMemType;

typedef struct
{
	uint64_t		taintCol;
	SunflowerTaintMemType	memType; 
} ShadowMem;


struct TaintOriginNode
{
	uint64_t		taintAddress;
	uint32_t		taintstartPC;
	uint32_t		taintendPC;
	uint64_t		taintCol;
	SunflowerTaintMemType	memType;
	struct TaintOriginNode	* next;
};

typedef struct TaintOriginNode TaintOriginNode;

