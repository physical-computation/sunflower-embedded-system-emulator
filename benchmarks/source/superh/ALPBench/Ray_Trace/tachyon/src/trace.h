/* 
 * trace.h - This file contains the declarations for the main tracing calls.
 *
 *   $Id: trace.h,v 1.32 2001/01/19 08:31:39 johns Exp $
 */

typedef struct {
  int tid;
  int nthr;
  scenedef * scene;
  unsigned long * local_mbox;
  unsigned long serialno; 
  int startx;
  int stopx;
  int xinc;
  int starty;
  int stopy;
  int yinc;
  rt_barrier_t * runbar; /* Thread barrier */
} thr_parms;

color trace(ray *);
void * thread_trace(thr_parms *); 

