/*
 * dag.c -- DAG utility routines.
 *
 * **********************************************
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1997 Carnegie Mellon University.
 * ALL RIGHTS RESERVED.
 * **********************************************
 * 
 * HISTORY
 * 
 * 07-Feb-1997	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Started.
 */


#include <libutil/libutil.h>
#include "dag.h"


void dag_reachable_bwd (dagnode_t *d)
{
    daglink_t *l;
    
    d->reachable = 1;
    for (l = d->predlist; l; l = l->next)
	if (! l->dst->reachable)
	    dag_reachable_bwd (l->dst);
}


void dag_reachable_fwd (dagnode_t *d)
{
    daglink_t *l;
    
    d->reachable = 2;
    for (l = d->succlist; l; l = l->next)
	if (l->dst->reachable == 1)
	    dag_reachable_fwd (l->dst);
}


/* Link all nodes ending at same time as exit node to new dummy exit node */
void dag_append_sentinel (dag_t *dag, s3wid_t wid)
{
    dagnode_t *final, *d;
    int32 f;
    
    final = (dagnode_t *) listelem_alloc (sizeof(dagnode_t));
    
    final->wid = wid;
    final->seqid = dag->nnode;
    final->reachable = 0;
    final->sf = dag->nfrm;
    final->fef = dag->nfrm;
    final->lef = dag->nfrm;
    final->succlist = NULL;
    final->predlist = NULL;
    final->next = NULL;

    assert (dag->node_sf[dag->nfrm] == NULL);
    dag->node_sf[dag->nfrm] = final;
    dag->nnode++;
    dag->nfrm++;
    
    for (f = 0; f < dag->nfrm-1; f++) {
	for (d = dag->node_sf[f]; d; d = d->next) {
	    if (d->lef == dag->exit.dst->lef) {
		dag_link (d, final);
		dag->nlink++;
	    }
	}
    }
    
    dag->exit.dst = final;
}


void dag_link (dagnode_t *src, dagnode_t *dst)
{
    daglink_t *l;
    
    /* Link dst into successor list for src */
    l = (daglink_t *) listelem_alloc (sizeof(daglink_t));
    l->dst = dst;
    l->src = src;
    l->next = src->succlist;
    src->succlist = l;

    /* Link src into predecessor list for dst */
    l = (daglink_t *) listelem_alloc (sizeof(daglink_t));
    l->dst = src;
    l->src = dst;
    l->next = dst->predlist;
    dst->predlist = l;
}


void dag_destroy (dag_t *dag)
{
    dagnode_t *d, *nd;
    daglink_t *l, *nl;
    int32 f;
    
    for (f = 0; f < dag->nfrm; f++) {
	for (d = dag->node_sf[f]; d; d = nd) {
	    nd = d->next;
	    
	    for (l = d->succlist; l; l = nl) {
		nl = l->next;
		listelem_free ((char *)l, sizeof(daglink_t));
	    }
	    for (l = d->predlist; l; l = nl) {
		nl = l->next;
		listelem_free ((char *)l, sizeof(daglink_t));
	    }
	    
	    listelem_free ((char *)d, sizeof(dagnode_t));
	}
    }
    
    ckd_free (dag->node_sf);
    ckd_free (dag);
}
