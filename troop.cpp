#include <math.h>

#include "troop.h"
#include "shot.h"

extern int ticker;
extern Troop *pdome[25][24];

TroopStats Troop::defstat[TROOP_MAX];

int Troop::maxtr;
Troop *Troop::trp[MAX_TROOPS];

#define getroll() ((rand()%1000)+1)

int miss(int misschance)  {
  int dist=0, roll=getroll();
  while(roll<misschance) { ++dist; roll<<=1; };
  return dist;
  }

void Troop::Add() {
  lnum = maxtr;
  if(maxtr >= MAX_TROOPS) U2_Exit(1, "Troop overflow!!\n");
  trp[maxtr++] = this;
  }

void Troop::DeleteAll() {
  Shot *tmpsh;
  tmpsh->DeleteAll();
  int ctr=0;
  for(ctr=0; ctr<maxtr; ++ctr) if (trp[ctr] != NULL) {
    delete trp[ctr];
    }
  }

void Troop::AllAct() {
  Shot *tmpsh;
  tmpsh->AllAct();
  int ctr=0;
  for(ctr=0; ctr<maxtr; ++ctr) if (trp[ctr] != NULL) {
    trp[ctr]->Act();
    }
  }

void Troop::InitTroopList() {
  Shot *tmpsh;
  tmpsh->InitShotList();
  int ctr, ctr2;
  maxtr=0;
  TroopStats *s;
  for(ctr=0; ctr<MAX_TROOPS; ++ctr) {
    trp[ctr] = NULL;
    }
  for(ctr=0; ctr<TROOP_MAX; ++ctr) {
    s=&defstat[ctr]; s->type=ctr;
    for(ctr2=0; ctr2<8; ++ctr2) s->contains[ctr2]=NULL;
    switch(ctr) {
      case(TROOP_DOME): {
	s->name="Non-combative Dome"; s->modnum=0;
	s->cls=BUILDING_DOME;
	s->wtype=WEAPON_NONE; s->mtype=MOVEMENT_DOME;
	s->armor=1000; s->men=0; s->maxmen=1000; s->dodge=0; s->speed=0;
	s->wdam=0; s->wmdam=0; s->wacc=0; s->wrng=0; s->wrof=0;
	}break;
      case(TROOP_HEAVY_LASERMASER): {
	s->name="Heavy Laser-Maser"; s->modnum=0;
	s->cls=BUILDING_HEAVY_LASERMASER;
	s->wtype=WEAPON_LMASER; s->mtype=MOVEMENT_DOME;
	s->armor=1000; s->men=1000; s->maxmen=1000; s->dodge=0; s->speed=0;
	s->wdam=10; s->wmdam=30; s->wacc=500; s->wrng=120; s->wrof=0;
	}break;
      case(TROOP_BALLISTIC_DEFENDER): {
	s->name="Ballistic Defender"; s->modnum=0;
	s->cls=BUILDING_BALLISTIC_DEFENDER;
	s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_DOME;
	s->armor=1000; s->men=1000; s->maxmen=1000; s->dodge=0; s->speed=0;
	s->wdam=250; s->wmdam=125; s->wacc=750; s->wrng=310; s->wrof=40;
	}break;
      case(TROOP_DROPSHIP): {
	s->name="Interatmospheric Dropship"; s->modnum=2;
	s->cls=-1;
	s->wtype=WEAPON_DEPLOY; s->mtype=MOVEMENT_DROP;
	s->armor=500; s->men=1000; s->maxmen=1000; s->dodge=0; s->speed=0;
	s->wdam=0; s->wmdam=0; s->wacc=0; s->wrng=0; s->wrof=0;
	}break;
      case(TROOP_BASE): {
	s->name="Military Base"; s->modnum=4;
	s->cls=BUILDING_MILITARY_BASE;
	s->wtype=WEAPON_DEPLOY; s->mtype=MOVEMENT_DOME;
	s->armor=1000; s->men=1000; s->maxmen=1000; s->dodge=0; s->speed=0;
	s->wdam=0; s->wmdam=0; s->wacc=0; s->wrng=0; s->wrof=0;
	}break;
      case(TROOP_MARINE): {
	s->name="Marine Platoon"; s->modnum=4;
	s->cls=-1; s->wtype=WEAPON_GUN; s->mtype=MOVEMENT_WALK;
	s->armor=10; s->men=200; s->maxmen=200; s->dodge=500; s->speed=100;
	s->wdam=1; s->wmdam=20; s->wacc=800; s->wrng=45; s->wrof=10;
	}break;
      case(TROOP_ARTILLIARY): {
	s->name="Artilliary Squad"; s->modnum=4;
	s->cls=-1; s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_WALK;
	s->armor=10; s->men=200; s->maxmen=200; s->dodge=500; s->speed=100;
	s->wdam=10; s->wmdam=10; s->wacc=500; s->wrng=100; s->wrof=20;
	}break;
      case(TROOP_BTANK): {
	s->name="Ballistic Tank"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_DRIVE;
	s->armor=100; s->men=100; s->maxmen=100; s->dodge=250; s->speed=50;
	s->wdam=75; s->wmdam=35; s->wacc=600; s->wrng=120; s->wrof=40;
	}break;
      case(TROOP_LTANK): {
	s->name="Laser Tank"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_LASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=100; s->men=100; s->maxmen=100; s->dodge=250; s->speed=50;
	s->wdam=5; s->wmdam=7; s->wacc=400; s->wrng=100; s->wrof=0;
	}break;
      case(TROOP_MTANK): {
	s->name="Maser Tank"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_MASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=100; s->men=100; s->maxmen=100; s->dodge=250; s->speed=50;
	s->wdam=5; s->wmdam=10; s->wacc=400; s->wrng=80; s->wrof=0;
	}break;
      case(TROOP_BHEAVYTANK): {
	s->name="Heavy Ballistic Tank"; s->modnum=1;
	s->cls=-1; s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_DRIVE;
	s->armor=200; s->men=100; s->maxmen=100; s->dodge=100; s->speed=25;
	s->wdam=250; s->wmdam=125; s->wacc=700; s->wrng=240; s->wrof=40;
	}break;
      case(TROOP_LHEAVYTANK): {
	s->name="Heavy Laser Tank"; s->modnum=0;
	s->cls=-1; s->wtype=WEAPON_LASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=200; s->men=100; s->maxmen=100; s->dodge=100; s->speed=25;
	s->wdam=10; s->wmdam=9; s->wacc=500; s->wrng=200; s->wrof=0;
	}break;
      case(TROOP_MHEAVYTANK): {
	s->name="Heavy Maser Tank"; s->modnum=1;
	s->cls=-1; s->wtype=WEAPON_MASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=200; s->men=100; s->maxmen=100; s->dodge=100; s->speed=25;
	s->wdam=10; s->wmdam=12; s->wacc=500; s->wrng=160; s->wrof=0;
	}break;
      case(TROOP_BMECH): {
	s->name="Ballistic Mech"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_DRIVE;
	s->armor=200; s->men=50; s->maxmen=50; s->dodge=100; s->speed=30;
	s->wdam=75; s->wmdam=35; s->wacc=600; s->wrng=120; s->wrof=40;
	}break;
      case(TROOP_LMECH): {
	s->name="Laser Mech"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_LASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=200; s->men=50; s->maxmen=50; s->dodge=100; s->speed=30;
	s->wdam=5; s->wmdam=7; s->wacc=400; s->wrng=100; s->wrof=0;
	}break;
      case(TROOP_MMECH): {
	s->name="Maser Mech"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_MASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=200; s->men=50; s->maxmen=50; s->dodge=100; s->speed=30;
	s->wdam=5; s->wmdam=10; s->wacc=400; s->wrng=80; s->wrof=0;
	}break;
      case(TROOP_BHEAVYMECH): {
	s->name="Heavy Ballistic Mech"; s->modnum=1;
	s->cls=-1; s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_DRIVE;
	s->armor=300; s->men=50; s->maxmen=50; s->dodge=50; s->speed=15;
	s->wdam=250; s->wmdam=125; s->wacc=700; s->wrng=240; s->wrof=40;
	}break;
      case(TROOP_LHEAVYMECH): {
	s->name="Heavy Laser Mech"; s->modnum=1;
	s->cls=-1; s->wtype=WEAPON_LASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=300; s->men=50; s->maxmen=50; s->dodge=50; s->speed=15;
	s->wdam=10; s->wmdam=9; s->wacc=500; s->wrng=200; s->wrof=0;
	}break;
      case(TROOP_MHEAVYMECH): {
	s->name="Heavy Maser Mech"; s->modnum=1;
	s->cls=-1; s->wtype=WEAPON_MASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=300; s->men=50; s->maxmen=50; s->dodge=50; s->speed=15;
	s->wdam=10; s->wmdam=12; s->wacc=500; s->wrng=160; s->wrof=0;
	}break;
      case(TROOP_BATV): {
	s->name="Ballistic ATV"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_BALLISTIC; s->mtype=MOVEMENT_DRIVE;
	s->armor=50; s->men=50; s->maxmen=50; s->dodge=900; s->speed=500;
	s->wdam=75; s->wmdam=35; s->wacc=400; s->wrng=100; s->wrof=40;
	}break;
      case(TROOP_LATV): {
	s->name="Laser ATV"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_LASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=50; s->men=50; s->maxmen=50; s->dodge=900; s->speed=500;
	s->wdam=5; s->wmdam=7; s->wacc=200; s->wrng=85; s->wrof=0;
	}break;
      case(TROOP_MATV): {
	s->name="Maser ATV"; s->modnum=2;
	s->cls=-1; s->wtype=WEAPON_MASER; s->mtype=MOVEMENT_DRIVE;
	s->armor=50; s->men=50; s->maxmen=50; s->dodge=900; s->speed=500;
	s->wdam=5; s->wmdam=10; s->wacc=200; s->wrng=70; s->wrof=0;
	}break;
      default: {
	s->name="Nothing"; s->modnum=0;
	s->cls=-1; s->wtype=WEAPON_NONE; s->mtype=MOVEMENT_NONE;
	s->armor=0; s->men=0; s->maxmen=0; s->dodge=0; s->speed=0;
	s->wdam=0; s->wmdam=0; s->wacc=0; s->wrng=0; s->wrof=0;
	}break;
      }
    }
  }

int Troop::CountTeam(int t) {
  int ctr, ret=0;
  for(ctr=0; ctr<maxtr; ctr++) if(trp[ctr]->side==t) {
    if(trp[ctr]->type != TROOP_NONE && trp[ctr]->men>0 && trp[ctr]->armor>0)
      ++ret;
    }
  return ret;
  }

void Troop::Erase() {
  if(cls>=0) pdome[xp>>5][yp>>5] = 0;
  if(wsnd>=0) { spk->Stop(wsnd); wsnd = -1; }
  image.Erase();
//  image.Remove();
  if(wtype == WEAPON_DEPLOY) {
    int ctr;
    for(ctr=0; ctr<maxtr; ctr++) {
      if(trp[ctr]->parent==this && trp[ctr]->xp<0) {
	trp[ctr]->Erase();
	}
      }
    }
  type = TROOP_NONE;
  }

Troop::~Troop() {
  Erase();
  }

int Troop::Hit(int wd, int md) {
  if(type == TROOP_NONE || armor<1) return 0;
  if(men>0) {
    men-=(md*md)/armor;
    if(men<0) men=0;
    }
  if(armor>0 && type != TROOP_MARINE && type != TROOP_ARTILLIARY) {
    armor -= wd;
    if(armor<0) armor=0;
    }
  if(armor<1) {
/*
    switch(type) {
//      case(TROOP_MARINE): sound[SOUND_DEATH]->Play(); break;
      case(TROOP_DOME):
      case(TROOP_HEAVY_LASERMASER):
      case(TROOP_BALLISTIC_DEFENDER):
      case(TROOP_BASE):
	sound[SOUND_DOMEDIE]->Play(); break;
      }
*/
    if(cls>=0) {
      sound[SOUND_DOMEDIE]->Play();
      image.UseImage(dmdth[0]);
      image.Draw();
      armor=0;
      }
    else Erase();
    return 1;
    }
  if(men<1) {
    switch(type) {
      case(TROOP_MARINE):
      case(TROOP_ARTILLIARY): {
	Erase();
	}break;
      }
    }
  return 0;
  }

Troop::Troop(TroopStats *st, int t, Troop *p) {
  parent=p; xp=-1; yp=-1; zp=0;
  if(parent!=NULL) parent->wdam++;
  Init(st, t);
  }

Troop::Troop(TroopStats *st, int t, int x, int y) {
  parent=NULL; xp=x; yp=y; zp=0;
  Init(st, t);
  }

void Troop::Init(TroopStats *st, int t) {
  shot=NULL;
  if(st==NULL || st->type>MAX_TROOPS || st->type<0)
	U2_Exit(1, "Null troopstats!\n");
  target=-1; stat=0; wstat=0; wtm=0; wsnd=-1; tsnd=-1;
  axp=(double)xp; ayp=(double)yp;
  side=t;

  type=st->type; cls=st->cls;
  armor=st->armor; men=st->men; maxmen=st->maxmen;
  wtype=st->wtype; wrof=st->wrof; wrng=st->wrng;
  wdam=st->wdam; wmdam=st->wmdam; wacc=st->wacc;
  mtype=st->mtype; speed=st->speed; dodge=st->dodge;

  if(cls>=0) image.UseImage(*buildg[cls][0]);
  else image.UseImage(*troopg[type]);

  if(mtype==MOVEMENT_DROP) { stat=100; zp=100; }

  Add();

  if(st->wtype==WEAPON_DEPLOY) {
    for(int ctr=0; ctr<st->modnum; ++ctr) if(st->contains[ctr]) {
      for(int ctr2=0; ctr2<st->contains[ctr]->modnum; ++ctr2) {
	new Troop(st->contains[ctr], t, this);
	}
      }
    }
  }

void Troop::Act() {
  if(xp < 0 || type == TROOP_NONE) return;
  if(armor<1) {
    armor--;
    if(armor > -17) {
      image.UseImage(dmdth[-armor]);
      image.Draw();
      }
    else Erase();
    return;
    }
  if(target >= 0 && (trp[target]->type == TROOP_NONE || trp[target]->xp<0
	|| (stat < 1000 && trp[target]->men<=0) || trp[target]->armor<=0))
    target = -1;
  switch(wtype) {
    default: {
      U2_Exit(-1, "Unknown WTYPE (%d)!\n", wtype);
      }break;
    case(WEAPON_NONE): break;
    case(WEAPON_LMASER):
    case(WEAPON_MASER):
    case(WEAPON_LASER):
    case(WEAPON_GUN): {
      if(wtm>0) wtm--;
      if(men<=0 || (target >= 0 && trp[target]->stat >= 1000))
	target = -1;
      if(men>0 && target<0) {
	int ctr2, xd, yd, tdist, dist=1024*1024+1;
	for(ctr2=0; ctr2<maxtr; ctr2++)  if(trp[ctr2]->side==(1-side)) {
	  if(trp[ctr2]->type != TROOP_NONE && trp[ctr2]->stat < 1000
		&& trp[ctr2]->xp>=0 && trp[ctr2]->men > 0
		&& trp[ctr2]->armor > 0) {
	    xd = (trp[ctr2]->xp)-(xp);
	    yd = (trp[ctr2]->yp)-(yp);
	    tdist=xd*xd+yd*yd;
	    if(tdist<dist) { dist=tdist; target=ctr2; }
	    }
	  }
	}
      if(target>=0 && mtype != MOVEMENT_DOME
		&& trp[target]->mtype != MOVEMENT_DOME) {
	int xd = (trp[target]->xp)-(xp);
	int yd = (trp[target]->yp)-(yp);
	int dst = (int)sqrt(xd*xd+yd*yd);
	if(dst<=wrng && trp[target]->zp==0) {
	  int acc = (wacc*(maxmen+men))/(maxmen+maxmen);
	  int chms = 1000-(((1000-(trp[target]->dodge*dst)/wrng)*acc)/1000);
	  if(shot != NULL) shot->Target(xp, yp, trp[target], miss(chms), wrof);
	  else switch(wtype) {
	    case(WEAPON_GUN): {
	      shot=new Shot(SHOT_GUN, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    case(WEAPON_LASER): {
	      shot=new Shot(SHOT_LASER, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    case(WEAPON_MASER): {
	      shot=new Shot(SHOT_MASER, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    case(WEAPON_LMASER): {
	      shot=new Shot(SHOT_LASERMASER, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    }
	  if(shot->TargetDead()) { target=-1; }
	  }
	}
      else if(target>=0 && mtype == MOVEMENT_DOME && stat>15) {
	int nangle, angle = (stat-16)&63;
	int xd = (trp[target]->xp)-(xp);
	int yd = (trp[target]->yp)-(yp);
	double tang = atan2(-xd, yd); tang+=M_PI;
	tang *= 32.0 /*64/2*/; tang /= M_PI;
	nangle = (int)tang;
	nangle &= 63;
	if(nangle<angle) {
	  if(nangle+32>angle) angle+=63;
	  else angle++;
	  }
	else if(nangle>angle) {
	  if(angle+32>nangle) angle++;
	  else angle+=63;
	  }
	angle &= 63;
	int tdist=xd*xd+yd*yd;
	if(angle == nangle && tdist <= wrng*wrng && trp[target]->zp==0) {
	  int dst = (int)sqrt(xd*xd+yd*yd);
	  int acc = (wacc*(maxmen+men))/(maxmen+maxmen);
	  int chms = 1000-(((1000-(trp[target]->dodge*dst)/wrng)*acc)/1000);
	  if(shot != NULL) shot->Target(xp, yp, trp[target], miss(chms), wrof);
	  else switch(wtype) {
	    case(WEAPON_GUN): {
	      shot=new Shot(SHOT_GUN, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    case(WEAPON_LASER): {
	      shot=new Shot(SHOT_LASER, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    case(WEAPON_MASER): {
	      shot=new Shot(SHOT_MASER, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    case(WEAPON_LMASER): {
	      shot=new Shot(SHOT_LASERMASER, wdam, wmdam, xp, yp,
		trp[target], miss(chms), wrof);
	      }break;
	    }
	  if(shot->TargetDead()) { target=-1; }
	  stat = angle+80;
	  }
	else {
	  stat = angle+16;
	  }
	}
      else if(mtype == MOVEMENT_DOME && target>=0 && stat<16) {
	if(stat == 0) sound[SOUND_OPEN]->Play();
	stat++;
	}
      else if(mtype == MOVEMENT_DOME && target<0 && stat>0) {
	if(stat < 48) stat--;
	else if(stat < 80) stat++;
	else if(stat == 80) stat=16;
	else if(stat < 112) stat-=65;
	else if(stat < 143) stat-=63;
	else stat=16;
	}
      else {
	if(wsnd>=0) {
	  sound[SOUND_LASER]->Stop(wsnd);
	  wsnd = -1;
	  }
	}
      }break;
    case(WEAPON_BALLISTIC): {
      if(wtm>0) wtm--;
      if(men<=0 || (target >= 0 && trp[target]->stat >= 1000))
	target = -1;
      if(men>0 && target<0) {
	int ctr2, xd, yd, tdist, dist=1024*1024+1;
	for(ctr2=0; ctr2<maxtr; ctr2++)  if(trp[ctr2]->side==(1-side)) {
	  if(trp[ctr2]->type != TROOP_NONE && trp[ctr2]->stat < 1000
		&& trp[ctr2]->xp>=0 && trp[ctr2]->men > 0
		&& trp[ctr2]->armor > 0) {
	    xd = (trp[ctr2]->xp)-(xp);
	    yd = (trp[ctr2]->yp)-(yp);
	    tdist=xd*xd+yd*yd;
	    if(tdist<dist) { dist=tdist; target=ctr2; }
	    }
	  }
	}
      if(target>=0 && mtype != MOVEMENT_DOME && wtm<1) {
	int xd = (trp[target]->xp)-(xp);
	int yd = (trp[target]->yp)-(yp);
	int dst = (int)sqrt(xd*xd+yd*yd);
	if(dst<=wrng && trp[target]->zp==0) {
	  int chms = 1000-(((1000-(trp[target]->dodge*dst)/wrng)*wacc)/1000);
	  new Shot(SHOT_BALLISTIC, wdam, wmdam, xp, yp,
		trp[target], 1+miss(chms), wrof);
	  wtm=(wrof*(maxmen+maxmen))/(maxmen+men);
	  }
	}
      else if(target>=0 && mtype == MOVEMENT_DOME && stat>15) {
	int nangle, angle = stat-16;
	int xd = (trp[target]->xp)-(xp);
	int yd = (trp[target]->yp)-(yp);
	double tang = atan2(-xd, yd); tang+=M_PI;
	tang *= 32.0 /*64/2*/; tang /= M_PI;
	nangle = (int)tang;
	nangle &= 63;
	if(nangle<angle) {
	  if(nangle+32>angle) angle+=63;
	  else angle++;
	  }
	else if(nangle>angle) {
	  if(angle+32>nangle) angle++;
	  else angle+=63;
	  }
	angle &= 63;
	int tdist=xd*xd+yd*yd;
	if(angle == nangle && tdist <= wrng*wrng && wtm<1 && trp[target]->zp==0) {
	  int dst = (int)sqrt(xd*xd+yd*yd);
	  int chms = 1000-(((1000-(trp[target]->dodge*dst)/wrng)*wacc)/1000);
	  new Shot(SHOT_BALLISTIC, wdam, wmdam, xp, yp,
		trp[target], 1+miss(chms), wrof);
	  wtm=(wrof*(maxmen+maxmen))/(maxmen+men);
	  stat = angle+80;
	  }
	else stat = angle+16;
	}
      else if(mtype == MOVEMENT_DOME && target>=0 && stat<16) {
	if(stat == 0) sound[SOUND_OPEN]->Play();
	stat++;
	}
      else if(mtype == MOVEMENT_DOME && target<0 && stat>0) {
	if(stat < 48) stat--;
	else if(stat < 80) stat++;
	else if(stat == 80) stat=16;
	else if(stat < 112) stat-=65;
	else if(stat < 143) stat-=63;
	else stat=16;
	}
      }break;
    case(WEAPON_DEPLOY): {
      if(mtype == MOVEMENT_DROP && men > 0 && stat > 0) {
	--stat; zp=(stat*stat*stat)/10000;
	if(stat==50 && pdome[xp>>5][yp>>5]) pdome[xp>>5][yp>>5]->Hit(10000, 0);
	else if(stat==21) pdome[xp>>5][yp>>5]=this;
	}
      else if(mtype == MOVEMENT_DROP && men <= 0) {
	if(stat>=100) Erase();
	else { ++stat; zp=(stat*stat*stat)/10000; }
	if(stat==22) pdome[xp>>5][yp>>5]=NULL;
	}
      else if(mtype == MOVEMENT_DOME && men > 0 && stat < 16) {
	if(!stat) sound[SOUND_OPEN]->Play();
	++stat;
	}
      else if(mtype == MOVEMENT_DOME && men <= 0) {
	if(stat) --stat;
	}
      else if((ticker%DROPSHIP_DELAY) == (lnum%DROPSHIP_DELAY)) {
	if(wdam > 0) {
	  int numd=1, ctr, drop=-1;
	  for(ctr=0; ctr<maxtr && drop<0; ++ctr) {
	    if(trp[ctr]->parent == this && trp[ctr]->xp<0) {
	      numd=defstat[trp[ctr]->type].modnum; drop=ctr; break;
	      }
	    }
	  trp[drop]->xp = xp;
	  trp[drop]->yp = yp+16;
	  trp[drop]->axp=(double)trp[drop]->xp;
	  trp[drop]->ayp=(double)trp[drop]->yp;
	  trp[drop]->doing = 0;
	  --(wdam);
	  if(numd>1) {
	    for(ctr=drop,drop=-1; ctr<maxtr && drop<0; ++ctr)
	      if(trp[ctr]->parent == this && trp[ctr]->xp<0) drop=ctr;
	    trp[drop]->xp = xp;
	    trp[drop]->yp = yp-16;
	    trp[drop]->axp=(double)trp[drop]->xp;
	    trp[drop]->ayp=(double)trp[drop]->yp;
	    trp[drop]->doing = 0;
	    --(wdam);
	    }
	  if(numd>2) {
	    for(ctr=drop,drop=-1; ctr<maxtr && drop<0; ++ctr)
	      if(trp[ctr]->parent == this && trp[ctr]->xp<0) drop=ctr;
	    trp[drop]->xp = xp+16;
	    trp[drop]->yp = yp;
	    trp[drop]->axp=(double)trp[drop]->xp;
	    trp[drop]->ayp=(double)trp[drop]->yp;
	    trp[drop]->doing = 0;
	    --(wdam);
	    for(ctr=drop,drop=-1; ctr<maxtr && drop<0; ++ctr)
	      if(trp[ctr]->parent == this && trp[ctr]->xp<0) drop=ctr;
	    trp[drop]->xp = xp-16;
	    trp[drop]->yp = yp;
	    trp[drop]->axp=(double)trp[drop]->xp;
	    trp[drop]->ayp=(double)trp[drop]->yp;
	    trp[drop]->doing = 0;
	    --(wdam);
	    }
	  if(wdam<1) men=0;
	  }
	else men=0;
	}
      }break;
    }
  switch(mtype) {
    default: {
      U2_Exit(-1, "Unknown MTYPE (%d)!\n", mtype);
      }break;
    case(MOVEMENT_NONE): break;
    case(MOVEMENT_DRIVE): {
      if(xp>=0 && men>0 && armor>0 && target >= 0) {
	int xd = (trp[target]->xp)-(xp);
	int yd = (trp[target]->yp)-(yp);
	if(xd*xd+yd*yd >= (wrng)*(wrng)) {
	  double oxp=axp, oyp=ayp;
	  double axd = ((double)trp[target]->xp)-(axp);
	  double ayd = ((double)trp[target]->yp)-(ayp);
	  double dst=(double)speed/100.0;
	  double dx=dst*cos(atan2(ayd, axd));
	  double dy=dst*sin(atan2(ayd, axd));
	  axp+=dx; ayp+=dy;

	  int txp=(int)axp, typ=(int)ayp;
	  { //Just to make life easier
	    if(pdome[txp>>5][typ>>5]) {
	      double dxp=(double)pdome[txp>>5][typ>>5]->xp;
	      double dyp=(double)pdome[txp>>5][typ>>5]->yp;
	      double xrad=(axp-dxp), yrad=(ayp-dyp);
	      double rad = sqrt(xrad*xrad+yrad*yrad);
	      if(rad<16.0) {
		double oxrad=(oxp-dxp), oyrad=(oyp-dyp);
		double iaxp=dxp+16.0*cos(atan2(oyrad, oxrad));
		double iayp=dyp+16.0*sin(atan2(oyrad, oxrad));
		double nxrad=(iaxp-dxp), nyrad=(iayp-dyp);
#define sq(x) ((x)*(x))
		double ldst=dst-sqrt(sq(iaxp-oxp)+sq(iayp-oyp));
#undef sq(x)
		if(ldst>0.0) {
		  ldst /= 16.0;
		  if((atan2(yrad, xrad) > atan2(oyrad, oxrad)
			&& atan2(yrad, xrad) < M_PI+atan2(oyrad, oxrad))
			|| atan2(yrad, xrad)+M_PI < atan2(oyrad, oxrad)) {
		    iaxp=dxp+16.0*cos(atan2(nyrad, nxrad)+ldst);
		    iayp=dyp+16.0*sin(atan2(nyrad, nxrad)+ldst);
		    }
		  else {
		    iaxp=dxp+16.0*cos(atan2(nyrad, nxrad)-ldst);
		    iayp=dyp+16.0*sin(atan2(nyrad, nxrad)-ldst);
		    }
		  }
		axp=iaxp; ayp=iayp;
		txp=(int)axp; typ=(int)ayp;
		}
	      }
	    } //Just to make life easier
	  xp=txp; yp=typ;
	  }
	}
      }break;
    case(MOVEMENT_WALK): {
      if(stat < 1000 && target >= 0 && trp[target]->mtype==MOVEMENT_DOME) {
	int xd, yd, dst2;
	xd = (trp[target]->xp)-(xp);
	yd = (trp[target]->yp)-(yp);
	dst2=xd*xd+yd*yd;
	if(dst2<256) {
//	  image.Remove();
	  image.Erase();
	  stat = 1000;
	  if(wsnd<0) wsnd = sound[SOUND_GUN]->Loop();
	  }
	}
      if(stat >= 1000) {
	if(target < 0 || trp[target]->type == TROOP_NONE) {
	  Erase();
//	  numtr[side]--;
	  }
	else {
	  if(trp[target]->men > 0) {
	    trp[target]->men-=wmdam;
	    if(trp[target]->men <= 0) {
	      trp[target]->men = 0;
//	      numtr[1-side]--;
	      }
	    }
	  if(trp[target]->men <= 0) {
	    stat = 0;
	    target = -1;
	    doing = 0;
	    if(wsnd>=0) {
              sound[SOUND_GUN]->Stop(wsnd);
              wsnd = -1;
              }
	    }
	  }
	break;
	}
      else if(xp>=0 && men>0 && armor>0) {
	if(target < 0) {
	  int ctr2, xd, yd, tdist, dist=1024*1024*2;
	  for(ctr2=0; ctr2<maxtr; ctr2++) {
	    if(trp[ctr2]->side==(1-side) && trp[ctr2]->men>0) {
	      if(trp[ctr2]->type != TROOP_NONE && trp[ctr2]->men > 0) {
		xd = (trp[ctr2]->xp)-(xp);
		yd = (trp[ctr2]->yp)-(yp);
		tdist=xd*xd+yd*yd;
		if(tdist<dist) { dist=tdist; target=ctr2; }
		}
	      }
	    }
	  }
	if(target>=0) {
	  int xd = (trp[target]->xp)-(xp);
	  int yd = (trp[target]->yp)-(yp);
	  if(trp[target]->mtype==MOVEMENT_DOME
		|| xd*xd+yd*yd >= (wrng)*(wrng)) {
	    double oxp=axp, oyp=ayp;
	    double axd = ((double)trp[target]->xp)-(axp);
	    double ayd = ((double)trp[target]->yp)-(ayp);
	    double dst=(double)speed/100.0;
	    double dx=dst*cos(atan2(ayd, axd));
	    double dy=dst*sin(atan2(ayd, axd));
	    axp+=dx; ayp+=dy;

	    int txp=(int)axp, typ=(int)ayp;
	    if(pdome[txp>>5][typ>>5]) {
	      double dxp=(double)pdome[txp>>5][typ>>5]->xp;
	      double dyp=(double)pdome[txp>>5][typ>>5]->yp;
	      double xrad=(axp-dxp), yrad=(ayp-dyp);
	      double rad = sqrt(xrad*xrad+yrad*yrad);
	      if(rad<16.0) {
		double oxrad=(oxp-dxp), oyrad=(oyp-dyp);
		double iaxp=dxp+16.0*cos(atan2(oyrad, oxrad));
		double iayp=dyp+16.0*sin(atan2(oyrad, oxrad));
		double nxrad=(iaxp-dxp), nyrad=(iayp-dyp);
#define sq(x) ((x)*(x))
		double ldst=dst-sqrt(sq(iaxp-oxp)+sq(iayp-oyp));
#undef sq(x)
		if(ldst>0.0) {
		  ldst /= 16.0;
		  if((atan2(yrad, xrad) > atan2(oyrad, oxrad)
			&& atan2(yrad, xrad) < M_PI+atan2(oyrad, oxrad))
			|| atan2(yrad, xrad)+M_PI < atan2(oyrad, oxrad)) {
		    iaxp=dxp+16.0*cos(atan2(nyrad, nxrad)+ldst);
		    iayp=dyp+16.0*sin(atan2(nyrad, nxrad)+ldst);
		    }
		  else {
		    iaxp=dxp+16.0*cos(atan2(nyrad, nxrad)-ldst);
		    iayp=dyp+16.0*sin(atan2(nyrad, nxrad)-ldst);
		    }
		  }
		axp=iaxp; ayp=iayp;
		txp=(int)axp; typ=(int)ayp;
		}
	      }
	    xp=txp; yp=typ;
	    }
	  }
        }
      }break;
    case(MOVEMENT_DOME): {
      }break;
    case(MOVEMENT_DROP): {
      }break;
    }
  if(type != TROOP_NONE && stat < 1000) {
    if(cls<0) image.Move(xp, yp-zp);
//    if(cls==0) image.Position(xp, yp-zp);
    else {
      image.UseImage(*buildg[cls][stat]);
      image.Move(xp, yp-zp);
//      image.Position(xp, yp-zp);
      }
    }
  }
