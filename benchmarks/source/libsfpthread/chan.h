#define RELEASE_0(c)		pthread_mutex_lock(&(c)->mutex0);\
				pthread_cond_broadcast(&(c)->cond0);\
				pthread_mutex_unlock(&(c)->mutex0);

#define	PRE_WAIT_0(c)		pthread_mutex_lock(&(c)->mutex0);

#define	POST_WAIT_0(c)		pthread_cond_wait(&(c)->cond0, &(c)->mutex0);\
				pthread_mutex_unlock(&(c)->mutex0);

#define RELEASE_1(c)		pthread_mutex_lock(&(c)->mutex1);\
				pthread_cond_broadcast(&(c)->cond1);\
				pthread_mutex_unlock(&(c)->mutex1);

#define	PRE_WAIT_1(c)		pthread_mutex_lock(&(c)->mutex1);

#define	POST_WAIT_1(c)		pthread_cond_wait(&(c)->cond1, &(c)->mutex1);\
				pthread_mutex_unlock(&(c)->mutex1);

#define CHAN_INIT(c)		pthread_cond_init(&(c)->cond0, NULL);\
				pthread_cond_init(&(c)->cond1, NULL);\
				pthread_mutex_init(&(c)->mutex0, NULL);\
				pthread_mutex_init(&(c)->mutex1, NULL);

#define CHAN_DESTROY(c)		pthread_mutex_destroy(&(c)->mutex0);\
				pthread_cond_destroy(&(c)->cond0);\
				pthread_mutex_destroy(&(c)->mutex1);\
				pthread_cond_destroy(&(c)->cond1);

#define	POST_NATAL(in)		PRE_WAIT_1(in)\
				RELEASE_0(in)

#define	SEND(out)		PRE_WAIT_0(out)\
				RELEASE_1(out)\
				POST_WAIT_0(out)

#define	RECEIVE(in, use)	POST_WAIT_1(in)\
				use;\
				PRE_WAIT_1(in)\
				RELEASE_0(in)

typedef struct
{
	pthread_mutex_t		mutex0;
	pthread_mutex_t		mutex1;
	pthread_cond_t		cond0;
	pthread_cond_t		cond1;

	ulong			data;
	ulong			aux;
} Chan;
