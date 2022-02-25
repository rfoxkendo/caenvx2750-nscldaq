##
# Tentative Makefile (hardcoded paths etc)
# Note this will be replaced with something more configurable.
# The assumption is that we're not out of tree:

#  This is the configurable part when we move towards production

HERE=$(shell pwd)
FELIB=$(HERE)/../felib
DIG2=$(HERE)/../dig2

FELIB_CPPFLAGS=-I$(FELIB)/include
FELIB_LDFLAGS=-L$(FELIB)/lib -lCAEN_FELib -Wl,-rpath=$(FELIB)/lib


DIG2_CPPFLAGS=-I$(DIG2)/include
DIG2_LDFLAGS=-L$(DIG2)/lib -lCAEN_Dig2 -Wl,-rpath=$(DIG2)/lib

CPPFLAGS=$(FELIB_CPPFLAGS) $(DIG2_CPPFLAGS)


libCaenVx2750.a:  Dig2Device.o
	ar -ruv $@ $?

Dig2Device.o: Dig2Device.cpp Dig2Device.h
	$(CXX) $(CPPFLAGS) -c $<

