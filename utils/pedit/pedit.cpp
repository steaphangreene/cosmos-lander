#include <stdio.h>
#include <string.h>
#include <user.h>
#include <zlib.h>

#define PEDIT_VERSION "0.0.1"

Screen *screen;
Mouse *mouse;
Keyboard *key;
InputQueue *input;

int main(int argc, char **argv)  {
  if(argc < 2 || argc > 3)
    U2_Exit(1, "Usage: pedit [<planet_resource> &| <planet_bitmap>]\n");
  int ftype = 0;
  { gzFile f = gzopen(argv[1], "r");
    if(f==NULL) { printf("\"%s\" not found!\n", argv[1]); exit(1); }
    char buf[16];
    gzread(f, buf, 16);
    if(!strncmp(buf, "BM", 2)) {
      ftype = 1;
      }
    else if(!strncmp(buf, "Insomnia", 8))  {
      ftype = 2;
      }
    else { printf("\"%s\" not BMP or RESFILE!\n", argv[1]); exit(1); }
    if(ftype==2 && argc == 3) {
      FILE *f = fopen(argv[2], "r");
      if(f==NULL) { printf("\"%s\" not found!\n", argv[2]); exit(1); }
      char buf[16];
      fread(buf, 1, 16, f);
      if(!strncmp(buf, "BM", 2)) {
        ftype = 3;
        }
      else { printf("\"%s\" not BMP or RESFILE!\n", argv[1]); exit(1); }
      }
    gzclose(f);
    }
  int ctr, ctr2;
  U2_Init(argc, argv);
  screen = new Screen(800, 768, "Cosmos Planet Editor, ver" PEDIT_VERSION);
  key = new Keyboard;
  input = new InputQueue;
  mouse = new Mouse;
  CharBag *tltype = NULL;
  Graphic *pg=NULL;
  if(ftype & 1) {
    tltype = new CharBag(600);
    for(ctr=0; ctr<600; ctr++) (*tltype)[ctr] = 0;
    pg = new Graphic(argv[1+(ftype>1)]);
    }
  if(ftype & 2) {
    ResFile rf(argv[1]);
    tltype = rf.GetCharBag();
    if(ftype == 2) pg = rf.GetGraphic();
    }
  else if(!(ftype & 1)) { printf("AAARRRGGGHHH!!!\n"); exit(1); }
  screen->FullScreenGraphic(*pg);
  screen->Show();
  Graphic *tlg[4];
  tlg[0] = new Graphic("tile.bmp");
  tlg[1] = new Graphic("ltile.bmp");
  tlg[2] = new Graphic("wtile.bmp");
  Sprite *tl[25][24];
  for(ctr=0; ctr<25; ctr++) {
    for(ctr2=0; ctr2<24; ctr2++) {
      tl[ctr][ctr2] = new Sprite;
      tl[ctr][ctr2]->SetImage(tlg[(*tltype)[ctr+25*ctr2]]);
      tl[ctr][ctr2]->Move(ctr<<5, ctr2<<5);
      }
    }
  InputAction *inp;
  int quit=0;
  while(!quit) {
    inp = input->WaitForNextAction();
    if(inp->g.type == INPUTACTION_KEYDOWN) {
      if(inp->k.key == KEY_ESC) {
	quit=1;
	}
      if(inp->k.key == KEY_Q) {
	NewResFile rf("planet.pf");
	rf.Add(tltype);
	rf.Add(pg);
	quit=1;
	}
      if(inp->k.key == KEY_S) {
	NewResFile rf("planet.pf");
	rf.Add(tltype);
	rf.Add(pg);
	}
      }
    if(inp->g.type == INPUTACTION_MOUSEDOWN) {
      if(inp->m.button == 1) {
	(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 0;
	}
      else if(inp->m.button == 2) {
	(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 1;
	}
      else if(inp->m.button == 3) {
	(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 2;
	}
      else if(inp->m.button == 4) {
	++((*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)]);
	if((*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] > 2)
	  (*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 0;
	}
      else if(inp->m.button == 5) {
	--((*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)]);
	if((*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] < 0)
	  (*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 2;
	}
      tl[inp->m.x>>5][inp->m.y>>5]->
	SetImage(tlg[(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)]]);
      tl[inp->m.x>>5][inp->m.y>>5]->Draw();
      }
    }
  U2_Exit(0);
  }
