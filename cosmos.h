#ifndef COSMOS_COSMOS_H
#define COSMOS_COSMOS_H

#define MAX_PLANETS 1024

#include <user.h>
#include "planet.h"
#include "sounds.h"

extern Screen *screen;
extern InputQueue *input;
extern Mouse *mouse;
extern Keyboard *key;
extern Speaker *spk;

extern int player;
extern Planet *plan[MAX_PLANETS];
extern Graphic *buildimg[BUILDING_MAX];
extern Graphic *buildg[BUILDING_MAX][BUILDSTAT_MAX];
extern Graphic *dmdth[17];
extern int builds[BUILDING_MAX][BUILDSTAT_MAX];

#endif
