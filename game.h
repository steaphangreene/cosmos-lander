#ifndef COSMOS_GAME_H
#define COSMOS_GAME_H

#include "../user/user.h"

#include "cosmos.h"
#include "planet.h"
#include "viewport.h"

#define MAX_PLAYERS 32

#define PLAYER_NONE	0
#define PLAYER_CONSOLE	1
#define PLAYER_COMPUTER	2
#define PLAYER_NET	3

class Game {
  public:
  Game();
  int Initialize();
  void Play();
  void Save(FILE *);
  void Load(FILE *);

  private:
  int numplayers;
  int ptype[MAX_PLAYERS];
  ViewPort *vp;
  };

#endif
