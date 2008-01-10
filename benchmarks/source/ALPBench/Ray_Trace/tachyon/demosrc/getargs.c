
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tachyon.h"
#include "getargs.h"
#include "ctype.h"

static int strupncmp(const char *a, const char *b, int n) {
   while (n-- > 0) {
      if (toupper(*a) != toupper(*b)) {
         return toupper(*b) - toupper(*a);
      }
      if (*a == 0) return 0;
      a++; b++;
   }
   return 0;
}

static int compare(const char *a, const char *b) {
  if (strlen(a) != strlen(b)) 
    return -1;

  return strupncmp(a, b, strlen(a));
}

static void printusage(char **argv) {
  printf("Usage: \n");
  printf("  %s modelfile [options] \n", argv[0]);
  printf("\n");
  printf("Model file formats supported:\n");
  printf("  filename.dat -- The model files originated with this package.\n");
  printf("  filaname.ac  -- AC3D model files.\n");
#ifdef USELIBMGF
  printf("  filaname.mgf -- MGF model files.\n");
#endif
  printf("  filename.nff -- The NFF scene format used by Eric Haines' SPD.\n");
  printf("\n");
  printf("Valid options:  (** denotes default behaviour)\n");
  printf("----------------------------------------------\n");
  printf("Message Options:\n");
  printf(" +V verbose messages on \n");
  printf(" -V verbose messages off **\n");
  printf("\n");
  printf("Speed Tuning Options:\n");
  printf(" -numthreads xxx   (** default is auto-determined)\n");
  printf(" -nobounding\n");
  printf(" -boundthresh xxx  (** default threshold is 16)\n");
  printf("\n");
  printf("Shading Options:\n");
  printf("  -fullshade    best quality rendering (and slowest) **\n");
  printf("  -mediumshade  good quality rendering, but no shadows\n");
  printf("  -lowshade     low quality rendering, preview (and fast)\n");
  printf("  -lowestshade  worst quality rendering, preview (and fastest)\n");
  printf("\n");
  printf("Specular Highlight Shading Options:\n");
  printf("  -shade_phong       Phong specular highlights\n");
  printf("  -shade_blinn       Blinn's specular highlights**\n");
  printf("  -shade_blinn_fast  fast approximation to Blinn's highlights\n");
  printf("  -shade_nullphong   disable specular highlights\n");
  printf("\n");
  printf("Transparency Shading Options:\n");
  printf("  -trans_orig        Original implementation**\n");
  printf("  -trans_vmd         Opacity post-multiply used by VMD\n");
  printf("\n");
  printf("Antialiasing Options:\n");
  printf("  -aasamples xxx  (maximum supersamples taken per pixel)\n");
  printf("                  (** default is 0, or scene file determined)\n");
  printf("\n");
  printf("Output Options:\n");
  printf("  -o outfile.tga  set output file name\n");
  printf("  -format TARGA   24-bit Targa        (uncompressed) **\n");
  printf("  -format BMP     24-bit Windows BMP  (uncompressed)\n");
  printf("  -format PPM     24-bit PPM          (uncompressed)\n");
  printf("  -format RGB     24-bit SGI RGB      (uncompressed)\n");
#if defined(USEJPEG)
  printf("  -format JPEG    24-bit JPEG         (compressed, but lossy)\n");
#else
  printf("  -format JPEG    XXX Not compiled into this binary XXX\n");
#endif
#if defined(USEPNG)
  printf("  -format PNG     24-bit PNG          (compressed, lossless)\n");
#else
  printf("  -format PNG     XXX Not compiled into this binary XXX\n");
#endif
  printf("\n");
  printf("Animation Related Options:\n");
  printf(" -camfile filename.cam  Animate using file of camera positions.\n");
  printf(" -nosave                Disable writing of output frames to disk\n");
  printf("                        (only used for doing real-time rendering)\n");
  printf("\n");

#if defined(USESPACEBALL)
  printf("Interactive Spaceball Control:\n");
  printf(" -spaceball devicename \n");
  printf("\n");
#endif

}

static void initoptions(argoptions * opt) {
  memset(opt, 0, sizeof(argoptions));
  opt->filenames = (char **) malloc(sizeof(char *) * 10);
  opt->numfiles = 0;
  opt->useoutfilename = -1;
  opt->outimageformat = -1;
  opt->verbosemode = -1;
  opt->aa_maxsamples = -1;
  opt->boundmode = -1; 
  opt->boundthresh = -1; 
  opt->usecamfile = -1;
  opt->shadermode = -1;
  opt->phongfunc = -1;
  opt->transmode = -1;
  opt->numthreads = -1;
  opt->nosave = -1;
}

int useoptions(argoptions * opt, SceneHandle scene) {
  if (opt->outimageformat == -1) {
    opt->outimageformat = RT_FORMAT_TARGA;
  } 

  if (opt->useoutfilename == -1) {
    if (opt->usecamfile != -1) {
      strcpy(opt->outfilename, "cam.%04d");
    } else if (opt->numfiles > 1) {
      strcpy(opt->outfilename, "outfile.%04d");
    } else {
      strcpy(opt->outfilename, "outfile");
    }

    switch (opt->outimageformat) {
      case RT_FORMAT_PPM:
        strcat(opt->outfilename, ".ppm");
        break;  
  
      case RT_FORMAT_WINBMP:
        strcat(opt->outfilename, ".bmp");
        break;  

      case RT_FORMAT_SGIRGB:
        strcat(opt->outfilename, ".rgb");
        break;  

      case RT_FORMAT_JPEG:
        strcat(opt->outfilename, ".jpg");
        break;  

      case RT_FORMAT_PNG:
        strcat(opt->outfilename, ".png");
        break;  

      case RT_FORMAT_TARGA:
                   default:
        strcat(opt->outfilename, ".tga");
        break;  
    }
  }

  if (opt->nosave == 1) {
    strcpy(opt->outfilename, "\0");
  }

  rt_outputformat(scene, opt->outimageformat);
  rt_outputfile(scene, opt->outfilename);

  if (opt->verbosemode == 1) {
    rt_verbose(scene, 1);
  }
  if (opt->verbosemode == 0) {
    rt_verbose(scene, 0);
  }  

  if (opt->aa_maxsamples != -1) {
    rt_aa_maxsamples(scene, opt->aa_maxsamples);
  } 

  if (opt->boundmode != -1) {
    rt_boundmode(scene, opt->boundmode);
  }

  if (opt->boundthresh != -1) {
    rt_boundthresh(scene, opt->boundthresh);
  }

  if (opt->shadermode != -1) {
    rt_shadermode(scene, opt->shadermode);
  }

  if (opt->phongfunc != -1) {
    rt_phong_shader(scene, opt->phongfunc);
  }

  if (opt->transmode != -1) {
    rt_trans_mode(scene, opt->transmode);
  }

  if (opt->numthreads != -1) {
    rt_set_numthreads(scene, opt->numthreads);
  }

  return 0;
}    

static int getparm(int argc, char **argv, int num, argoptions * opt, int node) {
  if (!strcmp(argv[num], "-o")) {
    opt->useoutfilename = 1;
    sscanf(argv[num + 1], "%s", (char *) &opt->outfilename);
    return 2;
  }
  if (!strcmp(argv[num], "-numthreads")) {
    sscanf(argv[num + 1], "%d", &opt->numthreads);
    return 2;
  }
  if (!strcmp(argv[num], "-camfile")) {
    opt->usecamfile = 1;
    sscanf(argv[num + 1], "%s", &opt->camfilename[0]);
    return 2;
  }
  if (!strcmp(argv[num], "-nosave")) {
    /* disable writing of images to disk files */
    opt->nosave = 1;
    return 1;
  }
  if (!strcmp(argv[num], "-aasamples")) {
    sscanf(argv[num + 1], "%d", &opt->aa_maxsamples);
    return 2;
  }
  if (!strcmp(argv[num], "-V")) {
    /* turn verbose messages off */
    opt->verbosemode = 0;
    return 1;
  }
  if (!strcmp(argv[num], "+V")) {
    /* turn verbose messages on */
    opt->verbosemode = 1;
    return 1;
  }
  if (!strcmp(argv[num], "-nobounding")) {
    /* disable automatic spatial subdivision optimizations */
    opt->boundmode = RT_BOUNDING_DISABLED;
    return 1;
  }
  if (!strcmp(argv[num], "-boundthresh")) {
    /* set automatic bounding threshold control value */
    sscanf(argv[num + 1], "%d", &opt->boundthresh);
    return 2;
  }
  if (!strcmp(argv[num], "-fullshade")) {
    opt->shadermode = RT_SHADER_FULL;
    return 1;
  }
  if (!strcmp(argv[num], "-mediumshade")) {
    opt->shadermode = RT_SHADER_MEDIUM;
    return 1;
  }
  if (!strcmp(argv[num], "-lowshade")) {
    opt->shadermode = RT_SHADER_LOW;
    return 1;
  }
  if (!strcmp(argv[num], "-lowestshade")) {
    opt->shadermode = RT_SHADER_LOWEST;
    return 1;
  }
  if (!strcmp(argv[num], "-shade_phong")) {
    opt->phongfunc  = RT_SHADER_PHONG;
    return 1;
  }
  if (!strcmp(argv[num], "-shade_blinn")) {
    opt->phongfunc  = RT_SHADER_BLINN;
    return 1;
  }
  if (!strcmp(argv[num], "-shade_blinn_fast")) {
    opt->phongfunc  = RT_SHADER_BLINN_FAST;
    return 1;
  }
  if (!strcmp(argv[num], "-shade_nullphong")) {
    opt->phongfunc  = RT_SHADER_NULL_PHONG;
    return 1;
  }
  if (!strcmp(argv[num], "-trans_orig")) {
    opt->transmode  = RT_TRANS_ORIG;
    return 1;
  }
  if (!strcmp(argv[num], "-trans_vmd")) {
    opt->transmode  = RT_TRANS_VMD;
    return 1;
  }
  if (!strcmp(argv[num], "-format")) {
    char str[80];
    sscanf(argv[num + 1], "%s", &str[0]);

    if (!compare(str, "TARGA")) {
      opt->outimageformat = RT_FORMAT_TARGA;
    } else if (!compare(str, "BMP")) {
      opt->outimageformat = RT_FORMAT_WINBMP;
    } else if (!compare(str, "PPM")) {
      opt->outimageformat = RT_FORMAT_PPM;
    } else if (!compare(str, "RGB")) {
      opt->outimageformat = RT_FORMAT_SGIRGB;
#if defined(USEJPEG)
    } else if (!compare(str, "JPEG")) {
      opt->outimageformat = RT_FORMAT_JPEG;
#endif
#if defined(USEPNG)
    } else if (!compare(str, "PNG")) {
      opt->outimageformat = RT_FORMAT_PNG;
#endif
    } else {
      if (node == 0) 
        printf("Unknown/Unsupported Image Format: %s, defaulting to Targa...\n", str);
    }
    return 2;
  }

#if defined(USESPACEBALL)
  if (!strcmp(argv[num], "-spaceball")) {
    sscanf(argv[num + 1], "%s", &opt->spaceball[0]);
    return 2;
  }
#endif

  /* unknown parameter setting */
  if (node == 0) 
    printf("Unrecognized parameter/option flag: %s\n", argv[num]);

  return -1;
}

int getargs(int argc, char **argv, argoptions * opt, int node) {
  int i, rc;

  if (opt == NULL)
    return -1;

  if (argc < 2) {
    if (node == 0)
      printusage(argv);

    return -1;
  }

  initoptions(opt);  

  i = 1;
  while (i < argc) {
    if (argv[i][0] == '-' || argv[i][0] == '+') {
      rc = getparm(argc, argv, i, opt, node);
      if (rc != -1) {
        i += rc;
      }
      else {
        if (node == 0)
          printusage(argv);

        return -1;
      }
    }
    else {
      opt->filenames = (char **) realloc(opt->filenames, sizeof(char *) * (opt->numfiles + 10));
      opt->filenames[opt->numfiles] = 
        (char *) malloc(sizeof(char) * strlen(argv[i]) + 1);
      strcpy(opt->filenames[opt->numfiles], argv[i]);        

#if 0
{ int i; 
  for (i=0; i<opt->numfiles; i++) {
    printf("parsefile[%d]: %s\n", i, opt->filenames[i]);
  }
}
#endif

      opt->numfiles++;
      i++;
    }
  }

  if (opt->numfiles == 0) {
    if (node == 0) {
      printf("Missing model file name!\n");
      printusage(argv);
    } 
    return -1;
  }

  return 0;
}

void freeoptions(argoptions * opt) {
  if (opt->filenames != NULL) {
    int i;

    for (i=0; i<opt->numfiles; i++) {
      if (opt->filenames[i] != NULL)
        free(opt->filenames[i]);
    }

    free(opt->filenames);
  }
}

