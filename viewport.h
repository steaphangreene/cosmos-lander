#ifndef COSMOS_VIEWPORT_H
#define COSMOS_VIEWPORT_H

#include "../user/user.h"

#define VPMODE_PLANET	0
#define VPMODE_SYSTEM	1
#define VPMODE_GALACTIC	2

#define VPSTAT_BUILD	0
#define VPSTAT_CONFIG	1
#define VPSTAT_ATTACK	2
#define VPSTAT_VIEW	3
#define VPSTAT_MAX	4

class ViewPort {
  public:
  ViewPort();
  void Update();
  void Click(int, int, int);
  void Key(int);
  void Position(int, int);
  int PlanetaryCombat();

  private:
  int mode, omode, stat, ostat, tile, oplanet, planet, system;
  int tsel[VPSTAT_MAX], otsel[VPSTAT_MAX];
  int *psel[VPSTAT_MAX], *opsel[VPSTAT_MAX];
  int tnum[VPSTAT_MAX], *pnum[VPSTAT_MAX], *poff[VPSTAT_MAX];
  int selx, sely, seltp;
  Sprite *pane[VPSTAT_MAX], *tab[VPSTAT_MAX];
  void SavePlan(FILE *);
  void LoadPlan(FILE *);
  };

#endif
