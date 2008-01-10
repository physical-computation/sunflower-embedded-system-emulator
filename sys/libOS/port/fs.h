typedef struct Leaf
{
	unsigned char	name[64];		/* File name. */
	struct Leaf	*child;
	struct Leaf	*sibling;
	struct Leaf	*parent;
	void 		(*server)();		/* Fxn that handles this node */
} Leaf;
