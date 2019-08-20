typedef struct
{
	uint64_t		taintCol;
	int			memType; 

	/*
	*	memType: 0 for memory, 1 for register, 2 for fregister
	*/
} ShadowMem;


struct TaintOriginNode
{
	uint64_t		taintAddress;
	uint32_t		taintPC;
	uint64_t		taintCol;
	int			memType;
	/*
	*	memType: 0 for memory, 1 for register, 2 for fregister
	*/
	struct TaintOriginNode	* next;
};

typedef struct TaintOriginNode TaintOriginNode;
