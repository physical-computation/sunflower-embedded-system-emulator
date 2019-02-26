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

#include <stdio.h>
#include <assert.h>
#include "sf.h"

void
mbitprint(Engine *E, State *S, short nbits, ulong num)
{
		assert(nbits >= 32);
        int	i = 0;

        for (i = (nbits-1); i >= 0; i--)
        {
		mprint(E, S, nodeinfo, "%c", ((num >> i)&1) + '0');

		if (!(i%4))
		{
			mprint(E, S, nodeinfo, "%c", ' ');
		}
        }

        return;
}

int
bit_flips_32(ulong w1, ulong w2)
{
	ulong tmp;

	tmp = w1^w2;
	tmp = (tmp&0x55555555) + ((tmp&0xaaaaaaaa) >> 1);
	tmp = (tmp&0x33333333) + ((tmp&0xcccccccc) >> 2);
	tmp = (tmp&0x0f0f0f0f) + ((tmp&0xf0f0f0f0) >> 4);
	tmp = (tmp&0x00ff00ff) + ((tmp&0xff00ff00) >> 8);
	tmp = (tmp&0x0000ffff) + ((tmp&0xffff0000) >> 16);

	return (int)tmp;
}

int
bit_flips_n(int n, ulong w1, ulong w2)
{
	int i, ret_val = 0;

	/*  This routine is _much_ slower than above, but more general	*/
	for (i = 0; i < n; i++)
	{
		ret_val += ((w1>>i) & 1)^((w2>>i) & 1);
	}

	return ret_val;
}
