/*
 * definitions for parsing MGF model files
 * 
 *  
 */

#define MGF_NOERR     0
#define MGF_BADFILE   1
#define MGF_BADSYNTAX 2
#define MGF_EOF       3

unsigned int ParseMGF(char *mgfname, SceneHandle scene, int defaultflag);

