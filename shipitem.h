#ifndef COSMOS_SHIPITEM_H
#define COSMOS_SHIPITEM_H

#define SHIPITEM_NONE		0
#define SHIPITEM_GENERATOR	1
#define SHIPITEM_PROJECTILE	2
#define SHIPITEM_BEAM		3
#define SHIPITEM_TORPEDO	4
#define SHIPITEM_SHEILD		5
#define SHIPITEM_ENGINE		6
#define SHIPITEM_STORAGE	7
#define SHIPITEM_HYPERDRIVE	8
#define SHIPITEM_SENSOR		9
#define SHIPITEM_COUNTERSENSOR	10
#define SHIPITEM_LANDER		11
#define SHIPITEM_LAUNCHER	12
#define SHIPITEM_MISC		13

#include "cosmos.h"

struct ShipItem {
  int type;
  int power;	// Power needed (Per use or continuous)
  int effect;	// Damage, Force, Strength, Size
  int range;	// Range
  int catagory;	// Light/Medium/Heavy/Ultraheavy -or- Basic/Advanced/Ultimate
  int space;	// Space Needed
  int state;	// Depleted? Contains?
  void *within;	// What's in it?
  };

#endif
