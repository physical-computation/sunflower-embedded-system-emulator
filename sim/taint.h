typedef struct
{
	uint64_t		taintCol;
	int			memType; //0 for memory, 1 for register
} ShadowMem;


struct TaintOriginNode
{
	uint64_t		taintAddress;
	uint32_t		taintPC;
	uint64_t		taintCol;
	int			memType; //0 for memory, 1 for register
	struct TaintOriginNode	* next;
};

typedef struct TaintOriginNode TaintOriginNode;
