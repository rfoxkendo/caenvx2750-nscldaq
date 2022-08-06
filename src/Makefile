##
# Tentative Makefile (hardcoded paths etc)
# Note this will be replaced with something more configurable.
# The assumption is that we're not out of tree:

#  This is the configurable part when we move towards production

HERE=$(shell pwd)
FELIB=$(HERE)/../felib
DIG2=$(HERE)/../dig2

CPPUNIT_CPPFLAGS=$(shell pkg-config cppunit --cflags)
CPPUNIT_LDFLAGS=$(shell pkg-config cppunit --libs)

FELIB_CPPFLAGS=-I$(FELIB)/include
FELIB_LDFLAGS=-L$(FELIB)/lib -lCAEN_FELib -Wl,-rpath=$(FELIB)/lib


DIG2_CPPFLAGS=-I$(DIG2)/include
DIG2_LDFLAGS=-L$(DIG2)/lib -lCAEN_Dig2 -Wl,-rpath=$(DIG2)/lib

JSON_CPPFLAGS = $(shell pkg-config jsoncpp --cflags)
JSON_LDLFAGS  = $(shell pkg-config jsoncpp  --libs)

TCL_CPPFLAGS = $(shell pkg-config tcl8.6 --cflags)
TCL_LDFLAGS  = $(shell pkg-config tcl8.6 --libs) 

NSCLDAQROOT=/usr/opt/daq/12.0-pre3
NSCLDAQ_CXXFLAGS=-I$(NSCLDAQROOT)/include -I$(NSCLDAQROOT)/include/sbsreadout
NSCLDAQ_LDFLAGS=-L$(NSCLDAQROOT)/lib -Wl,-rpath=$(NSCLDAQROOT)/lib


ROOTSYS=/usr/opt/root/root-6.24.06
SPECTCLROOT=/usr/opt/spectcl/5.10-000
SPECTCL_CXXFLAGS=-I$(SPECTCLROOT)/include -I$(ROOTSYS)/include -c -g

PUGI_CXXFLAGS=-I$(HERE)../pugixml/include
PUGI_LDFLAGS=-L$(HERE)../pugixml/lib -lpugixml




CPPFLAGS=$(FELIB_CPPFLAGS) $(DIG2_CPPFLAGS) $(JSON_CPPFLAGS) $(NSCLDAQ_CXXFLAGS) \
	$(TCL_CPPFLAGS) $(PUGI_CXXFLAGS)

all: libCaenVx2750.a libCaenVxUnpackers.a test_programs

libCaenVxUnpackers.a:  VX2750ModuleUnpacker.o VX2750EventProcessor.o \
	VX2750EventBuiltEventProcessor.o
	ar -ruv $@ $?

VX2750ModuleUnpacker.o: VX2750ModuleUnpacker.cpp VX2750ModuleUnpacker.h 
	$(CXX) $(SPECTCL_CXXFLAGS) $<

VX2750EventProcessor.o: VX2750EventProcessor.cpp VX2750EventProcessor.h \
	VX2750ModuleUnpacker.h
	$(CXX) $(SPECTCL_CXXFLAGS) $<

VX2750EventBuiltEventProcessor.o: VX2750EventBuiltEventProcessor.cpp \
	VX2750EventBuiltEventProcessor.h \
	VX2750EventProcessor.h \
	VX2750ModuleUnpacker.h
	$(CXX) $(SPECTCL_CXXFLAGS) $<


libCaenVx2750.a:  Dig2Device.o VX2750Pha.o XXUSBConfigurableObject.o VX2750PHAConfiguration.o \
	VX2750TclConfig.o CAENVX2750PhaTrigger.o  VX2750MultiTrigger.o \
	VX2750EventSegment.o VX2750MultiModuleEventSegment.o \
	VX2750XMLConfig.o
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

CAENVX2750PhaTrigger.o: CAENVX2750PhaTrigger.cpp CAENVX2750PhaTrigger.h
	$(CXX) $(CPPFLAGS) -c $<

VX2750MultiTrigger.o: VX2750MultiTrigger.cpp VX2750MultiTrigger.h CAENVX2750PhaTrigger.h \
	VX2750Pha.h
	$(CXX) $(CPPFLAGS) -c $<

VX2750EventSegment.o: VX2750EventSegment.cpp VX2750EventSegment.h \
	VX2750Pha.h VX2750TclConfig.h VX2750PHAConfiguration.h
	$(CXX) $(CPPFLAGS) -c $<

VX2750MultiModuleEventSegment.o: VX2750MultiModuleEventSegment.cpp \
	VX2750MultiModuleEventSegment.h VX2750Pha.h  VX2750MultiTrigger.h
	$(CXX) $(CPPFLAGS) -c $<

VX2750XMLConfig.o: VX2750XMLConfig.cpp VX2750XMLConfig.h \
	VX2750XMLConfig.h VX2750PHAConfiguration.h
	$(CXX) $(CPPFLAGS) -c $<

test_programs:  fejackettests

tests:  fejackettests
	./fejackettests

fejackettests: TestRunner.cpp
	$(CXX) $(CPPUNIT_CPPFLAGS) $(CPPUNIT_LDFLAGS) -o fejackettests $<

