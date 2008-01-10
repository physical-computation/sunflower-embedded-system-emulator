/*
 * definitions for parsing NFF model files
 * 
 *  
 */

#define NFFNOERR     0
#define NFFBADFILE   1
#define NFFBADSYNTAX 2
#define NFFEOF       3

unsigned int ParseNFF(char *nffname, SceneHandle scene);


