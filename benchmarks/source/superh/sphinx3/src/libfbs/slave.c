static void Load_phone_states()
{
  

static void emw(s3wid_t w, whmm_t *h_ws, int32 *senscr)
{
        s3pid_t pid, prevpid;
        s3senid_t *senp;
        int32 **tp;
        int32 to, from, bestfrom;
        int32 newscr, scr, bestscr;

        prevpid = BAD_PID;

        for (from = n_state - 2; from >= 0; --from) {
                if ((pid = h->pid[from]) != prevpid) {
                        senp = mdef->phone[pid].state;
                        prevpid = pid;
                }  
        
                if ((st_sen_scr[from] =
                     h->score[from] + senscr[senp[from]]) < LOGPROB_ZERO)
                        st_sen_scr[from] = LOGPROB_ZERO;
        }
        
        /* Evaluate final-state first, which does not have a self-transition */
        to = final_state;
        scr = LOGPROB_ZERO;
        bestfrom = -1;
        prevpid = BAD_PID;
        for (from = to - 1; from >= 0; --from) {
                if ((pid = h->pid[from]) != prevpid) {
                        tp = tmat->tp[mdef->phone[pid].tmat];
                        prevpid = pid;
                }
        
                if ((tp[from][to] > LOGPROB_ZERO) &&
                    ((newscr = st_sen_scr[from] + tp[from][to]) > scr)) {
                        scr = newscr;
                        bestfrom = from;
                }
        }
        h->score[to] = scr;
        if (bestfrom >= 0) {
                h->history[to] = h->history[bestfrom];
                h->pid[to] = h->pid[bestfrom];
        }
                    
        bestscr = scr;
                        
        /* Evaluate all other states, which might have self-transitions */
        for (to = final_state - 1; to >= 0; --to) {
                /* Score from self-transition, if any */
                if ((pid = h->pid[to]) != prevpid) {
                        tp = tmat->tp[mdef->phone[pid].tmat];
                        prevpid = pid;
                }
                scr = (tp[to][to] > LOGPROB_ZERO) ? st_sen_scr[to] + tp[to][to]
                      : LOGPROB_ZERO;
                        
                /* Scores from transitions from other states */
                bestfrom = -1;
                
                for (from = to - 1; from >= 0; --from) {
                        if ((pid = h->pid[from]) != prevpid) { 
                                tp = tmat->tp[mdef->phone[pid].tmat];
                                prevpid = pid;
                        }
                
                        if ((tp[from][to] > LOGPROB_ZERO) &&
                            ((newscr =
                              st_sen_scr[from] + tp[from][to]) > scr)) {
                                scr = newscr;
                                bestfrom = from;
                        }
                }
                                
                /* Update new result for state to */
                h->score[to] = scr;
                if (bestfrom >= 0) {
                        h->history[to] = h->history[bestfrom];
                        h->pid[to] = h->pid[bestfrom];
                }
                                
                if (bestscr < scr)
                        bestscr = scr;
        }                       
                
        h->bestscore = bestscr;
}

