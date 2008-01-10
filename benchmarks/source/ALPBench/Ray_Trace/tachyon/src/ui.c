/*
 * ui.c - Contains functions for dealing with user interfaces
 *
 *  $Id: ui.c,v 1.7 1998/07/26 00:23:28 johns Exp $
 */

#include "machine.h"
#include "types.h"
#include "macros.h"
#include "util.h"
#include "ui.h"

static void (* rt_static_ui_message) (int, char *) = NULL;
static void (* rt_static_ui_progress) (int) = NULL;
static int (* rt_static_ui_checkaction) (void) = NULL;

void rt_set_ui_message(void (* func) (int, char *)) {
  rt_static_ui_message = func;
}

void rt_set_ui_progress(void (* func) (int)) {
  rt_static_ui_progress = func;
}

void rt_ui_message(int level, char * msg) {
  if (rt_static_ui_message != NULL) 
    rt_static_ui_message(level, msg);
}

void rt_ui_progress(int percent) {
  if (rt_static_ui_progress != NULL)
    rt_static_ui_progress(percent);
}

int rt_ui_checkaction(void) {
  if (rt_static_ui_checkaction != NULL) 
    return rt_static_ui_checkaction();
  else
    return 0;
}














