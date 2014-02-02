#include <string.h>
#include <math.h>
#include <algorithm>
using std::max;

#include "cosmos.h"
#include "viewport.h"
#include "troop.h"
#include "ship.h"

int ticker;

#define TOP_BAR1_Y	0
#define TOP_BAR1_YS	32
#define TOP_BAR2_Y	32
#define TOP_BAR2_YS	32
#define BAR_Y		64
#define BAR_X		800
#define BAR_XS		224
#define BAR_YS		704
#define PANE_YS		384
#define PANE_SPREAD	(BAR_YS-PANE_YS)
#define PANE_TAIL	(PANE_YS+BAR_Y)
#define PANE_AREA_Y	(BAR_Y+104)
#define PANE_AREA_X	(BAR_X+4)
#define TAB_X		800
#define TAB_Y		32
#define TAB_XS		128
#define TAB_YS		32
#define TAB_SPREAD	(BAR_XS-TAB_XS)

static Sprite *attsp[MAX_SHIPS][MAX_DROPSHIPS];
static Sprite *attpsp[MAX_SHIPS][MAX_DROPSHIPS];

const int tmax[VPSTAT_MAX] = { BTYPE_MAX, 0, MAX_SHIPS };
const int pmax[VPSTAT_MAX] = { BUILDING_MAX, 0, MAX_SHIPS*MAX_DROPSHIPS };

extern int *numships;
extern Ship ***ship;

Troop *pdome[25][24];

ViewPort::ViewPort() {
  mode = VPMODE_PLANET;
  omode = VPMODE_PLANET-1;
  stat = VPSTAT_BUILD;
  ostat = VPSTAT_CONFIG;
  selx = -1;
  sely = -1;
  tsel[VPSTAT_BUILD] = 0;
  tsel[VPSTAT_CONFIG] = 0;
  tsel[VPSTAT_VIEW] = 0;
  tsel[VPSTAT_ATTACK] = 0;
  otsel[VPSTAT_BUILD] = 0;
  otsel[VPSTAT_CONFIG] = 0;
  otsel[VPSTAT_VIEW] = 0;
  otsel[VPSTAT_ATTACK] = 0;
  planet = 0;
  oplanet = 1;
  tile = -1;
  system = 1;
  screen->SetPaletteEntry(0x81, 0x40, 0x40, 0x80);
  screen->SetPaletteEntry(0x80, 0x80, 0x80, 0x80);
  int ctr, ind;
  psel[VPSTAT_BUILD] = new int[BTYPE_MAX];
  opsel[VPSTAT_BUILD] = new int[BTYPE_MAX];
  memset(psel[VPSTAT_BUILD], 0, sizeof(int)*BTYPE_MAX);
  memset(opsel[VPSTAT_BUILD], 0, sizeof(int)*BTYPE_MAX);
  pane[VPSTAT_BUILD] = new Sprite[BUILDING_MAX];
  tab[VPSTAT_BUILD] = new Sprite[BTYPE_MAX];
  tnum[VPSTAT_BUILD] = BTYPE_MAX;
  tnum[VPSTAT_CONFIG] = 0;
  tnum[VPSTAT_VIEW] = 0;
  tnum[VPSTAT_ATTACK] = numships[player];
  pnum[VPSTAT_BUILD] = new int[BTYPE_MAX];
  poff[VPSTAT_BUILD] = new int[BTYPE_MAX];

  psel[VPSTAT_CONFIG] = new int[1];
  opsel[VPSTAT_CONFIG] = new int[1];
  memset(psel[VPSTAT_CONFIG], 0, sizeof(int)*1);
  memset(opsel[VPSTAT_CONFIG], 0, sizeof(int)*1);
  pane[VPSTAT_CONFIG] = NULL;
  tab[VPSTAT_CONFIG] = NULL;
  pnum[VPSTAT_CONFIG] = new int[1];
  pnum[VPSTAT_CONFIG][0] = 0;
  poff[VPSTAT_CONFIG] = NULL;

  psel[VPSTAT_VIEW] = new int[1];
  opsel[VPSTAT_VIEW] = new int[1];
  memset(psel[VPSTAT_VIEW], 0, sizeof(int)*1);
  memset(opsel[VPSTAT_VIEW], 0, sizeof(int)*1);
  pane[VPSTAT_VIEW] = NULL;
  tab[VPSTAT_VIEW] = NULL;
  pnum[VPSTAT_VIEW] = new int[1];
  pnum[VPSTAT_VIEW][0] = 0;
  poff[VPSTAT_VIEW] = NULL;

  psel[VPSTAT_ATTACK] = new int[MAX_SHIPS];
  opsel[VPSTAT_ATTACK] = new int[MAX_SHIPS];
  memset(psel[VPSTAT_ATTACK], 0, sizeof(int)*MAX_SHIPS);
  memset(opsel[VPSTAT_ATTACK], 0, sizeof(int)*MAX_SHIPS);
  pane[VPSTAT_ATTACK] = new Sprite[MAX_SHIPS*MAX_DROPSHIPS];
  tab[VPSTAT_ATTACK] = new Sprite[MAX_SHIPS];
  pnum[VPSTAT_ATTACK] = new int[MAX_SHIPS];
  poff[VPSTAT_ATTACK] = new int[MAX_SHIPS];
  Graphic g("pane.tga"), *g2, tg("tab.tga");
  screen->MakeFriendly(&g);
  screen->MakeFriendly(&tg);
  screen->SetFont("fonts/basic16.sgf");
  for(ctr=0; ctr<BTYPE_MAX; ++ctr) {
    g2 = new Graphic(tg);
    if(ctr==BTYPE_MISC)	
	screen->GPrint(g2, 20, 12, 0, 0xFFFFFFFF, "Misc.");
    else if(ctr==BTYPE_WEAPON)
	screen->GPrint(g2, 20, 12, 0, 0xFFFFFFFF, "Weapons");
    else if(ctr==BTYPE_GENERATOR)
	screen->GPrint(g2, 20, 12, 0, 0xFFFFFFFF, "Generators");
    tab[VPSTAT_BUILD][ctr].SetImage(g2);
    tab[VPSTAT_BUILD][ctr].SetPriority(10000+ctr);
    tab[VPSTAT_BUILD][ctr].Position(BAR_X+(TAB_SPREAD*ctr)/(BTYPE_MAX-1), 32);
    tab[VPSTAT_BUILD][ctr].Remove();
    delete g2;
    }
  for(ctr=0; ctr<BUILDING_MAX; ++ctr) {
    g2 = new Graphic(g);
    g2->PasteGraphic(buildimg[ctr], 3, 3);
    pane[VPSTAT_BUILD][ctr].SetImage(g2);
    pane[VPSTAT_BUILD][ctr].DisableCollisions();
    pane[VPSTAT_BUILD][ctr].SetPriority(10000+ctr);
    pane[VPSTAT_BUILD][ctr].SetFlag(SPRITE_RECTANGLE|SPRITE_SOLID);
    pnum[VPSTAT_BUILD][0]=BUILDING_MISC_MAX;
    poff[VPSTAT_BUILD][0]=0;
    pnum[VPSTAT_BUILD][1]=BUILDING_WEAPON_MAX-BUILDING_MISC_MAX;
    poff[VPSTAT_BUILD][1]=BUILDING_MISC_MAX;
    pnum[VPSTAT_BUILD][2]=BUILDING_GENERATOR_MAX-BUILDING_WEAPON_MAX;
    poff[VPSTAT_BUILD][2]=BUILDING_WEAPON_MAX;
    if(ctr<BUILDING_MISC_MAX) {
      ind=ctr;
      int pn=max(pnum[VPSTAT_BUILD][0]-1, 1);
      pane[VPSTAT_BUILD][ctr].Position(BAR_X, BAR_Y+(PANE_SPREAD*ind)/pn);
      pane[VPSTAT_BUILD][ctr].Remove();
      }
    else if(ctr<BUILDING_WEAPON_MAX) {
      int pn=max(pnum[VPSTAT_BUILD][1]-1, 1);
      ind=ctr-BUILDING_MISC_MAX;
      pane[VPSTAT_BUILD][ctr].Position(BAR_X, BAR_Y+(PANE_SPREAD*ind)/pn);
      pane[VPSTAT_BUILD][ctr].Remove();
      }
    else if(ctr<BUILDING_GENERATOR_MAX) {
      int pn=max(pnum[VPSTAT_BUILD][2]-1, 1);
      ind=ctr-BUILDING_WEAPON_MAX;
      pane[VPSTAT_BUILD][ctr].Position(BAR_X, BAR_Y+(PANE_SPREAD*ind)/pn);
      pane[VPSTAT_BUILD][ctr].Remove();
      }
    delete g2;
    }
  for(ctr=0; ctr<numships[player]; ++ctr) {
    g2 = new Graphic(tg);
    screen->GPrint(g2, 20, 12, 0, 0xFFFFFFFF, ship[player][ctr]->Name());
    tab[VPSTAT_ATTACK][ctr].SetImage(g2);
    tab[VPSTAT_ATTACK][ctr].SetPriority(10000+ctr);
    tab[VPSTAT_ATTACK][ctr].Position(BAR_X+(TAB_SPREAD*ctr)/((numships[player])-1), 32);
    tab[VPSTAT_ATTACK][ctr].Remove();
    delete g2;
    pnum[VPSTAT_ATTACK][ctr]=MAX_DROPSHIPS;
    poff[VPSTAT_ATTACK][ctr]=MAX_DROPSHIPS*ctr;
    int ctrd;
    g2 = new Graphic(g);
    g2->PasteGraphic(troopimg[TROOP_DROPSHIP], 3, 3);
    for(ctrd=0; ctrd<MAX_DROPSHIPS; ++ctrd) {
      int ind=ctrd+MAX_DROPSHIPS*ctr;
      int pn=max(pnum[VPSTAT_ATTACK][0]-1, 1);
      pane[VPSTAT_ATTACK][ind].SetImage(g2);
      pane[VPSTAT_ATTACK][ind].DisableCollisions();
      pane[VPSTAT_ATTACK][ind].SetPriority(10000+ctr);
      pane[VPSTAT_ATTACK][ind].SetFlag(SPRITE_RECTANGLE|SPRITE_SOLID);
      pane[VPSTAT_ATTACK][ind].Position(BAR_X, BAR_Y+(PANE_SPREAD*ctrd)/pn);
      pane[VPSTAT_ATTACK][ind].Remove();

      attsp[ctr][ctrd] = new Sprite(*troopg[TROOP_DROPSHIP]);
      attpsp[ctr][ctrd] = new Sprite();

      int ctr1, ctr2, max=2, y=124;
      TroopStats *ts = ship[player][ctr]->Drop(ctrd);
      Graphic *g = pane[VPSTAT_ATTACK][ind].GetImage();
      screen->SetFont("fonts/basic16.sgf");
      screen->GPrint(g, 4, 104, 0x00000000, 0xFFFFFFFF, "Troops:");
      g->DrawFillRect(12, y, BAR_XS-24, 2, 32, 0xFF7F7F7F);
      y+=4;
      for(ctr1=0; ctr1<max; ++ctr1) {
	if(ts->contains[ctr1]) {
	  for(ctr2=0; ctr2<ts->contains[ctr1]->modnum; ++ctr2) {
	    screen->GPrintf(g, 20, y,
		0x00000000, 0xFFFFFFFF, "%s", ts->contains[ctr1]->name);
	    y+=18;
	    }
	  g->DrawFillRect(12, y+2, BAR_XS-24, 2, 32, 0xFF7F7F7F);
	  y+=6;
	  }
	else {
	  screen->GPrint(g, 20, y, 0x00000000, 0xFFFFFFFF, "Empty");
	  y+=18;
	  g->DrawFillRect(12, y+2, BAR_XS-24, 2, 32, 0xFF7F7F7F);
	  y+=6;
	  }
	}
      }
    }
  delete g2;
  }

void ViewPort::Update()  {
  int ctr, ctr1, ctr2;
  if(plan[planet]->Owner()!=player) {
    if(stat==VPSTAT_CONFIG||stat==VPSTAT_BUILD) stat=VPSTAT_VIEW;
    }
  else {
    if(stat==VPSTAT_VIEW||stat==VPSTAT_ATTACK) stat=VPSTAT_BUILD;
    }

  if(omode != mode) {
    switch(mode) {
      case(VPMODE_PLANET): {
	screen->FullScreenGraphic(*(plan[planet]->Image()));
	}break;
      default:  {
	fprintf(stderr, "Unimplemented vpmode=%d\n", mode);
	exit(1);
	}break;
      }
    omode=mode;
    }

  switch(mode) {
    case(VPMODE_PLANET): {
      if(planet!=oplanet) {
        for(ctr1=0; ctr1<25; ++ctr1) for(ctr2=0; ctr2<24; ++ctr2) {
	  plan[oplanet]->items[ctr1][ctr2].Erase();
	  if(plan[planet]->Dome(ctr1, ctr2)>=0)
		plan[planet]->items[ctr1][ctr2].Draw();
	  }
	screen->FullScreenGraphic(*(plan[planet]->Image()));
	oplanet=planet;
	screen->RestoreRectangle(0, 0, 800, 768);
	screen->Refresh();
	if(plan[oplanet]->Owner()==player) stat=VPSTAT_BUILD;
	else stat=VPSTAT_VIEW;
	}
      if(stat!=ostat) {
	for(ctr=0; ctr<pmax[ostat]; ++ctr) {
	  pane[ostat][ctr].Remove();
	  }
	if(ostat == VPSTAT_ATTACK) {
	  for(ctr=0; ctr<tmax[ostat] && ctr<numships[player]; ++ctr) {
	    for(ctr2=0; ctr2<pnum[ostat][ctr]; ++ctr2) {
	      if(attpsp[ctr][ctr2]) attpsp[ctr][ctr2]->Erase();
	      }
	    }
	  }
	for(ctr=0; ctr<tmax[ostat]; ++ctr) {
	  tab[ostat][ctr].Remove();
	  }

	if(ostat == VPSTAT_CONFIG) {
	  plan[planet]->Deselect(); selx=-1; sely=-1;
	  screen->ClearArea(BAR_X, BAR_Y, BAR_XS, BAR_YS);
	  }

	for(ctr=0; ctr<pnum[stat][tsel[stat]]; ++ctr) {
	  pane[stat][ctr+poff[stat][tsel[stat]]].Position();
	  }
	for(ctr=0; ctr<tmax[stat]; ++ctr) {
	  tab[stat][ctr].Position();
	  }

	screen->SetFont("fonts/basic30.sgf");
	if(stat == VPSTAT_BUILD) {
	  screen->DrawRectangle(BAR_X, TOP_BAR1_Y, BAR_XS, TOP_BAR1_YS, 0xFF007F00);
	  screen->Printf(BAR_X, TOP_BAR1_Y,
		0x00007F00, 0xFF00FF00, "   Build   ");
	  }
	else if(stat == VPSTAT_CONFIG) {
	  screen->DrawRectangle(BAR_X, TOP_BAR1_Y, BAR_XS, TOP_BAR1_YS, 0xFF7F7F00);
	  screen->Printf(BAR_X, TOP_BAR1_Y,
		0x007F7F00, 0xFFFFFF00, " Configure ");
	  }
	else if(stat == VPSTAT_ATTACK) {
	  screen->DrawRectangle(BAR_X, TOP_BAR1_Y, BAR_XS, TOP_BAR1_YS, 0xFF7F0000);
	  screen->Printf(BAR_X, TOP_BAR1_Y,
		0x007F0000, 0xFFFF0000, "  ATTACK!  ");
	  }
	else if(stat == VPSTAT_VIEW) {
	  screen->DrawRectangle(BAR_X, TOP_BAR1_Y, BAR_XS, TOP_BAR1_YS, 0xFF00007F);
	  screen->Printf(BAR_X, TOP_BAR1_Y,
		0x0000007F, 0xFF0000FF, "   View    ");
	  }

	ostat=stat;
	}

      if(tsel[stat] != otsel[stat]) {
	for(ctr=0; ctr<pnum[stat][otsel[stat]]; ++ctr) {
	  pane[stat][ctr+poff[stat][otsel[stat]]].Remove();
	  }
	if(stat == VPSTAT_ATTACK) for(ctr=0; ctr<tmax[stat]; ++ctr) {
	  for(ctr2=0; ctr2<pnum[stat][ctr]; ++ctr2) {
	    attpsp[ctr][ctr2]->Erase();
	    }
	  }
	for(ctr=0; ctr<pnum[stat][tsel[stat]]; ++ctr) {
	  pane[stat][ctr+poff[stat][tsel[stat]]].Position();
	  if(stat == VPSTAT_ATTACK && ship[player][tsel[stat]]->DropX(ctr)>=0)
		attpsp[tsel[stat]][ctr]->Draw();
	  }
	for(ctr=0; ctr<tmax[stat]; ++ctr)
	  tab[stat][ctr].SetPriority(10000+abs(ctr-tsel[stat]));

	otsel[stat]=tsel[stat];
	}

      if(psel[stat][tsel[stat]] != opsel[stat][tsel[stat]]) {
	for(ctr=0; ctr<pnum[stat][tsel[stat]]; ++ctr) {
	  int ind=ctr+poff[stat][tsel[stat]];
	  pane[stat][ind].SetPriority(10000+abs(ctr-psel[stat][tsel[stat]]));
	  }
	opsel[stat][tsel[stat]]=psel[stat][tsel[stat]];
	}

      if(stat == VPSTAT_CONFIG) {
	static Graphic *g=NULL;
	if(!g) { g = new Graphic("fullpane.tga"); screen->MakeFriendly(g); }
	seltp=plan[planet]->Building(selx, sely);
	g->PasteGraphic(buildimg[seltp], 3, 3);
	screen->DrawGraphic(*g, BAR_X, BAR_Y);
	screen->SetFont("fonts/basic16.sgf");
	TroopStats *ts=plan[planet]->bt[selx][sely];
	if(ts!=NULL && ts->wtype == WEAPON_DEPLOY) {
	  screen->Print(PANE_AREA_X, PANE_AREA_Y,
		0x00000000, 0xFFFFFFFF, "Troops:");
	  screen->DrawRectangle(PANE_AREA_X+8, PANE_AREA_Y+20,
		BAR_XS-24, 2, 0xFF7F7F7F);
	  int max=4, y=PANE_AREA_Y+24;
	  for(ctr=0; ctr<max; ++ctr) {
	    if(ts->contains[ctr]) {
	      for(ctr2=0; ctr2<ts->contains[ctr]->modnum; ++ctr2) {
		screen->Printf(PANE_AREA_X+16, y,
			0x00000000, 0xFFFFFFFF, "%s", ts->contains[ctr]->name);
		y+=18;
		}
	      screen->DrawRectangle(PANE_AREA_X+8, y+2,
			BAR_XS-24, 2, 0xFF7F7F7F);
	      y+=6;
	      }
	    else {
	      screen->Print(PANE_AREA_X+16, y, 0x00000000, 0xFFFFFFFF, "Empty");
	      y+=18;
	      screen->DrawRectangle(PANE_AREA_X+8, y+2,
			BAR_XS-24, 2, 0xFF7F7F7F);
	      y+=6;
	      }
	    }
	  }
	else if(ts!=NULL) {
	  screen->Print(PANE_AREA_X+16, PANE_AREA_Y, 0x00000000, 0xFFFFFFFF, "Range:");
	  screen->Printf(PANE_AREA_X+70, PANE_AREA_Y, 0x00000000, 0xFFFFFFFF, "%d", ts->wrng);

	  screen->Print(PANE_AREA_X+16, PANE_AREA_Y+18, 0x00000000, 0xFFFFFFFF, "S Dam:");
	  screen->Printf(PANE_AREA_X+70, PANE_AREA_Y+18, 0x00000000, 0xFFFFFFFF, "%d", ts->wdam);

	  screen->Print(PANE_AREA_X+16, PANE_AREA_Y+36, 0x00000000, 0xFFFFFFFF, "M Dam:");
	  screen->Printf(PANE_AREA_X+70, PANE_AREA_Y+36, 0x00000000, 0xFFFFFFFF, "%d", ts->wmdam);
	  }
	}
      screen->RestoreRectangle(BAR_X, 0, BAR_XS, 768);
      }break;
    default:  {
      fprintf(stderr, "Unimplemented vpmode=%d\n", mode);
      exit(1);
      }break;
    }
  }

void ViewPort::Key(int k)  {
  if(k==KEY_TAB || k==KEY_P) {
    do{ ++planet; planet%=MAX_PLANETS; } while(plan[planet]==NULL);
    Update();
    }
  else if(k==KEY_L) {
    if(mode==VPMODE_PLANET && stat!=VPSTAT_ATTACK) {
//      FILE *fl=fopen("planet.sav", "r");
//      if(fl!=NULL) plan[planet]->Load(fl);
//      fclose(fl);
      }
    else if(mode==VPMODE_PLANET && stat==VPSTAT_ATTACK) {
      FILE *fl=fopen("attack.sav", "r");
      if(fl!=NULL) LoadPlan(fl);
      fclose(fl);
      }
    }
  else if(k==KEY_S) {
    if(mode==VPMODE_PLANET && stat!=VPSTAT_ATTACK) {
      FILE *fl=fopen("planet.sav", "w");
      if(fl!=NULL) plan[planet]->Save(fl);
      fclose(fl);
      }
    else if(mode==VPMODE_PLANET && stat==VPSTAT_ATTACK) {
      FILE *fl=fopen("attack.sav", "w");
      if(fl!=NULL) SavePlan(fl);
      fclose(fl);
      }
    }
  }

void ViewPort::Click(int x, int y, int b)  {
  int ctr1, ctr2;
  int pn=max(pnum[stat][tsel[stat]]-1, 1);
  if(x >= BAR_X && y >= BAR_Y) {
    if(stat==VPSTAT_BUILD) {
      int ps=psel[stat][tsel[stat]];
      while(y < BAR_Y+(PANE_SPREAD*ps)/pn && ps>0) --ps;
      while(y >= PANE_TAIL+(PANE_SPREAD*ps)/pn
		&& ps<pnum[stat][tsel[stat]]) ++ps;
      psel[stat][tsel[stat]] = ps;
      Update();
      }
    else if(stat==VPSTAT_ATTACK) {
      int ps=psel[stat][tsel[stat]];
      if(y < BAR_Y+(PANE_SPREAD*ps)/pn && ps>0) {
	while(y < BAR_Y+(PANE_SPREAD*ps)/pn && ps>0) --ps;
	psel[stat][tsel[stat]] = ps;
	}
      else if(y >= PANE_TAIL+(PANE_SPREAD*ps)/pn
		&& ps<pnum[stat][tsel[stat]]) {
	while(y >= PANE_TAIL+(PANE_SPREAD*ps)/pn
		&& ps<pnum[stat][tsel[stat]]) ++ps;
	psel[stat][tsel[stat]] = ps;
	}
      else {	
	int ctr, yp=PANE_AREA_Y+21+(PANE_SPREAD*ps)/pn;
	TroopStats *ts = ship[player][tsel[stat]]->Drop(psel[stat][tsel[stat]]);
	if(y>=yp) for(ctr=0; ctr<ts->modnum; ++ctr) {
	  if(ts->contains[ctr]) yp += (6+18*ts->contains[ctr]->modnum);
	  else yp += 24;
	  if(y<yp) {
	    if(b==1) {
	      int ctr2, which=0;  Troop *tr=NULL;
	      if(ts->contains[ctr]) which=ts->contains[ctr]->type+1;
	      for(ctr2=which; ctr2<TROOP_MAX
		&& (tr->defstat[ctr2].wtype == WEAPON_DEPLOY
		|| (!tr->defstat[ctr2].modnum)); ++ctr2);
	      if(ts->contains[ctr])
	        { delete ts->contains[ctr]; ts->contains[ctr]=NULL; }
	      if(ctr2<TROOP_MAX)
	        ts->contains[ctr]=new TroopStats(tr->defstat[ctr2]);
	      Update();
	      }
	    else {
	      int ctr2, which=TROOP_MAX-1;  Troop *tr=NULL;
	      if(ts->contains[ctr]) which=ts->contains[ctr]->type-1;
	      for(ctr2=which; ctr2>0
		&& (tr->defstat[ctr2].wtype == WEAPON_DEPLOY
		|| (!tr->defstat[ctr2].modnum)); --ctr2);
	      if(ts->contains[ctr])
	        { delete ts->contains[ctr]; ts->contains[ctr]=NULL; }
	      if(ctr2 > 0)
	        ts->contains[ctr]=new TroopStats(tr->defstat[ctr2]);
	      Update();
	      }
	    int max=2, y=124;
	    TroopStats *ts = ship[player][tsel[stat]]->Drop(psel[stat][tsel[stat]]);
	    Graphic *g = pane[VPSTAT_ATTACK][psel[stat][tsel[stat]]+poff[stat][tsel[stat]]].GetImage();
	    g->ClearArea(4, 104, BAR_XS-8, PANE_YS-108);
	    screen->SetFont("fonts/basic16.sgf");
	    screen->GPrint(g, 4, 104, 0x00000000, 0xFFFFFFFF, "Troops:");
	    g->DrawFillRect(12, y, BAR_XS-24, 2, 32, 0xFF7F7F7F);
	    y+=4;
	    for(ctr1=0; ctr1<max; ++ctr1) {
	      if(ts->contains[ctr1]) {
		for(ctr2=0; ctr2<ts->contains[ctr1]->modnum; ++ctr2) {
		  screen->GPrintf(g, 20, y, 0x00000000, 0xFFFFFFFF,
			"%s", ts->contains[ctr1]->name);
		  y+=18;
		  }
		g->DrawFillRect(12, y+2, BAR_XS-24, 2, 32, 0xFF7F7F7F);
		y+=6;
		}
	      else {
		screen->GPrint(g, 20, y, 0x00000000, 0xFFFFFFFF, "Empty");
		y+=18;
		g->DrawFillRect(12, y+2, BAR_XS-24, 2, 32, 0xFF7F7F7F);
		y+=6;
		}
	      }
	    break;
	    }
	  }
	}
      Update();
      }
    else if(stat==VPSTAT_CONFIG) {
      TroopStats *ts=plan[planet]->bt[selx][sely];
      int ctr, yp=PANE_AREA_Y+21;
      if(y>=yp) for(ctr=0; ctr<ts->modnum; ++ctr) {
	if(ts->contains[ctr]) yp += (6+18*ts->contains[ctr]->modnum);
	else yp += 24;
	if(y<yp && b==1) {
	  int which=0;  Troop *tr=NULL;
	  if(ts->contains[ctr]) which=ts->contains[ctr]->type+1;
	  for(ctr2=which; ctr2<TROOP_MAX
		&& (tr->defstat[ctr2].wtype == WEAPON_DEPLOY
		|| (!tr->defstat[ctr2].modnum)); ++ctr2);
	  if(ts->contains[ctr])
	    { delete ts->contains[ctr]; ts->contains[ctr]=NULL; }
	  if(ctr2<TROOP_MAX)
	    ts->contains[ctr]=new TroopStats(tr->defstat[ctr2]);
	  Update(); break;
	  }
	else if(y<yp) {
	  int ctr2, which=TROOP_MAX-1;  Troop *tr=NULL;
	  if(ts->contains[ctr]) which=ts->contains[ctr]->type-1;
	  for(ctr2=which; ctr2>0
		&& (tr->defstat[ctr2].wtype == WEAPON_DEPLOY
		|| (!tr->defstat[ctr2].modnum)); --ctr2);
	  if(ts->contains[ctr])
	    { delete ts->contains[ctr]; ts->contains[ctr]=NULL; }
	  if(ctr2 > 0)
	    ts->contains[ctr]=new TroopStats(tr->defstat[ctr2]);
	  Update(); break;
	  }
	}
      }
    }
  else if(x>=BAR_X) {
    if(y<32) {
      if(stat == VPSTAT_ATTACK) {
	if(PlanetaryCombat()) plan[planet]->SetOwner(player);
	plan[planet]->Deselect(); selx=-1; sely=-1;
	stat=VPSTAT_VIEW; Update();
	}
      else if(stat == VPSTAT_VIEW) {
	plan[planet]->Deselect(); selx=-1; sely=-1;
	stat=VPSTAT_ATTACK; Update();
	}
      }
    else if(y>=40 && stat!=VPSTAT_CONFIG && stat!=VPSTAT_VIEW){
      IntList which = screen->CollideRectangle(x, y, 1, 1);
      if(which.Size()>1 && which != tab[stat][tsel[stat]].Number()) {
	int ctr;
	for(ctr=tsel[stat]+1; ctr<tmax[stat]; ++ctr) {
	  if(which == tab[stat][ctr].Number()) {
	    tsel[stat]=ctr; Update(); break;
	    }
	  }
	if(ctr>=tmax[stat]) for(ctr=tsel[stat]-1; ctr>=0; --ctr) {
	  if(which == tab[stat][ctr].Number()) {
	    tsel[stat]=ctr; Update(); break;
	    }
	  }
	}
      }
    }
  else if(b==2) {
    x>>=5; y>>=5;
    if(stat==VPSTAT_BUILD) {
      if(plan[planet]->Building(x,y) != BUILDING_NONE && stat == VPSTAT_BUILD) {
	plan[planet]->PutBuilding(x, y, BUILDING_NONE, NULL);
	}
      }
//    else if(stat==VPSTAT_ATTACK) {
//      int del;
//      for(del=0; del<MAX_SHIPS*MAX_DROPSHIPS; ++del) {
//	if((x<<5)+16==attx[del] && (y<<5)+16==atty[del]) {
//	  break;
//	  }
//	}
//      if(del<MAX_SHIPS*MAX_DROPSHIPS) {
//	attx[del]=-1; atty[del]=-1; attsp[del]->Erase(); attpsp[del]->Erase();
//	}
//      }
    }
  else if(b==1) {
    x>>=5; y>>=5;
//    printf("selected %d,%d\n", x, y);
    if((plan[planet]->Building(x,y) == BUILDING_NONE || (x==selx && y==sely))
		|| (stat == VPSTAT_ATTACK)) {
      if(stat == VPSTAT_CONFIG) {
	stat=VPSTAT_BUILD; Update();
	}
      else if(stat == VPSTAT_BUILD) {
	int ctr; Troop *tr;  TroopStats *st=NULL;
	if(psel[stat][tsel[stat]]+poff[stat][tsel[stat]]) for(ctr=0; st==NULL && ctr<TROOP_MAX; ++ctr)
	  if(tr->defstat[ctr].cls==psel[stat][tsel[stat]]+poff[stat][tsel[stat]]) st=&tr->defstat[ctr];
	if(plan[planet]->Terrain(x, y) != TERRAIN_LAND) return;
	plan[planet]->PutBuilding(x, y, psel[stat][tsel[stat]]+poff[stat][tsel[stat]], st);
	if(psel[stat][tsel[stat]]+poff[stat][tsel[stat]] == BUILDING_MILITARY_BASE) {
	  plan[planet]->bt[x][y]->contains[0] = new TroopStats;
	  *(plan[planet]->bt[x][y]->contains[0]) = tr->defstat[TROOP_MARINE];
	  plan[planet]->bt[x][y]->contains[1] = new TroopStats;
	  *(plan[planet]->bt[x][y]->contains[1]) = tr->defstat[TROOP_MARINE];
	  plan[planet]->bt[x][y]->contains[2] = new TroopStats;
	  *(plan[planet]->bt[x][y]->contains[2]) = tr->defstat[TROOP_MARINE];
	  plan[planet]->bt[x][y]->contains[3] = new TroopStats;
	  *(plan[planet]->bt[x][y]->contains[3]) = tr->defstat[TROOP_MARINE];
	  }
	plan[planet]->items[x][y].SetImage(buildg[psel[stat][tsel[stat]]+poff[stat][tsel[stat]]][0]);
	plan[planet]->items[x][y].SetPriority(10000-(y<<2));
	plan[planet]->items[x][y].Move((x<<5)+16, (y<<5)+16);
	}
      else if(stat == VPSTAT_ATTACK) {
//	int ctr;
//	for(ctr=0; ctr<MAX_SHIPS*MAX_DROPSHIPS; ++ctr) {
//	  if((x<<5)+16==attx[ctr] && (y<<5)+16==atty[ctr]) {
//	    for(tsel[stat]=0; tsel[stat]<(tnum[stat]-1); ++tsel[stat])
//	      if(poff[stat][tsel[stat]+1] > ctr) break;
//	    psel[stat][tsel[stat]] = ctr-poff[stat][tsel[stat]];
//	    Update(); return;
//	    }
//	  }

	ship[player][tsel[stat]]->MoveDrop(psel[stat][tsel[stat]],
	  (x<<5)+16, (y<<5)+16);
	attsp[tsel[stat]][psel[stat][tsel[stat]]]->Move((x<<5)+16, (y<<5)+16);
	attpsp[tsel[stat]][psel[stat][tsel[stat]]]->
	  SetLine(BAR_X-((x<<5)+16),
	  (BAR_Y+(PANE_SPREAD*(psel[stat][tsel[stat]]))/pn)
	  -((y<<5)+16), 32, 0xFFFF0000);
	attpsp[tsel[stat]][psel[stat][tsel[stat]]]->
	  Move((x<<5)+16, (y<<5)+16);

	int ps=psel[stat][tsel[stat]];
	++ps; ps%=pnum[stat][tsel[stat]];
	for(; ps != psel[stat][tsel[stat]]; ++ps,ps%=pnum[stat][tsel[stat]])
	  if(ship[player][tsel[stat]]->DropX(ps)<0) break;
	if(ps == psel[stat][tsel[stat]]) {
	  ++tsel[stat]; tsel[stat]%=tnum[stat];
	  for(ps=0; ps<pnum[stat][tsel[stat]]; ++ps)
	    if(ship[player][tsel[stat]]->DropX(ps)<0) break;
	  }
	psel[stat][tsel[stat]] = ps;
	Update();
	}
      }
    else {
      selx=x; sely=y; stat=VPSTAT_CONFIG;
      plan[planet]->Select(selx, sely); Update();
      }
    }
  }

void ViewPort::Position(int xp, int yp)  {
  int x = xp>>5; int y = yp>>5;
//  if(x<0 || x>24 || y<0 || y>23) return;
  plan[planet]->Over(x, y);
  plan[planet]->Update();
  }

int ViewPort::PlanetaryCombat() {
Debug("PRE-PRE");
  int numtr[2]={0,0}, ctr, ctr2;
  Troop *tr; tr->InitTroopList();
Debug("PRE");
  for(ctr=0; ctr<MAX_SHIPS; ++ctr) {
    if(ship[player][ctr]) for(ctr2=0; ctr2<MAX_DROPSHIPS; ++ctr2) {
      if(ship[player][ctr]->DropX(ctr2)>=0) {
        new Troop(ship[player][ctr]->Drop(ctr2), 1, 
	  ship[player][ctr]->DropX(ctr2), ship[player][ctr]->DropY(ctr2));
        attsp[ctr][ctr2]->Erase();
        attpsp[ctr][ctr2]->Erase();
        }
      }
    }
  for(ctr=0; ctr<25; ++ctr) {
    for(ctr2=0; ctr2<24; ++ctr2) {
      if(plan[planet]->bt[ctr][ctr2]) {
	pdome[ctr][ctr2] = new Troop(plan[planet]->bt[ctr][ctr2], 0,
		(ctr<<5)+16, (ctr2<<5)+16);
	plan[planet]->items[ctr][ctr2].Erase();
	}
      else if(plan[planet]->Dome(ctr,ctr2)>=0) {
	pdome[ctr][ctr2] = new Troop(&tr->defstat[TROOP_DOME], 0,
		(ctr<<5)+16, (ctr2<<5)+16);
	plan[planet]->items[ctr][ctr2].Erase();
	}
      else {
	pdome[ctr][ctr2]=0;
	}
      }
    }
  int done=0;  ticker=0;
  screen->SetFrameRate(20);
  numtr[0] = tr->CountTeam(0);
  numtr[1] = tr->CountTeam(1);
  if(numtr[1]<=0 || numtr[0]<=0) done=100;
  while(done<100) {
Debug("BEGIN");
    tr->AllAct();
//    screen->RestoreInvalidRectangles();
//    screen->RestoreRectangle(0, 0, BAR_X, 768);
    if(done) ++done;
    else {
      numtr[0] = tr->CountTeam(0);
      numtr[1] = tr->CountTeam(1);
      if(numtr[0]<=0 || numtr[1]<=0) done=1;
      }
    screen->Refresh();
    ++ticker;
    }
Debug("POST");
  tr->DeleteAll();
//  screen->RestoreRectangle(0, 0, BAR_X, 768);
  for(ctr=0; ctr<MAX_SHIPS; ++ctr) {
    if(ship[player][ctr]) for(ctr2=0; ctr2<MAX_DROPSHIPS; ++ctr2) {
      if(ship[player][ctr]->DropX(ctr2)>=0) {
	ship[player][ctr]->MoveDrop(ctr2, -1, -1);
        attsp[ctr][ctr2]->Erase();
        attpsp[ctr][ctr2]->Erase();
        }
      }
    }
  for(ctr=0; ctr<25; ++ctr) {
    for(ctr2=0; ctr2<24; ++ctr2) {
      if(plan[planet]->Building(ctr,ctr2) != BUILDING_NONE) {
	plan[planet]->items[ctr][ctr2].Draw();
	}
      }
    }
  return (numtr[1]>0);
  }

void ViewPort::SavePlan(FILE *fl) {
  exit(1);
/*
  int ctr, tp1=-1, tp2=-1;
  for(ctr=0; ctr<MAX_SHIPS*MAX_DROPSHIPS; ++ctr) {
    if(attackers[ctr].contains[0] != NULL)
	tp1 = attackers[ctr].contains[0]->type;
    if(attackers[ctr].contains[1] != NULL)
	tp2 = attackers[ctr].contains[1]->type;
    fprintf(fl, "%d %d %d %d\n", attx[ctr], atty[ctr], tp1, tp2);
    }
*/
  }

void ViewPort::LoadPlan(FILE *fl) {
  int ctrs, ctrd, tp1, tp2, ax, ay;
  for(ctrs=0; ctrs<MAX_SHIPS; ++ctrs) {
    for(ctrd=0; ctrd<MAX_DROPSHIPS; ++ctrd) {
      int pn=max(pnum[VPSTAT_ATTACK][0]-1, 1);
      fscanf(fl, "%d %d %d %d\n", &ax, &ay, &tp1, &tp2);
      if(ship[player][ctrs]) {
	ship[player][ctrs]->MoveDrop(ctrd, ax, ay);
	if(ax) {
	  attsp[ctrs][ctrd]->Move(
	    ship[player][ctrs]->DropX(ctrd), ship[player][ctrs]->DropY(ctrd));
	  attpsp[ctrs][ctrd]->Erase();
	  attpsp[ctrs][ctrd]->SetLine(BAR_X-ship[player][ctrs]->DropX(ctrd),
	    (BAR_Y+(PANE_SPREAD*ctrd)/pn)
	    -ship[player][ctrs]->DropY(ctrd), 32, 0xFFFF0000);
	  attpsp[ctrs][ctrd]->Position(
	    ship[player][ctrs]->DropX(ctrd), ship[player][ctrs]->DropY(ctrd));
	  attpsp[ctrs][ctrd]->Remove();
	  }
	else {
	  attsp[ctrs][ctrd]->Erase();
	  attpsp[ctrs][ctrd]->Erase();
	  }
	}
      }
    }
  tsel[VPSTAT_ATTACK] = 0; otsel[VPSTAT_ATTACK] = 1; Update();
  }
