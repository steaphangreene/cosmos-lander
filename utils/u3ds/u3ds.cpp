#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <user.h>

int main(int argc, char **argv) {
  if(argc < 2) Exit(1, "Usage: tga2bmp <filename>...\n");
  char fn[256]; int ind; Graphic *g;

  int len;
  for(ind=1; ind<argc; ++ind) {
    if(!strcasecmp(".tga", &argv[ind][strlen(argv[ind])-4])) {
      sprintf(fn, "%s%c", argv[ind], 0);
      len = strlen(argv[ind]);
      fn[len-3] = 'b';
      fn[len-2] = 'm';
      fn[len-1] = 'p';
      g=new Graphic(argv[ind]);
      g->Undo3ds();
      g->SaveBMP(fn);
      delete g;
      }
    }
  }
