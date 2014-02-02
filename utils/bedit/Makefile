CFLAGS:=$(shell ../../../user/U2-CFlgs)
LIBS=	$(shell ../../../user/U2-Libs)
CC=     gcc -O3 -s -Wall $(CFLAGS)
USER=	$(shell ../../../user/U2-Dir)
DEPS=   $(USER)/*.cpp
DEPH=   $(USER)/*.h
ALL=    $(DEPS) $(DEPH) Makefile
TSTR:=  $(shell date +"%Y%m%d%H%M")

all:	bedit

bedit:	bedit.o $(ALL)
	make -C $(USER)
	$(CC) -o bedit bedit.o $(LIBS)

bedit.o:	bedit.cpp $(DEPH)
	$(CC) -c bedit.cpp
