#ifndef COSMOS_SHIP_H
#define COSMOS_SHIP_H

#include <user.h>

#include "cosmos.h"
#include "shipitem.h"

#define MAX_SHIPITEMS 256

class Ship {
  public:
  Ship(int);
  void AddItem(ShipItem*);

  private:
  int tonns;
  ShipItem *item[MAX_SHIPITEMS];
  Graphic ***image;
  };

#endif
