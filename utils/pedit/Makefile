CFLAGS:=$(shell U2-CFlgs)
LIBS=	$(shell U2-Libs)
CC=     gcc -O3 -s -Wall $(CFLAGS)
USER=	$(shell U2-Dir)
DEPS=   $(USER)/*.cpp
DEPH=   $(USER)/*.h
ALL=    $(DEPS) $(DEPH) Makefile
TSTR:=  $(shell date +"%Y%m%d%H%M")

all:	pedit

pedit:	pedit.o $(ALL)
	make -C $(USER)
	$(CC) -o pedit pedit.o $(LIBS)

pedit.o:	pedit.cpp $(DEPH)
	$(CC) -c pedit.cpp
