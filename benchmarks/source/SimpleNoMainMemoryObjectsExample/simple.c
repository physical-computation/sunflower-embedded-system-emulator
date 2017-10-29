int gUninitializedInt;
int gInitializedInt = 42;

int
noMain(void)
{
	int	i, localInt;

	for (i = 0; i < 10; i++)
	{
		localInt += gInitializedInt + gUninitializedInt;
	}

	return 0;
}
