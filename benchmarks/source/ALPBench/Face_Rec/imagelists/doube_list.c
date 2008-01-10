#include <stdio.h>

int main (int argc, char**argv) 
{
  int start_num = atoi(argv[1]);
  char* fn = argv[2];
  FILE* infile;
  char str[100];
  int blah[4];
  char *sp, dig[6], new_dig[6];
  int first=1, sub;

  dig[5] = new_dig[5]= '\0';
  infile = fopen(fn, "r");
  strcpy(dig,"00001");
  
  while(!feof(infile)) {
    fscanf(infile, "%s\n", str);
    if (!first) {
      if (strncmp(dig, str,5)!=0) 
	printf("\n");
      else
	printf("\t");
    } else first=0;
    
    strncpy(dig, str,5);
    
    sp = str;
    sp[3] = sp[3] + start_num;
    if (sp[3] > '9') {
      sp[3]-=10;
      sp[2]+=1;
    }
    printf("%s ",str);
  };
  printf("\n");
}
