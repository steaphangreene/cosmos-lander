CFLAGS:=$(shell U2-CFlgs)
LIBS=	$(shell U2-Libs)
CC=     gcc $(CFLAGS)
USER=	$(shell U2-Dir)
DEPS=   $(USER)/*.cpp
DEPH=   $(USER)/*.h
OBJS=   ship.o viewport.o planet.o game.o troop.o shot.o
ALL=    $(DEPS) $(DEPH) Makefile cosmos.h planet.h
TSTR:=  $(shell date +"%Y%m%d%H%M")
HDRS:=	ship.h shipitem.h cosmos.h
TSTR:=	$(shell date +"%Y%m%d%H%M")

all:	cosmos

cosmos:	cosmos.o $(OBJS) $(ALL)
	make -C $(USER)
	$(CC) -o cosmos cosmos.o $(OBJS) $(LIBS)

cosmos.o:	cosmos.cpp $(HDRS) $(DEPH)
	$(CC) -c cosmos.cpp

game.o:	game.cpp $(HDRS) $(DEPH)
	$(CC) -c game.cpp

ship.o:	ship.cpp $(HDRS) $(DEPH)
	$(CC) -c ship.cpp

planet.o:	planet.cpp $(HDRS) $(DEPH)
	$(CC) -c planet.cpp

viewport.o:	viewport.cpp troop.h $(HDRS) $(DEPH)
	$(CC) -c viewport.cpp

troop.o:	troop.cpp troop.h $(HDRS) $(DEPH)
	$(CC) -c troop.cpp

shot.o:	shot.cpp shot.h $(HDRS) $(DEPH)
	$(CC) -c shot.cpp

tar:	cosmos.h
	cd .. ; tar czhvf ~/c/archive/cosmos-pre$(TSTR).tar.gz cosmos

package:	cosmos
	cd .. ; tar chzvf ~/tar/cosmos-bin-pre$(TSTR).tar.gz		\
	cosmos/cosmos cosmos/*.tga cosmos/fonts cosmos/sounds		\
	cosmos/buildings cosmos/planets					\
	cosmos/graphics/deaths/dome cosmos/graphics/troops		\
	cosmos/graphics/explosions/boom10