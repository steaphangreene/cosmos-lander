#include <math.h>

#include "shot.h"
#include "troop.h"

extern int ticker;

int Shot::maxsh;
Shot *Shot::sht[MAX_SHOTS];
int wsp[SHOT_MAX];
int wsn[SHOT_MAX];


void Shot::Add() {
  int ctr=0;
  for(ctr=0; ctr<maxsh && sht[ctr]!=NULL; ++ctr);
  lnum = ctr;
  if(ctr >= MAX_SHOTS) U2_Exit(1, "Shots overflow!!\n");
  if(maxsh==ctr) ++maxsh;
  sht[ctr] = this;
  }

void Shot::DeleteAll() {
  int ctr=0;
  for(ctr=0; ctr<maxsh; ++ctr) if (sht[ctr] != NULL) {
    delete sht[ctr];
    }
  }

void Shot::AllAct() {
  int ctr=0;
  for(ctr=0; ctr<maxsh; ++ctr) if (sht[ctr] != NULL) {
    sht[ctr]->Act();
    }
  }

void Shot::InitShotList() {
  int ctr=0;
  for(ctr=0; ctr<MAX_SHOTS; ++ctr) sht[ctr] = NULL;
  maxsh=0;
  }

int Shot::Count() {
  int ctr, ret=0;
  for(ctr=0; ctr<maxsh; ctr++) {
    if(sht[ctr] != NULL) ++ret;
    }
  return ret;
  }

void Shot::Erase() {
  image.Erase();
//  image.Remove();
  type = SHOT_NONE;
  sht[lnum]=NULL;
  }

int Shot::TargetDead() {
  return(target->type==TROOP_NONE || target->armor<1 || target->men<1);
  }

Shot::~Shot() {
  if(type==WEAPON_GUN || type==WEAPON_LASER
	|| type==WEAPON_MASER || type==WEAPON_LMASER) {
    --wsn[type];
    if(wsn[type]<=0) sound[type]->Stop(wsp[type]);
    }
  Erase();
  }

void Shot::Target(int x, int y, Troop *tg, int ms, int r) {
  rof=r; target = tg; miss = ms;
  Graphic tmpg;
  switch(type) {
    case(SHOT_GUN): {
      if(1) {
	if(wtm) --wtm;
	xp = x; yp = y;
	if(wsn[type]==0) wsp[type] = sound[type]->Loop();
	++wsn[type];
	int xd=target->xp-xp, yd=target->yp-yp;
	int dst=(int)sqrt(xd*xd+yd*yd);
	if(dst) { xd*=3; xd/=dst; yd*=3; yd/=dst; }
	else { xd=3; yd=0; }
	if(!stat) {
	  tmpg.SetLine(xd, yd, 32, 0xFFFFFFFF);
	  image.SetImage(tmpg);
	  image.SetPriority(-(yp+100));
	  image.Move(xp, yp);
//	  image.Position(xp, yp);
	  if(!wtm) {
	    if(!miss) target->Hit(wdam, mdam);
	    wtm=rof;
	    }
	  stat=1; act=1;
	  }
	else {
	  image.Erase();
//	  image.Remove();
	  if(!wtm) {
	    if(!miss) target->Hit(wdam, mdam);
	    wtm=rof;
	    }
	  stat=0; act=1;
	  }
	}
      }break;
    case(SHOT_LASER):
    case(SHOT_MASER):
    case(SHOT_LASERMASER): {
      if(x!=xp || y!=yp	|| target->xp!=wxt || target->yp!=wyt) {
	xp = x; yp = y;
	if(wsn[type]==0) wsp[type] = sound[type]->Loop();
	++wsn[type];
	int xd=target->xp-xp, yd=target->yp-yp;
	if(type==SHOT_LASER) tmpg.SetLine(xd, yd, 32, 0xFFFF0000);
	else if(type==SHOT_MASER) tmpg.SetLine(xd, yd, 32, 0xFF00FF00);
	else tmpg.SetLine(xd, yd, 32, 0xFFFFFF00);
	image.SetImage(tmpg);
	image.SetPriority(-(yp+100));
	image.Move(xp, yp);
//	image.Position(xp, yp);
	if(wxt>=0) screen->SetLine(wxt, wyt, target->xp, target->yp, 0xFF000000);
	else screen->SetPoint(target->xp, target->yp, 0xFF000000);
	}
      if(!miss) target->Hit(wdam, mdam);
      act=1; wxt=target->xp; wyt=target->yp;
      }break;
    case(SHOT_BALLISTIC): {
      if(1) {
	xp = x; yp = y;
	sound[SOUND_BALLISTIC]->Play();
	tmpg.SetLine(0, 0, 32, 0xFFFFFFFF);
	image.SetImage(tmpg);
	image.SetPriority(-(yp+100));
	image.Move(xp, yp);
//	image.Position(xp, yp);
	}
      }break;
    }
  }

Shot::Shot(int tp, int wd, int md, int xs, int ys, Troop *tg, int ms, int r) {
  wtm=0;
  type = tp;
  wxt = tg->xp; wyt = tg->yp;
  stat=0; act=0;
  wdam = wd; mdam = md;
  Add();
  Target(xs, ys, tg, ms, r);
  }

void Shot::Act() {
  switch(type) {
    case(SHOT_GUN): {
      if(!act) {
	--wsn[type];
	if(wsn[type]<=0) sound[type]->Stop(wsp[type]);
	image.Erase();
//	image.Remove();
	}
      act=0;
      }break;
    case(SHOT_LASER):
    case(SHOT_MASER):
    case(SHOT_LASERMASER): {
      if(!act) {
	image.Erase();
//	image.Remove();
	wxt=-1; wyt=-1;
	--wsn[type];
	if(wsn[type]<=0) sound[type]->Stop(wsp[type]);
	}
      act=0;
      }break;
    case(SHOT_BALLISTIC): {
      if(stat && stat<9) {
	char buf[64];
	sprintf(buf, "graphics/explosions/boom10/%.4d.tga%c", stat-1, 0);
	Graphic g(buf);
	g.FindTrueCenter();
	g.Undo3ds();
	image.SetImage(g);
	image.Draw(wxt, wyt);
	++stat;
	return;
	}
      else if(stat) {
	Erase();
	return;
	}
      if(target->type != TROOP_NONE) {
        wxt = target->xp-miss;
        wyt = target->yp;
        }
      if(wxt<0) printf("Shooting %d, %d!\n", wxt, wyt);
      int xd = wxt-xp;
      int yd = wyt-yp;
      if(xd*xd+yd*yd <= BALLISTIC_SPEED*BALLISTIC_SPEED) {
        Troop *tr;
        sound[SOUND_SHELL]->Play();
        int ctr, ex, ey, hx=wxt, hy=wyt;
        for(ctr=0; ctr<tr->maxtr; ctr++) {
          ex=tr->trp[ctr]->xp; ey=tr->trp[ctr]->yp;
          if(ex+2>hx && ex-2<hx && ey+2>hy && ey-2<hy) {
            tr->trp[ctr]->Hit(wdam, mdam);
            }
          }
	stat=1;
        }
      else {
        double xdst, ydst;
        if(xd==0) {
          xdst = 0.0; ydst = (double)BALLISTIC_SPEED;
          }
        else if(yd==0) {
          ydst = 0.0; xdst = (double)BALLISTIC_SPEED;
          }
        else {
          double xcoeff = yd*yd; xcoeff /= (double)(xd*xd); xcoeff += 1.0;
          xdst = BALLISTIC_SPEED*BALLISTIC_SPEED; xdst /= xcoeff;
          ydst = xdst * (xcoeff - 1.0);
          xdst = sqrt(xdst); ydst = sqrt(ydst);
          }
        if(xd<0) xdst = -xdst;
        if(yd<0) ydst = -ydst;
	xp+=(int)xdst; yp+=(int)ydst;
        image.Move(xp, yp);
//        image.Position(xp, yp);
        }
      }break;
    }
  }

