#ifndef COSMOS_PLANET_H
#define COSMOS_PLANET_H

#include <user.h>

#define MAX_PLANET_NAME 64

//#define BUILDING_DROPSHIP			1
#define BUILDING_NONE				-1
#define BUILDING_DOME				0
#define BUILDING_SPACEPORT			1
#define BUILDING_MISC_MAX			2

#define BUILDING_HEAVY_LASERMASER		2
#define BUILDING_BALLISTIC_DEFENDER		3
#define BUILDING_MILITARY_BASE			4
#define BUILDING_WEAPON_MAX			5

#define BUILDING_INSTELATION_COLLECTOR		5
#define BUILDING_GENERATOR_MAX			6

#define BUILDING_MAX				6

#define BUILDING_HIGH_PRECISION_LASER		4
#define BUILDING_ANTIMATER_PROJECTOR		5
#define BUILDING_ANTIORBITAL_MASER		6
#define BUILDING_KINETIC_INVERTER		7
#define BUILDING_ELECTROMAGNETIC_MEDIUM_VOID	8
#define BUILDING_ELECTROMAGNETIC_REPULSOR	9
#define BUILDING_MICROKINETIC_NULLIFIER		10
#define BUILDING_PROJECTILE_INTERCEPTER		11
#define BUILDING_SPACE_OBSERVATORY		12
#define BUILDING_STELLAR_RADIATION_ANALYSIS_LAB	13
//#define BUILDING_INSTELATION_COLLECTOR		14
#define BUILDING_FLUID_MOTION_GENERATOR		15
#define BUILDING_ANTIMATER_STATION		16
#define BUILDING_FISSION_FUSION_REACTOR		17
#define BUILDING_RESIDENTIAL_MEGAPLEX		18
#define BUILDING_INDUSTRIAL_MEGAPLEX		19
#define BUILDING_COMMERCIAL_MEGAPLEX		20
#define BUILDING_PUBLIC_MEDIA_CENTRE		21
#define BUILDING_SHIP_DESIGN_FACILITY		22
#define BUILDING_SHIPYARD			23
#define BUILDING_SATELLITE_PRODUCTION_PLANT	24
#define BUILDING_COLONY_MAKER			25
#define BUILDING_MILITARY_PRODUCTION_LINE	26
#define BUILDING_BIOTECH_UNIVERSITY		27
#define BUILDING_ASTROPHYSICS_UNIVERSITY	28
#define BUILDING_CHEMISTRY_UNIVERSITY		29
#define BUILDING_MATERIAL_SCIENCE_UNIVERSITY	30
#define BUILDING_MILITARY_SCHOOL		31
#define BUILDING_THEORETICAL_METAPHYSICS_LAB	32
#define BUILDING_SUBATOMIC_PHYSICS_LAB		33
#define BUILDING_VOLATILE_CHEMICALS_LAB		34
#define BUILDING_POWER_PROJECTOR		36
#define BUILDING_FIGHTER_HANGER			37

#define BTYPE_MISC	0
#define BTYPE_WEAPON	1
#define BTYPE_GENERATOR	2
#define BTYPE_MAX	3

#define BUILDSTAT_MAX	144

#define TERRAIN_NONE	0
#define TERRAIN_LAND	1
#define TERRAIN_WATER	2

#include "cosmos.h" 

struct TroopStats;

class Planet {
  public:
  Planet(const char*, const char*, int);
  Planet(FILE *);
  void PreInit();
  void Init();
  ~Planet();
  Graphic *Image() { return image; };
  void PutBuilding(int, int, int, TroopStats *);
  int Building(int x, int y) { return build[x][y]; };
  int Dome(int x, int y) { return dome[x][y]; };
  int Terrain(int x, int y) { return (*stats)[x+y*25]; };
  void Over(int x, int y);
  void Select(int x, int y);
  void Deselect();
  void Update();
  Sprite items[25][24];
  TroopStats *bt[25][24];
  void Save(FILE *);
  void Load(FILE *);
  int Owner() { return owner; };
  void SetOwner(int o) { owner=o; };

  private:
  int update[25][24];
  int selx, sely;
  int ovrx, ovry;
  int owner;
  char name[MAX_PLANET_NAME];
  char rfn[MAX_PLANET_NAME];
  unsigned long discovered;
  Graphic *image;
  CharBag *stats;
  int build[25][24];
  int dome[25][24];
  };

#endif
