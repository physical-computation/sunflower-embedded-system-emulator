/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"
#include "mextern.h"

void
propulsionSetPropulsionCoeffs(Engine *  E, State *  S,
	double xk1, double xk2, double xk3, double xk4, double xk5, double xk6,
	double yk1, double yk2, double yk3, double yk4, double yk5, double yk6,
	double zk1, double zk2, double zk3, double zk4, double zk5, double zk6
		)
{
	S->propulsionXvectorCoefficients[0] = xk1;
	S->propulsionXvectorCoefficients[1] = xk2;
	S->propulsionXvectorCoefficients[2] = xk3;
	S->propulsionXvectorCoefficients[3] = xk4;
	S->propulsionXvectorCoefficients[4] = xk5;
	S->propulsionXvectorCoefficients[5] = xk6;

	S->propulsionYvectorCoefficients[0] = yk1;
	S->propulsionYvectorCoefficients[1] = yk2;
	S->propulsionYvectorCoefficients[2] = yk3;
	S->propulsionYvectorCoefficients[3] = yk4;
	S->propulsionYvectorCoefficients[4] = yk5;
	S->propulsionYvectorCoefficients[5] = yk6;

	S->propulsionZvectorCoefficients[0] = zk1;
	S->propulsionZvectorCoefficients[1] = zk2;
	S->propulsionZvectorCoefficients[2] = zk3;
	S->propulsionZvectorCoefficients[3] = zk4;
	S->propulsionZvectorCoefficients[4] = zk5;
	S->propulsionZvectorCoefficients[5] = zk6;

	return;
}
