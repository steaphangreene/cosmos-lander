#include <stdio.h>
#include <string.h>
#include <user.h>

Screen *screen;
Mouse *mouse;
Keyboard *key;
InputQueue *input;

int main(int argc, char **argv)  {
  if(argc < 2 || argc > 3)
    Exit(1, "Usage: pedit [<planet_resource> &| <planet_bitmap>]\n");
  int ftype = 0;
  { FILE *f = fopen(argv[1], "r");
    if(f==NULL) { printf("\"%s\" not found!\n", argv[1]); exit(1); }
    char buf[16];
    fread(buf, 1, 16, f);
    if(!strncmp(buf, "BM", 2)) {
      ftype = 1;
      }
    else if(!strncmp(buf, "Insomnia", 8))  {
      ftype = 2;
      }
    else { printf("\"%s\" not BMP of RESFILE!\n", argv[1]); exit(1); }
    if(ftype==2 && argc == 3) {
      FILE *f = fopen(argv[2], "r");
      if(f==NULL) { printf("\"%s\" not found!\n", argv[2]); exit(1); }
      char buf[16];
      fread(buf, 1, 16, f);
      if(!strncmp(buf, "BM", 2)) {
        ftype = 3;
        }
      else { printf("\"%s\" not BMP of RESFILE!\n", argv[1]); exit(1); }
      }
    fclose(f);
    }
  int ctr, ctr2;
  InitUserEngine(argc, argv);
  screen = new Screen(800, 768);
  key = new Keyboard;
  input = new InputQueue;
  mouse = new Mouse;
  CharBag *tltype;
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
  else { printf("AAARRRGGGHHH!!!\n"); exit(1); }
  screen->FullScreenGraphic(*pg);
  screen->Show();
  Graphic tlg("tile.bmp");
  Graphic wtlg("wtile.bmp");
  Graphic ltlg("ltile.bmp");
  Sprite *tl[25][24];
  for(ctr=0; ctr<25; ctr++) {
    for(ctr2=0; ctr2<24; ctr2++) {
      tl[ctr][ctr2] = new Sprite;
      switch((*tltype)[ctr+25*ctr2]) {
	default: tl[ctr][ctr2]->SetImage(tlg); break;
	case(1): tl[ctr][ctr2]->SetImage(ltlg); break;
	case(2): tl[ctr][ctr2]->SetImage(wtlg); break;
	}
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
      }
    if(inp->g.type == INPUTACTION_MOUSEDOWN) {
      if(inp->m.button == 1) {
	tl[inp->m.x>>5][inp->m.y>>5]->SetImage(tlg);
	tl[inp->m.x>>5][inp->m.y>>5]->Draw();
	(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 0;
	}
      if(inp->m.button == 2) {
	tl[inp->m.x>>5][inp->m.y>>5]->SetImage(wtlg);
	tl[inp->m.x>>5][inp->m.y>>5]->Draw();
	(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 2;
	}
      if(inp->m.button == 3) {
	tl[inp->m.x>>5][inp->m.y>>5]->SetImage(ltlg);
	tl[inp->m.x>>5][inp->m.y>>5]->Draw();
	(*tltype)[(inp->m.x>>5)+25*(inp->m.y>>5)] = 1;
	}
      }
    }
  }
