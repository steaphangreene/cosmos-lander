#include "cosmos.h"
#include "ship.h"

int *numships;
Ship ***ship;

Ship::Ship(int o, const char *n) {
  int ctr;  Troop *tr=NULL;
  sprintf(name, "%s%c", n, 0);
  for(ctr=0; ctr<MAX_DROPSHIPS; ++ctr) {
    drop[ctr] = tr->defstat[TROOP_DROPSHIP];
    dropx[ctr] = -1;
    dropy[ctr] = -1;
    drop[ctr].contains[0] = new TroopStats;
    *(drop[ctr].contains[0]) = tr->defstat[TROOP_MARINE];
    drop[ctr].contains[1] = new TroopStats;
    *(drop[ctr].contains[1]) = tr->defstat[TROOP_MARINE];
    }
  owner = o;
  numdrop = 8;
  }
