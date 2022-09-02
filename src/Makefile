##
# Tentative Makefile (hardcoded paths etc)
# Note this will be replaced with something more configurable.
# The assumption is that we're not out of tree:

#  This is the configurable part when we move towards production

HERE=$(shell pwd)



CPPUNIT_CPPFLAGS=$(shell pkg-config cppunit --cflags)
CPPUNIT_LDFLAGS=$(shell pkg-config cppunit --libs)

FELIB_CPPFLAGS=
FELIB_LDFLAGS=-lCAEN_FELib 


DIG2_CPPFLAGS=
DIG2_LDFLAGS= #  -lCAEN_Dig2 

JSON_CPPFLAGS = $(shell pkg-config jsoncpp --cflags)
JSON_LDFLAGS  = $(shell pkg-config jsoncpp  --libs)

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




CPPFLAGS=$(FELIB_CPPFLAGS) $(DIG2_CPPFLAGS) $(JSON_CPPFLAGS) \
	$(NSCLDAQ_CXXFLAGS) \
	$(TCL_CPPFLAGS) $(PUGI_CXXFLAGS) -g

DEVTEST_LDFLAGS=$(CPPUNIT_LDFLAGS) $(FELIB_LDFLAGS) $(DIG2_LDFLAGS) \
	$(JSON_LDFLAGS)

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

test_programs:  fejackettests triggertests

#  The arguments below describe the module targeted by tests:

TEST_MODULE_ISUSB=1
TEST_MODULE_CONNECTION=15236

tests:  fejackettests triggertests
	./fejackettests $(TEST_MODULE_CONNECTION) $(TEST_MODULE_ISUSB)
	./triggertests $(TEST_MODULE_CONNECTION) $(TEST_MODULE_ISUSB)

fejackettests: TestRunner.o devtests.o vx2750phatests.o \
	triggertests.o libCaenVx2750.a 
	$(CXX) -g  $(CPPUNIT_CPPFLAGS) $(CPPUNIT_LDFLAGS) -o fejackettests  \
	TestRunner.o devtests.o vx2750phatests.o \
	triggertests.o \
	-L. -lCaenVx2750 $(DEVTEST_LDFLAGS) $(JSON_LDFLAGS)

triggertests: TestRunner.o triggertests.o libCaenVx2750.a
	$(CXX) -g  $(CPPUNIT_CPPFLAGS) $(CPPUNIT_LDFLAGS) -o triggertests  \
	TestRunner.o 	triggertests.o \
	-L. -lCaenVx2750 $(DEVTEST_LDFLAGS) $(JSON_LDFLAGS)


TestRunner.o : TestRunner.cpp
	$(CXX) -g -c  $(CPPUNIT_CPPFLAGS) $(JSON_CPPFLAGS) TestRunner.cpp

devtests.o: devtests.cpp
	$(CXX) -g -c  $(CPPUNIT_CPPFLAGS) $(JSON_CPPFLAGS)  devtests.cpp

vx2750phatests.o : vx2750phatests.cpp
	$(CXX) -g -c  $(CPPUNIT_CPPFLAGS) $(JSON_CPPFLAGS)  vx2750phatests.cpp

triggertests.o : triggertests.cpp
	$(CXX) -g  -c $(CPPUNIT_CPPFLAGS) $(JSON_CPPFLAGS)  triggertests.cpp

clean:
	rm -f *.o *.a
	rm -f fejackettests
