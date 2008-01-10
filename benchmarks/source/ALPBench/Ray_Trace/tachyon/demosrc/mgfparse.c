
#ifdef USELIBMGF

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "tachyon.h"  /* Ray Tracer API */
#include "parser.h"   /* MGF parser library */
#include "mgfparse.h" /* self proto */

static void * globtex = NULL;
static SceneHandle globscene = NULL;

int put_material(void);
int mgf2rgb(C_COLOR *cin, double intensity, float cout[3]);

char tabs[] = " ";

#define curmatname      (c_cmname == NULL ? "mat" : c_cmname)

/*
 * Convert MGF color to RGB representation defined below.
 */
			/* Change the following to suit your standard */
#define  CIE_x_r		0.640		/* nominal CRT primaries */
#define  CIE_y_r		0.330
#define  CIE_x_g		0.290
#define  CIE_y_g		0.600
#define  CIE_x_b		0.150
#define  CIE_y_b		0.060
#define  CIE_x_w		0.3333		/* use true white */
#define  CIE_y_w		0.3333

#define CIE_C_rD	( (1./CIE_y_w) * \
				( CIE_x_w*(CIE_y_g - CIE_y_b) - \
				  CIE_y_w*(CIE_x_g - CIE_x_b) + \
				  CIE_x_g*CIE_y_b - CIE_x_b*CIE_y_g	) )
#define CIE_C_gD	( (1./CIE_y_w) * \
				( CIE_x_w*(CIE_y_b - CIE_y_r) - \
				  CIE_y_w*(CIE_x_b - CIE_x_r) - \
				  CIE_x_r*CIE_y_b + CIE_x_b*CIE_y_r	) )
#define CIE_C_bD	( (1./CIE_y_w) * \
				( CIE_x_w*(CIE_y_r - CIE_y_g) - \
				  CIE_y_w*(CIE_x_r - CIE_x_g) + \
				  CIE_x_r*CIE_y_g - CIE_x_g*CIE_y_r	) )


static float xyz2rgbmat[3][3] = {	/* XYZ to RGB conversion matrix */
	{(CIE_y_g - CIE_y_b - CIE_x_b*CIE_y_g + CIE_y_b*CIE_x_g)/CIE_C_rD,
	 (CIE_x_b - CIE_x_g - CIE_x_b*CIE_y_g + CIE_x_g*CIE_y_b)/CIE_C_rD,
	 (CIE_x_g*CIE_y_b - CIE_x_b*CIE_y_g)/CIE_C_rD},
	{(CIE_y_b - CIE_y_r - CIE_y_b*CIE_x_r + CIE_y_r*CIE_x_b)/CIE_C_gD,
	 (CIE_x_r - CIE_x_b - CIE_x_r*CIE_y_b + CIE_x_b*CIE_y_r)/CIE_C_gD,
	 (CIE_x_b*CIE_y_r - CIE_x_r*CIE_y_b)/CIE_C_gD},
	{(CIE_y_r - CIE_y_g - CIE_y_r*CIE_x_g + CIE_y_g*CIE_x_r)/CIE_C_bD,
	 (CIE_x_g - CIE_x_r - CIE_x_g*CIE_y_r + CIE_x_r*CIE_y_g)/CIE_C_bD,
	 (CIE_x_r*CIE_y_g - CIE_x_g*CIE_y_r)/CIE_C_bD}
};


/* mgf2rgb() - convert MGF color to RGB       */
/* cin       - input MGF chrominance          */
/* intensity - input luminance or reflectance */
/* cout      - output RGB color               */
int mgf2rgb(C_COLOR *cin, double intensity, float cout[3]) {
	static double	cie[3]; /* get CIE XYZ representation */
	c_ccvt(cin, C_CSXY);
	cie[0] = intensity*cin->cx/cin->cy;
	cie[1] = intensity;
	cie[2] = intensity*(1./cin->cy - 1.) - cie[0];
					/* convert to RGB */
	cout[0] = xyz2rgbmat[0][0]*cie[0] + xyz2rgbmat[0][1]*cie[1]
			+ xyz2rgbmat[0][2]*cie[2];
	if(cout[0] < 0.) cout[0] = 0.;
	cout[1] = xyz2rgbmat[1][0]*cie[0] + xyz2rgbmat[1][1]*cie[1]
			+ xyz2rgbmat[1][2]*cie[2];
	if(cout[1] < 0.) cout[1] = 0.;
	cout[2] = xyz2rgbmat[2][0]*cie[0] + xyz2rgbmat[2][1]*cie[1]
			+ xyz2rgbmat[2][2]*cie[2];
	if(cout[2] < 0.) cout[2] = 0.;

  return MG_OK;
}

int put_material(void) {
  float   rgbval[3];
  apitexture tex;  
#if 0
  char    *mname = curmatname;

  if (!c_cmaterial->clock) {      /* current, just use it */
    printf("USE %s\n", mname);
  }
  else {
    printf("%sDEF %s Group {\n", tabs, mname);
  }
  printf("%sMaterial {\n", tabs);
#endif

  memset(&tex, 0, sizeof(tex));

  mgf2rgb(&c_cmaterial->rd_c, 1.0, rgbval);
  tex.col = rt_color(rgbval[0], rgbval[1], rgbval[2]);
  tex.ambient = 0.1;
  tex.diffuse = c_cmaterial->rd;
  tex.opacity = 1.0 - (c_cmaterial->td + c_cmaterial->ts);
  tex.texturefunc = 0;

  if (c_cmaterial->ed > FTINY) {
    mgf2rgb(&c_cmaterial->ed_c, 1.0, rgbval);
    tex.col = rt_color(rgbval[0], rgbval[1], rgbval[2]);
    tex.ambient = c_cmaterial->ed;

    if (tex.ambient > 1.0) 
      tex.ambient = 1.0;
    if (tex.ambient < 0.1)
      tex.ambient = 0.1; 
  }

  globtex = rt_texture(globscene, &tex); /* XXX memory leak city, */
                              /* we aren't keeping track of these... */

  if (c_cmaterial->rs > 0.0) {
    tex.specular = c_cmaterial->rs - (c_cmaterial->rs_a / 2.0);
    if (tex.specular < 0.0) tex.specular = 0.0;

    if (c_cmaterial->rs_a > 0.09) {
      flt pexp, pval ;
      pval = c_cmaterial->rs * 4;
      pval = 0.8;  /* XXX hack */
      pexp = 12.0 / c_cmaterial->rs_a;
      rt_tex_phong(globtex, pval, pexp, RT_PHONG_PLASTIC);
    } 
  }

  c_cmaterial->clock = 0;

  return MG_OK;
}
  

static int myfaceh(int ac, char **av) {  /* face handling routine */
  static char lastmat[256];
  C_VERTEX *vp;    /* vertex structure pointer */
  FVECT vert;      /* vertex point location */
  FVECT normal;      /* vertex point location */
  int i;
  apivector v0, v1, v2;
  apivector n0, n1, n2;
  if (ac < 4)                     /* check # arguments */
    return(MG_EARGC);

  if (strcmp(lastmat, curmatname) || c_cmaterial->clock) {
    put_material();
    strcpy(lastmat, curmatname);
  }

  if ((vp = c_getvert(av[1])) == NULL)    /* vertex from name */
      return(MG_EUNDEF);
  xf_xfmpoint(vert, vp->p);               /* transform vertex */
  v0 = rt_vector(vert[0], vert[1], vert[2]);
  xf_rotvect(normal, vp->n);              /* transform normal */
  n0 = rt_vector(normal[0], normal[1], normal[2]);

  if ((vp = c_getvert(av[2])) == NULL)    /* vertex from name */
      return(MG_EUNDEF);
  xf_xfmpoint(vert, vp->p);               /* transform vertex */
  v1 = rt_vector(vert[0], vert[1], vert[2]);
  xf_rotvect(normal, vp->n);              /* transform normal */
  n1 = rt_vector(normal[0], normal[1], normal[2]);

  for (i = 3; i < ac; i++) {
    if ((vp = c_getvert(av[i])) == NULL)   /* vertex from name */
      return(MG_EUNDEF);
    xf_xfmpoint(vert, vp->p);              /* transform vertex */
    v2 = rt_vector(vert[0], vert[1], vert[2]);
    xf_rotvect(normal, vp->n);             /* transform normal */
    n2 = rt_vector(normal[0], normal[1], normal[2]);

    if (((n0.x*n0.x + n0.y*n0.y + n0.z*n0.z) < 0.9) ||
        ((n1.x*n1.x + n1.y*n1.y + n1.z*n1.z) < 0.9) ||
        ((n2.x*n2.x + n2.y*n2.y + n2.z*n2.z) < 0.9)) {
      rt_tri(globscene, globtex, v0, v1, v2);
    } else { 
      rt_stri(globscene, globtex, v0, v1, v2, n0, n1, n2);
    }

    v1 = v2;
    n1 = n2;
  }

  return(MG_OK);                  /* normal exit */
}

static void DefaultTex(SceneHandle scene) {
  apitexture apitex;

  apitex.col.r=1.0;
  apitex.col.g=1.0;
  apitex.col.b=1.0;
  apitex.ambient=0.1;
  apitex.diffuse=0.9;
  apitex.specular=0.0;
  apitex.opacity=1.0;
  apitex.texturefunc=0;

  globtex = rt_texture(globscene, &apitex);
}


static void DefaultLight(SceneHandle scene) {
  /* lighting hack */
  apivector ctr;
  apitexture tex;

  memset(&tex, 0, sizeof(apitexture));

  tex.col.r = 1.0;
  tex.col.g = 1.0;
  tex.col.b = 1.0;
  ctr.x = 1000.0;
  ctr.y = 1000.0;
  ctr.z = -1000.0;

  rt_light(scene, rt_texture(globscene, &tex), ctr, 1.0);
}

static void DefaultScene(char *mgfname, SceneHandle scene) {
  FILE * ifp;
  char fname[1024];
  float x, y, z, zoom, len;
  apivector vp, vd, vup;

  strcpy(fname, mgfname);
  strcat(fname, ".scn");
  if ((ifp = fopen(fname, "r")) == NULL) {
    printf("No scene settings file found, using defaults\n");
    return;
  }
  printf("Reading default scene parameters from %s\n", fname);

  zoom = 1.0;
  fscanf(ifp, "%f", &zoom);

  fscanf(ifp, "%f %f %f", &x, &y, &z);
  vp = rt_vector(x, y, z);

  fscanf(ifp, "%f %f %f", &x, &y, &z);
  len = sqrt(x*x + y*y + z*z);
  vd = rt_vector(x/len, y/len, z/len);

  fscanf(ifp, "%f %f %f", &x, &y, &z);
  vup = rt_vector(x, y, z);

  rt_camera_setup(scene, zoom, 1.0, 0, 6, vp, vd, vup);
  
  fclose(ifp);
}

unsigned int ParseMGF(char *mgfname, SceneHandle scene, int defaultflag) {
  DefaultTex(scene);   /* hack, paranoia */

  globscene = scene;

  if (defaultflag == 1) {
    DefaultLight(scene); /* hack */
    DefaultScene(mgfname, scene); /* hack */
  }

  printf("MGF: loading %s\n", mgfname);

  /* initialize dispatch table */
  mg_ehand[MG_E_FACE] = myfaceh;          /* we do faces */
  mg_ehand[MG_E_VERTEX] = c_hvertex;      /* parser lib */
  mg_ehand[MG_E_POINT] = c_hvertex;       /* parser lib */
  mg_ehand[MG_E_NORMAL] = c_hvertex;      /* parser lib */
  mg_ehand[MG_E_XF] = xf_handler;         /* parser lib */

  /* lighting and coloring handling */
  mg_ehand[MG_E_COLOR] = c_hcolor;        /* they get color */
  mg_ehand[MG_E_CMIX] = c_hcolor;         /* they mix colors */
  mg_ehand[MG_E_CSPEC] = c_hcolor;        /* they get spectra */
  mg_ehand[MG_E_CXY] = c_hcolor;          /* they get chromaticities */
  mg_ehand[MG_E_CCT] = c_hcolor;          /* they get color temp's */
  mg_ehand[MG_E_ED] = c_hmaterial;        /* they get emission */
  mg_ehand[MG_E_MATERIAL] = c_hmaterial;  /* they get materials */
  mg_ehand[MG_E_RD] = c_hmaterial;        /* they get diffuse refl. */
  mg_ehand[MG_E_RS] = c_hmaterial;        /* they get specular refl. */
  mg_ehand[MG_E_SIDES] = c_hmaterial;     /* they get # sides */
  mg_ehand[MG_E_TD] = c_hmaterial;        /* they get diffuse trans. */
  mg_ehand[MG_E_TS] = c_hmaterial;        /* they get specular trans. */

  mg_init();                              /* initialize parser */
  if (mg_load(mgfname) != MG_OK)          /* and check for error */
    return MGF_BADSYNTAX;

  return MGF_NOERR;
}

#endif
