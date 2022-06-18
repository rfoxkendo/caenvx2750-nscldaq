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

JSON_CPPFLAGS = $(shell pkg-config jsoncpp --cflags)
JSON_LDLFAGS  = $(shell pkg-config jsoncpp  --libs)

TCL_CPPFLAGS = $(shell pkg-config tcl8.6 --cflags)
TCL_LDFLAGS  = $(shell pkg-config tcl8.6 --libs) 

NSCLDAQROOT=/usr/opt/daq/12.0-pre3
NSCLDAQ_CXXFLAGS=-I$(NSCLDAQROOT)/include
NSCLDAQ_LDFLAGS=-L$(NSCLDAQROOT)/lib -Wl,-rpath=$(NSCLDAQROOT)/lib

CPPFLAGS=$(FELIB_CPPFLAGS) $(DIG2_CPPFLAGS) $(JSON_CPPFLAGS) $(NSCLDAQ_CXXFLAGS) \
	$(TCL_CPPFLAGS)


libCaenVx2750.a:  Dig2Device.o VX2750Pha.o XXUSBConfigurableObject.o VX2750PHAConfiguration.o \
	VX2750TclConfig.o
	ar -ruv $@ $?

Dig2Device.o: Dig2Device.cpp Dig2Device.h
	$(CXX) $(CPPFLAGS) -c $<

VX2750Pha.o: VX2750Pha.cpp VX2750Pha.h Dig2Device.h
	$(CXX) $(CPPFLAGS) -c $<

XXUSBConfigurableObject.o: XXUSBConfigurableObject.cpp XXUSBConfigurableObject.h
	$(CXX) $(CPPFLAGS) -c $<

VX2750PHAConfiguration.o: VX2750PHAConfiguration.cpp VX2750PHAConfiguration.h \
	XXUSBConfigurableObject.h VX2750Pha.h Dig2Device.h
	$(CXX) $(CPPFLAGS) -c $<


VX2750PHATclConfiguration:o VX2750PHATclConfiguration.cpp VX2750PHATclConfiguration.h \
	VX2750PHAConfiguration.h XXUSBConfigurableObject.h
	$(CXX) $(CPPFLAGS) -c $<
