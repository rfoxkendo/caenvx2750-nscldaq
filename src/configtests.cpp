/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2017.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Authors:
             Ron Fox
             Giordano Cerriza
	     FRIB
	     Michigan State University
	     East Lansing, MI 48824-1321
*/

/** @file:  
 *  @brief: 
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include "Asserts.h"
#define private public
#include "VX2750Pha.h"     // Base module. -- white box it.
#undef private
#include "VX2750PHAConfiguration.h"  // Configuration module Tcl style.
#include <vector>
#include <string>

extern std::string connection;
extern bool        isUsb;


using namespace caen_nscldaq;

class cfgtest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(cfgtest);
    CPPUNIT_TEST(default_1);
    CPPUNIT_TEST(cfgreadout);
    CPPUNIT_TEST(clock);
    CPPUNIT_TEST(startsrc);
    CPPUNIT_TEST_SUITE_END();
    
private:
    VX2750Pha*              m_pModule;
    VX2750PHAModuleConfiguration* m_pConfig;
public:
    void setUp() {
        m_pModule = new VX2750Pha(connection.c_str(), isUsb);
        m_pConfig = new VX2750PHAModuleConfiguration("test");
    }
    void tearDown() {
        delete m_pConfig;
        delete m_pModule;
    }
protected:
    void default_1();
    void cfgreadout();
    void clock();
    void startsrc();
};

CPPUNIT_TEST_SUITE_REGISTRATION(cfgtest);

// Default config should, at least, be legal:
void cfgtest::default_1()
{
    CPPUNIT_ASSERT_NO_THROW(m_pConfig->configureModule(*m_pModule));
}
// should be able to turn on and off all the optional readout flags:

void cfgtest::cfgreadout()
{
    // turn them all on:
    
    m_pConfig->configure("readrawtimes", "true");
    m_pConfig->configure("readfinetimestamps", "true");
    m_pConfig->configure("readflags", "true");
    m_pConfig->configure("readtimedownsampling", "true");
    m_pConfig->configure("readanalogprobes","true true");
    m_pConfig->configure("readdigitalprobes", "true true true true");
    m_pConfig->configure("readsamplecount", "true");
    m_pConfig->configure("readeventsize", "true");
    
    m_pConfig->configureModule(*m_pModule);
    
    ASSERT(m_pModule->m_dppPhaOptions.s_enableRawTimestamps);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableFineTimestamps);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableFlags);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableDownsampledTime);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableAnalogProbe1);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableAnalogProbe2);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableDigitalProbe1);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableDigitalProbe2);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableDigitalProbe3);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableDigitalProbe4);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableSampleCount);
    ASSERT(m_pModule->m_dppPhaOptions.s_enableEventSize);
    
    // Turn them all off:

    m_pConfig->configure("readrawtimes", "false");
    m_pConfig->configure("readfinetimestamps", "false");
    m_pConfig->configure("readflags", "false");
    m_pConfig->configure("readtimedownsampling", "false");
    m_pConfig->configure("readanalogprobes", "false false");
    m_pConfig->configure("readdigitalprobes", "false false false false");
    m_pConfig->configure("readsamplecount", "false");
    m_pConfig->configure("readeventsize", "false");
    
    m_pConfig->configureModule(*m_pModule);
    
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableRawTimestamps);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableFineTimestamps);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableFlags);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableDownsampledTime);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableAnalogProbe1);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableAnalogProbe2);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableDigitalProbe1);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableDigitalProbe2);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableDigitalProbe3);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableDigitalProbe4);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableSampleCount);
    ASSERT(!m_pModule->m_dppPhaOptions.s_enableEventSize);
    
}
// test the general options which are just the clock source/output.
// note we've determined that the P0Clock won't work at least in this module.

void cfgtest::clock()
{
    std::vector<std::string> sources = {"Internal", "FPClkIn"};
    std::vector<VX2750Pha::ClockSource> configs = {VX2750Pha::Internal, VX2750Pha::FrontPanel};
    
    for (int i =0; i < sources.size(); i++) {
        m_pConfig->configure("clocksource", sources[i]);
        m_pConfig->configureModule(*m_pModule);
        EQ(configs[i], m_pModule->getClockSource());
    }
    
    m_pConfig->configure("outputfpclock", "false");
    m_pConfig->configureModule(*m_pModule);
    ASSERT(!m_pModule->isClockOutOnFP());
    
    m_pConfig->configure("outputfpclock", "true");
    m_pConfig->configureModule(*m_pModule);
    ASSERT(m_pModule->isClockOutOnFP());
    
}
// Test the various start sources:
void cfgtest::startsrc()
{
    std::vector<std::string> srcStrings = {"EncodedClkIn", "SINlevel", "SINedge", "SWcmd", "LVDS", "P0"};
    std::vector<VX2750Pha::StartSource> srcs = {
        VX2750Pha::Start_EncodedClockIn, VX2750Pha::SINLevel, VX2750Pha::SINEdge, VX2750Pha::SWCommand,
        VX2750Pha::Start_LVDS, VX2750Pha::Start_P0
    };
    
    for (int i =0; i < srcStrings.size(); i++)  { // Allows us to index srcs.
        m_pConfig->configure("startsource", srcStrings[i]);
        m_pConfig->configureModule(*m_pModule);
        EQ(srcs[i], m_pModule->getStartSource());
    }
}