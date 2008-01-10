
#include "sball.h"

typedef struct {
  SBallHandle sball;

  apivector camcent;
  apivector camviewvec;
  apivector camupvec;

  apivector orig_camcent;
  apivector orig_camviewvec;
  apivector orig_camupvec;
  apivector orig_camrightvec;

  float curtrans[3];
  float newtrans[3];
  float curquat[4];
  float lastquat[4];
} sbHandle;

void * tachyon_init_spaceball(SceneHandle scene, char * port);
int tachyon_spaceball_update(sbHandle * bh, SceneHandle scene);


