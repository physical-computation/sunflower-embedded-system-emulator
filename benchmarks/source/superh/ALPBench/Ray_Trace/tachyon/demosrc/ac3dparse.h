/*
 * definitions for parsing ac3d modeler files
 *  
 */

#define PARSENOERR       0
#define PARSEBADFILE     1
#define PARSEBADSUBFILE  2
#define PARSEBADSYNTAX   4
#define PARSEEOF         8
#define PARSEALLOCERR    16

unsigned int ParseAC3D(char *, SceneHandle);

#ifdef PARSE_INTERNAL
#define NUMTEXS 8192
#define TEXNAMELEN 24

typedef float RotMat[3][3]; 
typedef float TransMat[3];

typedef struct {
  char name[TEXNAMELEN];
  void * tex;
} texentry;

typedef struct tri_list_t {
  int v0;     /* index of vertex 0 */
  int v1;     /* index of vertex 1 */
  int v2;     /* index of vertex 2 */
  int smooth; /* smoothed or flat shaded flag */
  int texnum; /* index of texture to use */
  struct tri_list_t * next; /* pointer to next triangle */
} tri_list;

typedef unsigned int errcode;
static errcode add_texture(void * tex, char name[TEXNAMELEN]);

static void tlist_add_tri(tri_list ** tlist, int v0, int v1, int v2,
                          int smooth, int texnum);
static void tlist_delete(tri_list ** tlist);
static void clear_normals(apivector * normals, int numverts);
static void normalize(apivector * vec);
static void renormalize_normals(apivector * normals, int numverts);
static apivector tri_normal(apivector * v0, apivector * v1, apivector * v2);
static void gen_triangles(SceneHandle, tri_list * tlist, apivector * vertex,
                          apivector * normal);
static errcode GetMaterial(FILE *, SceneHandle);
static errcode GetString(FILE *, char *);
static errcode GetScenedefs(FILE *, SceneHandle);
static errcode GetScene(FILE *, SceneHandle);
static errcode GetObject(FILE *, SceneHandle);
#endif

