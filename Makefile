CFLAGS:=$(shell U2-CFlags)
LIBS=	$(shell U2-Libs)
CC=     gcc $(CFLAGS)
USER=	$(shell U2-Dir)
DEPS=   $(USER)/*.cpp
DEPH=   $(USER)/*.h
OBJS=   ship.o viewport.o planet.o game.o troop.o shot.o
ALL=    Makefile cosmos.h planet.h
TSTR:=  $(shell date +"%Y%m%d%H%M")
HDRS:=	ship.h shipitem.h cosmos.h
TSTR:=	$(shell date +"%Y%m%d%H%M")

all:	cosmos

cosmos:	cosmos.o $(OBJS) $(DEPS) $(DEPH) $(ALL)
	make -C $(USER)
	$(CC) -o cosmos cosmos.o $(OBJS) $(LIBS)

%.o:	%.cpp $(HDRS) $(DEPH) $(ALL)
	$(CC) -c $<

tar:	cosmos.h
	cd .. ; tar czhvf ~/c/archive/cosmos-pre$(TSTR).tar.gz cosmos

package:	cosmos
	cd .. ; tar chzvf ~/tar/cosmos-bin-pre$(TSTR).tar.gz		\
	cosmos/cosmos cosmos/*.tga cosmos/fonts cosmos/sounds		\
	cosmos/buildings cosmos/planets					\
	cosmos/graphics/deaths/dome cosmos/graphics/troops		\
	cosmos/graphics/explosions/boom10 cosmos/*.sav
