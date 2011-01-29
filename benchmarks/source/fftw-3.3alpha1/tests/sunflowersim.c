/*          8 GPRs + PR         */
unsigned char	REGSAVESTACK[36];


//BUG: for some reason, someone, somewhere, is using these symbols. need to track it down.
void _times(void)
{
}

void _gettimeofday(void)
{
}

void
startup(int argc, char *argv[])
{
	/*									*/
	/*	The main() for the fftw test/bench program is actually in	*/
	/*	libbench2/bench-main.c, and is called via aligned_main()	*/
	/*									*/
	return aligned_main(argc, argv);
}

/*
	If linking in libsfpthread, this is defined there
*/
/*
void
intr_hdlr(void)
{
        return;
}
*/
