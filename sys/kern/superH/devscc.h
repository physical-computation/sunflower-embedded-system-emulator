enum
{
	TX_RDY = (1<<7),
	TX_CLR = (1<<7)^0xFFFF,
	RX_RDY = (1<<6),
	RX_CLR = (1<<6)^0xFFFF,
};

void	devsccinit(void);
void	kputc(int, uchar);
uchar	kgetc(int);
int	kprobec(int);
void	sccputc(int, uchar);
uchar	sccgetc(int);
