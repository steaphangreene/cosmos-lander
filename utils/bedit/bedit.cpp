#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <user.h>

static int NUM_FRAMES=144;

int main(int argc, char **argv)  {
  if(argc != 2) Exit(1, "Usage: bedit <building_bitmap>\n");
  int ftype = 0;
  {
    struct stat s;
    stat(argv[1], &s);

    FILE *f = fopen(argv[1], "r");
    if(f==NULL) { printf("\"%s\" not found!\n", argv[1]); exit(1); }
    char buf[16];
    fread(buf, 1, 16, f);
    fclose(f);

    if (S_ISDIR(s.st_mode)) {
      ftype = 1;
      }
    else if(!strncmp(buf, "Insomnia", 8))  {
      ftype = 2;
      }
    else { printf("\"%s\" not BMP of RESFILE!\n", argv[1]); exit(1); }
    }
  int ctr;
  InitUserEngine(argc, argv);
  CharBag *stats;
  Graphic *img;
  Graphic *bg[NUM_FRAMES];
  int frm;
  char buf[256];
  if(ftype == 1) {
    sprintf(buf, "%s/image.tga%c", argv[1], 0);
    img = new Graphic(buf);
    stats = new CharBag(10);
    for(frm=0; frm<NUM_FRAMES; frm++) {
      sprintf(buf, "%s/%.4d.tga%c", argv[1], frm, 0);
      if(access(buf, R_OK)) sprintf(buf, "%s/%.4d.bmp%c", argv[1], frm, 0);
      if(!access(buf, R_OK)) {
	bg[frm] = new Graphic(buf);
	bg[frm]->FindTrueCenter();
	bg[frm]->Trim();
	}
      else if(frm>0) {
	if(frm==16 || frm==80) NUM_FRAMES = frm;
	else {
          if(frm>79) bg[frm] = new Graphic(*bg[frm-64]);
	  else bg[frm] = new Graphic(*bg[frm-1]);
	  }
	}
      else { printf("Umm...  AAARRRGGGHHH!!!\n"); exit(1); }
      }
    }
  else if(ftype == 2) {
    ResFile rf(argv[1]);
    stats = rf.GetCharBag();
    for(frm=0; frm<NUM_FRAMES; frm++) bg[frm] = rf.GetGraphic();
    }
  else { printf("AAARRRGGGHHH!!!\n"); exit(1); }
  {
    NewResFile rf("build.bf");
    (*stats)[0] = NUM_FRAMES;
    for(ctr=1; ctr<10; ctr++) (*stats)[ctr] = 0;
    rf.Add(stats);
    rf.Add(img);
    for(frm=0; frm<NUM_FRAMES; frm++) rf.Add(bg[frm]);
    }
  }
