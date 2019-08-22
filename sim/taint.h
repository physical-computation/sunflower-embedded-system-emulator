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
	uint32_t		taintPC;
	uint64_t		taintCol;
	SunflowerTaintMemType	memType;
	struct TaintOriginNode	* next;
};

typedef struct TaintOriginNode TaintOriginNode;

