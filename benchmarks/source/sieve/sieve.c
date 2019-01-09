#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define RELEASE_0(c)	pthread_mutex_lock(&(c)->mutex0);\
			pthread_cond_signal(&(c)->cond0);\
			pthread_mutex_unlock(&(c)->mutex0);

#define	PRE_WAIT_0(c)	pthread_mutex_lock(&(c)->mutex0);

#define	POST_WAIT_0(c)	pthread_cond_wait(&(c)->cond0, &(c)->mutex0);\
			pthread_mutex_unlock(&(c)->mutex0);

#define RELEASE_1(c)	pthread_mutex_lock(&(c)->mutex1);\
			pthread_cond_signal(&(c)->cond1);\
			pthread_mutex_unlock(&(c)->mutex1);

#define	PRE_WAIT_1(c)	pthread_mutex_lock(&(c)->mutex1);

#define	POST_WAIT_1(c)	pthread_cond_wait(&(c)->cond1, &(c)->mutex1);\
			pthread_mutex_unlock(&(c)->mutex1);

#define CHAN_INIT(c)	pthread_cond_init(&(c)->cond0, NULL);\
			pthread_cond_init(&(c)->cond1, NULL);\
			pthread_mutex_init(&(c)->mutex0, NULL);\
			pthread_mutex_init(&(c)->mutex1, NULL);

#define CHAN_DESTROY(c)	pthread_mutex_destroy(&(c)->mutex0);\
			pthread_cond_destroy(&(c)->cond0);\
			pthread_mutex_destroy(&(c)->mutex1);\
			pthread_cond_destroy(&(c)->cond1);

typedef struct
{
	pthread_mutex_t	mutex0;
	pthread_mutex_t	mutex1;
	pthread_cond_t	cond0;
	pthread_cond_t	cond1;

	int		data;
} Chan;

enum
{
	SIEVE_MAX_PRIME = 100, //1 << 10,
};

int		maxprime = SIEVE_MAX_PRIME;
static void	sieve(Chan *);
static void	fatal(char *);



int
main(int argc, char *argv[])
{
	Chan		*out;
	pthread_t	nextid;
	int		i, status;

	if (argc == 2)
	{
		char tmp;
		char *ep = &tmp;

		maxprime = strtoul(argv[1], &ep, 0);
		if (*ep != '\0' || maxprime <= 0)
		{
			fatal("Invalid argument / Argument too large");
		}
	}
	printf("\n\nGenerating prime numbers less than %d\n\n", maxprime);


	/*	"out" is used to communicate with next stage	*/
	out = (Chan *)calloc(1, sizeof(Chan));
	if (out == NULL)
	{
		fatal("Could not allocate memory for (Chan *)out in main()");
	}
	CHAN_INIT(out)
	

	/*	Create first thread in the pipeline & wait	*/
	PRE_WAIT_0(out)
	status = pthread_create(&nextid, NULL, (void *)sieve, (void *)out);
	if (status < 0)
	{
		fatal("Could not create new thread\n");
	}
	POST_WAIT_0(out)

	/*							*/
	/*	NOTE: This stops filling pipeline at maxprime,	*/
	/*	so in practice the maximum prime printed out	*/
	/*	will be less than maxprime.			*/
	/*							*/
	for (i = 2; i < maxprime; i++)
	{
		/*	Set data, release waiting next stage	*/
		out->data = i;

		PRE_WAIT_0(out)
		RELEASE_1(out)

		/*	Wait for next stage to consume data	*/
		POST_WAIT_0(out)
	}
	
	return 0;
}

void
sieve(Chan *in)
{
	Chan		*out;
	pthread_t	nextid;
	int		incoming, myprime, status;


	/*	Release previous stage waiting on our birth:	*/
	PRE_WAIT_1(in)
	RELEASE_0(in)

	/*	"out" is used to communicate with next stage	*/
	out = (Chan *)calloc(1, sizeof(Chan));
	if (out == NULL)
	{
		fatal("Could not allocate memory for (Chan *)out in sieve()");
	}
	CHAN_INIT(out)


	/*	Wait for incoming data to be set		*/
	POST_WAIT_1(in)

	myprime = incoming = in->data;
	printf("myprime = %d\n", myprime);

	/*	Release previous waiting stage			*/
	PRE_WAIT_1(in)
	RELEASE_0(in)

	/*		Create next stage in pipeline:		*/
	PRE_WAIT_0(out)
	status = pthread_create(&nextid, NULL, (void *)sieve, (void *)out);
	if (status < 0)
	{
		fatal("Could not create new thread\n");
	}

	/*	       Wait until next stage is created:	*/
	POST_WAIT_0(out)


	do
	{
		/*	Wait for incoming data to be set	*/
		POST_WAIT_1(in)

		incoming = in->data;

		/*	Release previous waiting stage		*/
		PRE_WAIT_1(in)
		RELEASE_0(in)


		/*	If not a m'ple of myprime, pass on:	*/
		if (incoming % myprime)
		{
			/*	Set data, release waiting next stage	*/
			out->data = incoming;
			PRE_WAIT_0(out)
			RELEASE_1(out)

			/*	Wait for next stage to consume data	*/
			POST_WAIT_0(out)
		}
	} while (incoming < maxprime);

	CHAN_DESTROY(in)
	CHAN_DESTROY(out)


	return;
}

void
fatal(char *error)
{
	printf("Sieve Error: \"%s\"\n", error);

	exit(-1);
}
