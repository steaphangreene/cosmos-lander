#ifndef TROOP_H
#define TROOP_H

#define DROPSHIP_DELAY	16

#define TROOP_NONE			0
#define TROOP_MARINE			1
#define TROOP_ARTILLIARY		2
#define TROOP_LTANK			3
#define TROOP_MTANK			4
#define TROOP_BTANK			5
#define TROOP_LHEAVYTANK		6
#define TROOP_MHEAVYTANK		7
#define TROOP_BHEAVYTANK		8
#define TROOP_ANTIOTANK			9
#define TROOP_LMECH			10
#define TROOP_MMECH			11
#define TROOP_BMECH			12
#define TROOP_LHEAVYMECH		13
#define TROOP_MHEAVYMECH		14
#define TROOP_BHEAVYMECH		15
#define TROOP_ANTIOMECH			16
#define TROOP_LATV			17
#define TROOP_MATV			18
#define TROOP_BATV			19
#define TROOP_ANTIOATV			20
#define TROOP_DROPSHIP			21
#define UNIT_MAX			22

#define TROOP_DOME			22
#define TROOP_HEAVY_LASERMASER		23
#define TROOP_BALLISTIC_DEFENDER	24
#define TROOP_BASE			25
#define TROOP_MAX			26

#define WEAPON_NONE		0
#define WEAPON_GUN		1
#define WEAPON_LASER		2
#define WEAPON_MASER		3
#define WEAPON_LMASER		4
#define WEAPON_BALLISTIC	5
#define WEAPON_MISSILE		6
#define WEAPON_DEPLOY		7
#define WEAPON_MAX		8

#define MOVEMENT_NONE		0
#define MOVEMENT_DOME		1
#define MOVEMENT_WALK		2
#define MOVEMENT_DRIVE		3
#define MOVEMENT_HOVER		4
#define MOVEMENT_DROP		5

#define DOING_NOTHING		0
#define GOING_NORTH		1
#define GOING_SOUTH		2
#define GOING_EAST		3
#define GOING_WEST		4
#define DOING_ATTACKING		5

#define BALLISTIC_SPEED		10

#define MAX_TROOPS 4096

#include "cosmos.h"

extern Graphic *troopg[TROOP_MAX];
extern Graphic *troopimg[TROOP_MAX];

class Shot;

struct TroopStats {
  const char *name;
  int cls, type, mtype, wtype, modnum;
  int men, maxmen, armor, dodge, speed;
  int wdam, wmdam, wrng, wacc, wrof;
  TroopStats *contains[4];
  };

class Troop {
  public:
  static int maxtr;
  static TroopStats defstat[TROOP_MAX];
  static Troop *trp[MAX_TROOPS];
  static void InitTroopList();
  static int CountTeam(int);
  static void AllAct();
  static void DeleteAll();
  void Add();

  double axp, ayp;
  int side, type, cls, zp, xp, yp, lnum;
  int mtype, men, maxmen, armor, dodge, speed;
  int wtype, wdam, wmdam, wrng, wacc, wrof, wtm, wstat;
  int doing, stat, target, wxt, wyt;
  int wsnd, tsnd;
  Troop *parent;
  Shot *shot;
  Sprite image;

  Troop(TroopStats *st, int t, int x, int y);
  Troop(TroopStats *st, int t, Troop *p);
  ~Troop();
  void Init(TroopStats *st, int t);
  void Act();
  int Hit(int, int);
  void Erase();
  };

#endif
