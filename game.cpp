#include <unistd.h>

#include "cosmos.h"
#include "game.h"
#include "troop.h"

Game::Game() {
  vp = NULL;
  }

int Game::Initialize() {
  Troop *tr=NULL; tr->InitTroopList();
  if(vp != NULL) return 0;
  int ctr, ctr2;
  screen->Show();
  numplayers = 2;
  ptype[0] = PLAYER_CONSOLE;
  ptype[1] = PLAYER_COMPUTER;

  for(ctr=0; ctr<MAX_PLANETS; ctr++) plan[ctr] = NULL;
  plan[0] = new Planet("planets/mars1.pf", "Ares", 0);
  plan[1] = new Planet("planets/venus1.pf", "Aphrodite", 0);
  plan[2] = new Planet("planets/ganymede2.pf", "Ganes", 1);
  plan[3] = new Planet("planets/ganymede1.pf", "Dierdra", 1);

  char buf[256];
  for(ctr=0; ctr<BUILDING_MAX; ++ctr) {
    int frm;
    sprintf(buf, "buildings/%.2d.bf%c", ctr, 0);
    ResFile rf(buf);
    CharBag *cb = rf.GetCharBag();
    buildimg[ctr] = rf.GetGraphic();
    screen->MakeFriendly(buildimg[ctr]);
    for(frm=0; frm<(unsigned char)(*cb)[0]; ++frm) {
      buildg[ctr][frm] = rf.GetGraphic();
      screen->MakeFriendly(buildg[ctr][frm]);
      }
    for(; frm<BUILDSTAT_MAX; ++frm) {
      if(frm>79) buildg[ctr][frm] = buildg[ctr][frm-64];
      else buildg[ctr][frm] = buildg[ctr][(unsigned char)(*cb)[0]-1];
      }
    for(ctr2=0; ctr2<cb->Size(); ++ctr2) {
      builds[ctr][ctr2] = (*cb)[ctr2];
      }
    delete(cb);
    }
  for(ctr=0; ctr<UNIT_MAX; ++ctr) {
    sprintf(buf, "graphics/troops/%.4d.tga%c", ctr, 0);
    if(access(buf, R_OK)) sprintf(buf, "graphics/troops/%.4d.bmp%c", ctr, 0);
    troopg[ctr] = new Graphic(buf);
    sprintf(buf, "graphics/troops/%.4di.tga%c", ctr, 0);
    if(access(buf, R_OK)) sprintf(buf, "graphics/troops/%.4di.bmp%c", ctr, 0);
    if(access(buf, R_OK)) troopimg[ctr] = troopg[ctr];
    else troopimg[ctr] = new Graphic(buf);
    
    troopg[ctr]->FindTrueCenter();
    screen->MakeFriendly(troopg[ctr]);
    }
  vp = new ViewPort;
  return 1;
  }

void Game::Play() {
  vp->Update();
  screen->RefreshFull();
  screen->Show();
  int quit=0;
  InputAction *ia;
  while(!quit) {
    screen->Refresh();
    ia = input->NextAction();
    while(ia != NULL) {
      if(ia->g.type == INPUTACTION_KEYDOWN) {
	if(ia->k.key == KEY_ESC) quit=1;
	else if(ia->k.key == KEY_S) { Save(NULL); vp->Update(); }
	else if(ia->k.key == KEY_L) { Load(NULL); vp->Key(ia->k.key); vp->Update(); }
	else vp->Key(ia->k.key);
	}
      else if(ia->g.type == INPUTACTION_MOUSEDOWN) {
	vp->Click(ia->m.x, ia->m.y, ia->m.button);
	}
      ia = input->NextAction();
      }
    vp->Position(mouse->XPos(), mouse->YPos());
    }
  }

void Game::Save(FILE *fl) {
  int ctr, cl=0;
  if(fl==NULL) { cl=1; fl=fopen("cosmos.sav", "w"); }
  if(fl) {
    fprintf(fl, "%d\n", MAX_PLANETS);
    for(ctr=0; ctr<MAX_PLANETS; ++ctr) {
      if(plan[ctr]==NULL) fprintf(fl, "0\n");
      else {
	fprintf(fl, "1\n");
	plan[ctr]->Save(fl);
	}
      }
    if(cl) fclose(fl);
    }
  }

void Game::Load(FILE *fl) {
  int ctr, cl=0, mp, ex;
  if(fl==NULL) { cl=1; fl=fopen("cosmos.sav", "r"); }
  if(fl) {
    fscanf(fl, "%d\n", &mp);
    for(ctr=0; ctr<mp; ++ctr) {
      if(plan[ctr]!=NULL) delete plan[ctr];
      fscanf(fl, "%d\n", &ex);
      if(ex) {
	plan[ctr] = new Planet(fl);
	}
      }
    if(cl) fclose(fl);
    }
  }

