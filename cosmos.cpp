#include <user.h>

#include "cosmos.h"
#include "planet.h"
#include "game.h"
#include "troop.h"

Screen *screen;
Keyboard *key;
Mouse *mouse;
InputQueue *input;

Planet *plan[MAX_PLANETS];
Graphic *troopg[TROOP_MAX];
Graphic *troopimg[TROOP_MAX];
Graphic *buildimg[BUILDING_MAX];
Graphic *buildg[BUILDING_MAX][BUILDSTAT_MAX];
Graphic *dmdth[17];
int builds[BUILDING_MAX][BUILDSTAT_MAX];
Sound *sound[SOUND_MAX];
int soundh[SOUND_MAX][2];
Speaker *spk;

int player=0;

int main(int argc, char **argv) {
  InitUserEngine(argc, argv);
  screen = new Screen(1024, 768, "Cosmos");
  spk = new Speaker(1, 16, 44100);
  screen->SetFrameRate(20);
  screen->SetFont("fonts/basic30.sgf");
  screen->SetPaletteEntry(0, 0, 0, 0);
  screen->SetPaletteEntry(255, 255, 255, 255);
  screen->SetApparentDepth(32);

  int ctr;
  for(ctr=0; ctr<SHOT_MAX; ctr++) {
    wsp[ctr] = -1;
    wsn[ctr] = 0;
    }
  for(ctr=0; ctr<17; ctr++) {
    char buf[256];
    sprintf(buf, "graphics/deaths/dome/%.4d.bmp%c", ctr, 0);
    dmdth[ctr] = new Graphic(buf);
    Palette p(buf);
    dmdth[ctr]->DepthConvert(32, p);
    screen->MakeFriendly(dmdth[ctr]);
    }

  Debug("Loading sound \"GUN\"");
  sound[SOUND_GUN] = new Sound("sounds/gun.wav");
  spk->MakeFriendly(*sound[SOUND_GUN]);
  Debug("Loading sound \"OPEN\"");
  sound[SOUND_OPEN] = new Sound("sounds/open.wav");
  spk->MakeFriendly(*sound[SOUND_OPEN]);
  Debug("Loading sound \"DEATH\"");
  sound[SOUND_DEATH] = new Sound("sounds/death.wav");
  spk->MakeFriendly(*sound[SOUND_DEATH]);
  Debug("Loading sound \"LASER\"");
  sound[SOUND_LASER] = new Sound("sounds/laser.wav");
  spk->MakeFriendly(*sound[SOUND_LASER]);
  Debug("Loading sound \"MASER\"");
  sound[SOUND_MASER] = new Sound("sounds/maser.wav");
  spk->MakeFriendly(*sound[SOUND_MASER]);
  Debug("Loading sound \"LASERMASER\"");
  sound[SOUND_LASERMASER] = new Sound("sounds/lmaser.wav");
  spk->MakeFriendly(*sound[SOUND_LASERMASER]);
  Debug("Loading sound \"BALLISTIC\"");
  sound[SOUND_BALLISTIC] = new Sound("sounds/ballistic.wav");
  spk->MakeFriendly(*sound[SOUND_BALLISTIC]);
  Debug("Loading sound \"SHELL\"");
  sound[SOUND_SHELL] = new Sound("sounds/shell.wav");
  spk->MakeFriendly(*sound[SOUND_SHELL]);
  Debug("Loading sound \"AMBIENT\"");
  sound[SOUND_AMBIENT] = new Sound("sounds/ambient.wav");
  Debug("Loading sound \"DOMEDIE\"");
  sound[SOUND_DOMEDIE] = new Sound("sounds/domedie.wav");
  spk->MakeFriendly(*sound[SOUND_AMBIENT]);
  spk->SetAsAmbient(*sound[SOUND_AMBIENT]);
  for(ctr=0; ctr<SOUND_MAX; ++ctr) { soundh[ctr][0]=-1; soundh[ctr][1]=-1; }
  key = new Keyboard;
  mouse = new Mouse;
  { Graphic cur("cursor.tga");
    screen->MakeFriendly(&cur);
    mouse->SetCursor(cur);
    }
  input = new InputQueue;
  Game *game = new Game;
  while(game->Initialize()) { game->Play(); }
  delete game;
  delete key;
  delete screen;
  return 0;
  }
