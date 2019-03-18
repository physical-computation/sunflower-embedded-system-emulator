.globl gcovunf
.globl best_guessunf
.globl svarunf
.globl cvarunf

.globl sqrtunf
.globl atan2unf

gcovunf:
	ungcov.s fa0, fa0, fa1;
	ret;

best_guessunf:
	ret;

svarunf:
	unsvar.s fa0, fa1;
	ret;

cvarunf:
	uncvar.s fa0;
	ret;

sqrtunf:
	unfsqrt.s fa0, fa0;
	ret;


