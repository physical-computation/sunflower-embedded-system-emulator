/*								*/
/*	Variable signature and home node ID.  Home node ID 	*/
/*	of -1 means wherever it is created.  A signature	*/
/*	that's a substring of a longer signature matches it.	*/
/*								*/

#if defined	SPTHR_STATICMAP_ALLON0
StaticMap	spthr_staticmaptable[] =
{
	/*	These are library internal globals:	*/
	{"StaticMap spthr_staticmaptable[]",	0},
	{"Maddr spthr_regstackptrs[]",		0},
	{"struct spthr_mallocmutex",		0},
	{"struct spthr_mapcounts",		0},
	{"ThreadState *spthr_threadqhd",	0},
	{"ThreadState *spthr_threadqtl",	0},
	{"RegionMap *spthr_maplisthd",		0},
	{"RegionMap *spthr_maplisttl",		0},
	{"int spthr_nthreads",			0},
	{"int spthr_nummaps",			0},
	{"int spthr_windowsize",		0},
	{"char spthr_initdone",			0},
	{"Maddr spthr_loweststack",		0},
	{"LibMode spthr_mode",			0},

	/*	Many instances of these are created:	*/
	{"pthread_cond_t *cond",		0},
	{"pthread_mutex_t *mutex",		0},
	{"Sleepq *s",				0},

	/*	One instance per thread created:	*/
	{"ThreadState *ThreadState",		0},

	/*	One instance, parent thread only:	*/
	{"ThreadState *init0",			0},

	/*	Application specific			*/
	{"uchar plaintext",			0},
	{"uchar ciphertext",			0},

	{0, SPTHR_MAP_END},
};

#elif defined	SPTHR_STATICMAP_LOCALTHREADSTATE
StaticMap	spthr_staticmaptable[] =
{
	/*	These are library internal globals:	*/
	{"StaticMap spthr_staticmaptable[]",	0},
	{"Maddr spthr_regstackptrs[]",		0},
	{"struct spthr_mallocmutex",		0},
	{"struct spthr_mapcounts",		0},
	{"ThreadState *spthr_threadqhd",	0},
	{"ThreadState *spthr_threadqtl",	0},
	{"RegionMap *spthr_maplisthd",		0},
	{"RegionMap *spthr_maplisttl",		0},
	{"int spthr_nthreads",			0},
	{"int spthr_nummaps",			0},
	{"int spthr_windowsize",		0},
	{"char spthr_initdone",			0},
	{"Maddr spthr_loweststack",		0},
	{"LibMode spthr_mode",			0},

	/*	Many instances of these are created:	*/
	{"pthread_cond_t *cond",		0},
	{"pthread_mutex_t *mutex",		0},
	{"Sleepq *s",				0},

	/*	One instance per thread created:	*/
	{"ThreadState *ThreadState",		SPTHR_MAP_LOCAL},

	/*	One instance, parent thread only:	*/
	{"ThreadState *init0",			SPTHR_MAP_LOCAL},

	/*	Application specific			*/
	{"uchar plaintext",			0},
	{"uchar ciphertext",			0},

	{0, SPTHR_MAP_END},
};

#elif defined	SPTHR_STATICMAP_CREATORLOCAL
StaticMap	spthr_staticmaptable[] =
{
	/*	These are library internal globals:	*/
	{"StaticMap spthr_staticmaptable[]",	SPTHR_MAP_LOCAL},
	{"Maddr spthr_regstackptrs[]",		SPTHR_MAP_LOCAL},
	{"struct spthr_mallocmutex",		SPTHR_MAP_LOCAL},
	{"struct spthr_mapcounts",		SPTHR_MAP_LOCAL},
	{"ThreadState *spthr_threadqhd",	SPTHR_MAP_LOCAL},
	{"ThreadState *spthr_threadqtl",	SPTHR_MAP_LOCAL},
	{"RegionMap *spthr_maplisthd",		SPTHR_MAP_LOCAL},
	{"RegionMap *spthr_maplisttl",		SPTHR_MAP_LOCAL},
	{"int spthr_nthreads",			SPTHR_MAP_LOCAL},
	{"int spthr_nummaps",			SPTHR_MAP_LOCAL},
	{"int spthr_windowsize",		SPTHR_MAP_LOCAL},
	{"char spthr_initdone",			SPTHR_MAP_LOCAL},
	{"Maddr spthr_loweststack",		SPTHR_MAP_LOCAL},
	{"LibMode spthr_mode",			SPTHR_MAP_LOCAL},

	/*	Many instances of these are created:	*/
	{"pthread_cond_t *cond",		SPTHR_MAP_LOCAL},
	{"pthread_mutex_t *mutex",		SPTHR_MAP_LOCAL},
	{"Sleepq *s",				SPTHR_MAP_LOCAL},

	/*	One instance per thread created:	*/
	{"ThreadState *ThreadState",		SPTHR_MAP_LOCAL},

	/*	One instance, parent thread only:	*/
	{"ThreadState *init0",			SPTHR_MAP_LOCAL},

	/*	Application specific			*/
	{"uchar plaintext",			SPTHR_MAP_LOCAL},
	{"uchar ciphertext",			SPTHR_MAP_LOCAL},

	{0, SPTHR_MAP_END},
};

#else
StaticMap	spthr_staticmaptable[] =
{
	{0, SPTHR_MAP_END},
};

#endif
