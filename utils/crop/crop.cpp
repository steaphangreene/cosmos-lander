#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <user.h>

int main(int argc, char **argv)  {
  InitUserEngine(argc, argv);
  if(argc != 6) Exit(1, "Usage: crop xstart ystart xsize ysize <bitmap>\n");
  {
    FILE *f = fopen(argv[5], "r");
    if(f==NULL) { printf("\"%s\" not found!\n", argv[5]); exit(1); }
    char buf[16];
    fread(buf, 1, 16, f);
    fclose(f);
    if(strncmp(buf, "BM", 2))  {
      printf("\"%s\" not BMP of RESFILE!\n", argv[5]);
      exit(1);
      }
    }
  int x1 = strtol(argv[1], NULL, 10);
  int y1 = strtol(argv[2], NULL, 10);
  int x2 = x1+strtol(argv[3], NULL, 10);
  int y2 = y1+strtol(argv[4], NULL, 10);
  Graphic og(argv[5]);
  Graphic ng(og.Partial(x1, y1, x2, y2));
  ng.SaveBMP(argv[5]);
  }
