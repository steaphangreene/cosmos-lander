#ifndef SHOT_H
#define SHOT_H

#define BALLISTIC_SPEED	10

#define MAX_SHOTS 2048

#define SHOT_NONE	0
#define SHOT_BALLISTIC	1
#define SHOT_LASER	2
#define SHOT_MASER	3
#define SHOT_LASERMASER	4
#define SHOT_GUN	5
#define SHOT_MAX	6

#include "cosmos.h"

class Troop;

class Shot {
  public:
  static int maxsh;
  static Shot *sht[MAX_SHOTS];
  static void InitShotList();
  static int Count();
  static void AllAct();
  static void DeleteAll();
  void Add();

  int xp, yp, lnum;
  int stat, act, wxt, wyt;
  int type, wdam, mdam, rof, miss, wtm;
  Troop *target;
  Sprite image;

  Shot(int tp, int wd, int md, int xs, int ys, Troop *tg, int ms, int rof=0);
  ~Shot();
  void Target(int x, int y, Troop *tg, int ms, int rof=0);
  void Act();
  void Erase();
  int TargetDead();
  };

#endif
