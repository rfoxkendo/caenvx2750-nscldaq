#
#  This establishes which version of NSCLDAQ we're using and where it's installed:
#  Supposedly  you only need to change this definition to update to a newer
#  version of the software:

INSTDIR=/usr/opt/daq/12.0-pre3

include $(INSTDIR)/etc/SBSRdoMakeIncludes

FELIB_CPPFLAGS=
FELIB_LDFLAGS=-lCAEN_FELib



JSON_CPPFLAGS = $(shell pkg-config jsoncpp --cflags)
JSON_LDFLAGS  = $(shell pkg-config jsoncpp  --libs)


#
#  The following allow you to add flags to all the default C and C++ compilation
#  rules.  In most cases C and C++ share the same set of compilation flags so:
#
#

USERCCFLAGS= -I.. $(JSON_CPPFLAGS) $(FELIB_CPPFLAGS)
USERCXXFLAGS=$(USERCCFLAGS)

#  If you have additional load flags (e.g. library dirs and libs):

USERLDFLAGS=-L.. -lCaenVx2750 $(JSON_LDFLAGS) $(FELIB_LDFLAGS)

#
#  This is a list of the objects that go into making the application
#  Make, in most cases will figure out how to build them:

OBJECTS=Skeleton.o DynamicMultiTrigger.o TclConfiguredReadout.o

Readout: $(OBJECTS)
	$(CXXLD) -o Readout $(OBJECTS) $(USERLDFLAGS) $(LDFLAGS)

clean:  
	rm -f $(OBJECTS) Readout

depend:
	makedepend $(USERCXXFLAGS) *.cpp *.c 


help:
	echo make           - Build Readout.
	echo make clean     - Remove products from previous builds.
	echo make depend    - Add header dependencies to Makefile.

