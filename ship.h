#ifndef COSMOS_SHIP_H
#define COSMOS_SHIP_H

#define MAX_SHIPS 16
#define MAX_DROPSHIPS 16

#include <user.h>

#include "cosmos.h"
#include "shipitem.h"
#include "troop.h"

#define MAX_SHIPITEMS 256

class Ship {
  public:
  Ship(int, const char *);
  void AddItem(ShipItem*);
  char *Name() { return name; };
  int NumDrop() { return numdrop; };
  int DropX(int n) { return dropx[n]; };
  int DropY(int n) { return dropy[n]; };
  void MoveDrop(int n, int x, int y) { dropx[n]=x; dropy[n]=y; };
  TroopStats *Drop(int n) { return &drop[n]; };

  private:
  int tonns, owner;
  ShipItem *item[MAX_SHIPITEMS];
  Graphic ***image;
  char name[32];
  int numdrop;
  TroopStats drop[MAX_DROPSHIPS];
  int dropx[MAX_DROPSHIPS], dropy[MAX_DROPSHIPS];
  };

#endif
