#include <string.h>

#include "cosmos.h"
#include "planet.h"
#include "troop.h"

Planet::Planet(FILE *fl) {
  PreInit();
  Load(fl);
  Init();
  }

Planet::Planet(const char *r, const char *n, int o) {
  PreInit();
  sprintf(rfn, "%s%c", r, 0);
  sprintf(name, "%s%c", n, 0);
  owner = o;
  Init();
  }

void Planet::PreInit() {
  int ctr, ctr2;
  discovered = 0;
  selx=-1; sely=-1;
  for(ctr=0; ctr<25; ++ctr) {
    for(ctr2=0; ctr2<24; ++ctr2) {
      build[ctr][ctr2] = BUILDING_NONE;
      dome[ctr][ctr2] = -1;
      update[ctr][ctr2] = 0;
      bt[ctr][ctr2] = NULL;
      }
    }
  }

void Planet::Init() {
  ResFile rf(rfn);
  stats = rf.GetCharBag();
  image = rf.GetGraphic();
  screen->MakeFriendly(image);
  }

Planet::~Planet() {
  int x, y;
  for(x=0; x<25; ++x) for(y=0; y<24; ++y) if(bt[x][y] != NULL) delete bt[x][y];
  }

void Planet::PutBuilding(int x, int y, int b, TroopStats *ts) {
  dome[x][y] = 0;
  build[x][y] = b;
  if(bt[x][y] != NULL) delete bt[x][y];
  if(ts!=NULL) bt[x][y] = new TroopStats(*ts);
  else bt[x][y] = NULL;
  if(b == BUILDING_NONE) {
    update[x][y] = 1;
    dome[x][y] = -1;
    }
  }

void Planet::Deselect() {
  if(selx>=0) update[selx][sely]=1;
  selx=-1; sely=-1;
  }

void Planet::Select(int x, int y) {
  if(player!=owner) return;
  selx=x; sely=y;
  update[x][y]=1;
  }

void Planet::Over(int x, int y) {
  if(player==owner && x<=24 && y<=23 && dome[x][y]>=0) { ovrx=x; ovry=y; }
  else { ovrx=-1; ovry=-1; }
  }

void Planet::Update() {
  int ctr, ctr2;
  for(ctr=0; ctr<25; ++ctr) {
    for(ctr2=0; ctr2<24; ++ctr2) {
      if(ctr==selx && ctr2==sely) {
	++dome[selx][sely];
	if(dome[selx][sely]>=80) dome[selx][sely]=16;
	update[selx][sely]=1;
	}
      else if(ctr==ovrx && ctr2==ovry) {
	++dome[ovrx][ovry];
	if(dome[ovrx][ovry]>=80) dome[ovrx][ovry]=16;
	update[ovrx][ovry]=1;
	}
      else if(dome[ctr][ctr2] > 0) {
	if(dome[ctr][ctr2] < 48) --dome[ctr][ctr2];
	else { ++dome[ctr][ctr2]; if(dome[ctr][ctr2]>=80) dome[ctr][ctr2]=16; }
	update[ctr][ctr2]=1;
	}
      if(update[ctr][ctr2]) {
	if(Building(ctr,ctr2)!=BUILDING_NONE) {
	  items[ctr][ctr2].UseImage(buildg[Building(ctr,ctr2)]
                [Dome(ctr,ctr2)]);
	  items[ctr][ctr2].Move((ctr<<5)+16, (ctr2<<5)+16);
	  }
	else {
	  items[ctr][ctr2].Erase();
	  }
	update[ctr][ctr2]=0;
        }
      }
    }
  }

void Planet::Save(FILE *fl) {
  int ctrx, ctry;
  fprintf(fl, "%s\n", rfn);
  fprintf(fl, "%s\n", name);
  fprintf(fl, "%d\n", owner);
  for(ctrx=0; ctrx<25; ++ctrx) {
    for(ctry=0; ctry<24; ++ctry) {
      fprintf(fl, "%d %d", dome[ctrx][ctry], build[ctrx][ctry]);
      if((!bt[ctrx][ctry]) || build[ctrx][ctry]!=BUILDING_MILITARY_BASE) {
	if(build[ctrx][ctry]==BUILDING_MILITARY_BASE)
	  Exit(1, "Base without troops!\n");
	fprintf(fl, "\n");
	}
      else {
	int tp1=-1, tp2=-1, tp3=-1, tp4=-1;
	if(bt[ctrx][ctry]->contains[0]) tp1=bt[ctrx][ctry]->contains[0]->type;
	if(bt[ctrx][ctry]->contains[1]) tp2=bt[ctrx][ctry]->contains[1]->type;
	if(bt[ctrx][ctry]->contains[2]) tp3=bt[ctrx][ctry]->contains[2]->type;
	if(bt[ctrx][ctry]->contains[3]) tp4=bt[ctrx][ctry]->contains[3]->type;
	fprintf(fl, " %d %d %d %d\n", tp1, tp2, tp3, tp4);
	}
      }
    }
  }

void Planet::Load(FILE *fl) {
  int ctrx, ctry, tp, ctr;
  Troop *tr=NULL;
  TroopStats *st;
  memset(rfn, 0, MAX_PLANET_NAME);
  memset(name, 0, MAX_PLANET_NAME);
  fscanf(fl, "%s\n", rfn);
  fscanf(fl, "%s\n", name);
  fscanf(fl, "%d\n", &owner);
  for(ctrx=0; ctrx<25; ++ctrx) {
    for(ctry=0; ctry<24; ++ctry) {
      st=NULL;
      int tp1=-1, tp2=-1, tp3=-1, tp4=-1;
      fscanf(fl, "%d %d", &dome[ctrx][ctry], &tp);
      if(tp>0) for(ctr=0; st==NULL && ctr<TROOP_MAX; ++ctr) {
        if(tr->defstat[ctr].cls==tp) st=&tr->defstat[ctr];
	}
      PutBuilding(ctrx, ctry, tp, st);
      if(tp!=BUILDING_MILITARY_BASE) fscanf(fl, "\n");
      else {
	fscanf(fl, " %d %d %d %d\n", &tp1, &tp2, &tp3, &tp4);
	bt[ctrx][ctry]->contains[0] = new TroopStats(tr->defstat[tp1]);
	bt[ctrx][ctry]->contains[1] = new TroopStats(tr->defstat[tp2]);
	bt[ctrx][ctry]->contains[2] = new TroopStats(tr->defstat[tp3]);
	bt[ctrx][ctry]->contains[3] = new TroopStats(tr->defstat[tp4]);
	}
      update[ctrx][ctry]=1;
      }
    }
  }
