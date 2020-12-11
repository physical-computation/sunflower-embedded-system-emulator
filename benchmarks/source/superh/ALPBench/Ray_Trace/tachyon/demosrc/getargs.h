

typedef struct {
  char **filenames;       /* list of model files to render */
  int numfiles;           /* number of files to render */
  int useoutfilename;     /* command line override of output filename */
  char outfilename[FILENAME_MAX]; /* name of output image file */
  int outimageformat;     /* format of output image */
  int verbosemode;        /* verbose flags */
  int aa_maxsamples;      /* antialiasing setting */
  int boundmode;          /* bounding mode */
  int boundthresh;        /* bounding threshold */
  int usecamfile;         /* use camera file */
  char camfilename[FILENAME_MAX]; /* camera filename */
  int shadermode;         /* quality level */
  int phongfunc;          /* shader for specular highlights */
  int transmode;          /* transparency rendering mode */
  int numthreads;         /* explicit number of threads to use */
  int nosave;             /* don't write output image to disk */
  char spaceball[FILENAME_MAX];   /* spaceball serial port device */
} argoptions;

int getargs(int argc, char **argv, argoptions * opt, int node);
int useoptions(argoptions * opt, SceneHandle scene);
void freeoptions(argoptions * opt);
