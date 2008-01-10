typedef struct
{
	int 	locked;
	ulong	pc;
} Lock;

void	lock(Lock *);
void	unlock(Lock *);
