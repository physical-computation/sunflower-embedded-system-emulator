typedef struct
{
	uint64_t		taintCol;
	SunflowerTaintMemType	memType; 
} ShadowMem;


typedef struct
{
	uint64_t		taintAddress;
	uint32_t		taintPC;
	uint64_t		taintCol;
	SunflowerTaintMemType	memType;
	struct TaintOriginNode	* next;
} TaintOriginNode;

enum
{
	kSunflowerTaintMemTypeMemory
	kSunflowerTaintMemTypeRegister
} SunflowerTaintType

