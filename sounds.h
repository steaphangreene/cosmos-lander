#include "shot.h"

#define SOUND_AMBIENT		0
#define SOUND_BALLISTIC		1
#define SOUND_LASER		2
#define SOUND_MASER		3
#define SOUND_LASERMASER	4
#define SOUND_GUN		5
#define SOUND_SHELL		6
#define SOUND_OPEN		7
#define SOUND_DEATH		8
#define SOUND_DOMEDIE		9
#define SOUND_MAX		10

extern Sound *sound[SOUND_MAX];
extern int wsp[SHOT_MAX];
extern int wsn[SHOT_MAX];
