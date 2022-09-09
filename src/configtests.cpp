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
#include <set>

extern std::string connection;
extern bool        isUsb;


using namespace caen_nscldaq;

class cfgtest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(cfgtest);
    CPPUNIT_TEST(default_1);
    CPPUNIT_TEST(cfgreadout);
    CPPUNIT_TEST(clock);
    CPPUNIT_TEST(startsrc);
    CPPUNIT_TEST(gbltrigsrc_1);
    CPPUNIT_TEST(gbltrigsrc_2);
    CPPUNIT_TEST(wavetrigger_1);
    CPPUNIT_TEST(wavetrigger_2);
    CPPUNIT_TEST(evttrigger_1);
    CPPUNIT_TEST(evttrigger_2);
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
    void gbltrigsrc_1();
    void gbltrigsrc_2();
    void wavetrigger_1();
    void wavetrigger_2();
    void evttrigger_1();
    void evttrigger_2();
private:
    static std::string vecToList(const std::vector<std::string>& strings);
    static std::string vecToListOfIdenticalLists(const std::vector<std::string>& strings, size_t numReps = 64);
};

CPPUNIT_TEST_SUITE_REGISTRATION(cfgtest);

/////////////////////////////////////// UTILITIES ///////////////////////////

// Convert a vector of strings into a space separated list:

std::string
cfgtest::vecToList(const std::vector<std::string>& strings) {
    std::string result;
    
    for (auto s : strings) {
        result += s;
        result += " ";                // Trailing space is harmless
    }
    
    return result;
}
// Make a list of identical lists from a vector of values:

std::string
cfgtest::vecToListOfIdenticalLists(
    const std::vector<std::string>&strings, size_t numReps
) {
    std::string result;
    
    std::string listEntry = "{";
    listEntry += vecToList(strings);   // List entry is an encoded sublist
    listEntry += "}";
    
    std::vector<std::string> finalVec(numReps, listEntry);
    
    return vecToList(finalVec);
}

//////////////////////////////////////// TESTS //////////////////////////////

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
// Test for global trigger source - individual sources:

void cfgtest::gbltrigsrc_1() {
    std::vector<std::string> sources = {
        "TrgIn", "P0", "SwTrg", "LVDS", "ITLA", "ITLB",
         "ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedClkIn", "GPIO", "TestPulse",
    };
    std::vector<VX2750Pha::GlobalTriggerSource> enumsrcs = {
        VX2750Pha::GlobalTrigger_TriggerIn, VX2750Pha::GlobalTrigger_P0,
        VX2750Pha::GlobalTrigger_Software, VX2750Pha::GlobalTrigger_LVDS,
        VX2750Pha::GlobalTrigger_InternalA,
        VX2750Pha::GlobalTrigger_InternalB, VX2750Pha::GlobalTrigger_InternalAandB,
        VX2750Pha::GlobalTrigger_InternalAorB, VX2750Pha::GlobalTrigger_EncodedClockIn,
        VX2750Pha::GlobalTrigger_GPIO,
        VX2750Pha::GlobalTrigger_TestPulse
    };
    auto prior = m_pModule->getGlobalTriggerSource();  // Vector of sources:
    
    for (int i =0; i < sources.size(); i++) {
        std::vector<std::string> src = {sources[i]};
        m_pConfig->configure("gbltriggersrc", vecToList(src));
        m_pConfig->configureModule(*m_pModule);
        auto actual = m_pModule->getGlobalTriggerSource();
        EQ(size_t(1), actual.size());
        EQ(enumsrcs[i], actual[0]);
    }
    m_pModule->setGlobalTriggerSource(prior);
}
// test combinations of trigger sources:

void cfgtest::gbltrigsrc_2() {
    std::vector<std::vector<std::string>> sources = {
        {"TrgIn", "P0"}, {"SwTrg", "LVDS", "ITLA", "ITLB"},
         {"ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedClkIn", "GPIO", "TestPulse"}
    };
    std::vector<std::vector<VX2750Pha::GlobalTriggerSource>> enumsrcs = {
        {VX2750Pha::GlobalTrigger_TriggerIn, VX2750Pha::GlobalTrigger_P0},
        {VX2750Pha::GlobalTrigger_Software, VX2750Pha::GlobalTrigger_LVDS,
        VX2750Pha::GlobalTrigger_InternalA,
        VX2750Pha::GlobalTrigger_InternalB}, {VX2750Pha::GlobalTrigger_InternalAandB,
        VX2750Pha::GlobalTrigger_InternalAorB, VX2750Pha::GlobalTrigger_EncodedClockIn,
        VX2750Pha::GlobalTrigger_GPIO,
        VX2750Pha::GlobalTrigger_TestPulse}
    };
    auto prior = m_pModule->getGlobalTriggerSource();
    
    for (int  i=0; i < sources.size(); i++) {
        m_pConfig->configure("gbltriggersrc", vecToList(sources[i]));
        m_pConfig->configureModule(*m_pModule);
        
        auto actual = m_pModule->getGlobalTriggerSource();
        EQ(enumsrcs[i].size(), actual.size());
        std::set<VX2750Pha::GlobalTriggerSource> actualSet(actual.begin(), actual.end());
        for (auto src : enumsrcs[i]) {
            EQ(size_t(1), actualSet.count(src));
        }
    }
    m_pModule->setGlobalTriggerSource(prior);
}

// single wave trigger source 
void cfgtest::wavetrigger_1() {
    std::vector<std::vector<std::string>> sources = {
        {"ITLB"}, {"ITLA"}, {"GlobalTriggerSource"}, {"TRGIN"}, {"ExternalInhibit"},
          {"ADCUnderSaturation"}, {"ADCOverSaturation"}, {"SWTrg"}, {"ChSelfTrigger"},
          {"Ch64Trigger"}, {"Disabled"}
    };
    std::vector<std::vector<VX2750Pha::WaveTriggerSource>> enumsrcs = {
         {VX2750Pha::WaveTrigger_InternalB},  {VX2750Pha::WaveTrigger_InternalA},
        {VX2750Pha::WaveTrigger_GlobalTriggerSource},
        {VX2750Pha::WaveTrigger_TRGIN}, {VX2750Pha::ExternalInhibit},
        {VX2750Pha::ADCUnderSaturation}, {VX2750Pha::ADCOverSaturation},
        {VX2750Pha::WaveTrigger_Software}, {VX2750Pha::WaveTrigger_ChannelSelfTrigger},
        {VX2750Pha::WaveTrigger_AnyChannelSelfTrigger}, {VX2750Pha::WaveTrigger_Disabled}
    };
    EQ(sources.size(), enumsrcs.size());               // Test the test data :-)
    
    
    
    std::vector<std::vector<VX2750Pha::WaveTriggerSource>> prior;
    for (int i =0; i < 64; i++) {
        prior.push_back(m_pModule->getWaveTriggerSource(i));
    }
    
    for (int i =0; i < sources.size(); i++) {
        std::string cfgValue = vecToListOfIdenticalLists(sources[i]);
        CPPUNIT_ASSERT_NO_THROW(m_pConfig->configure("wavetriggersrc", cfgValue));
        m_pConfig->configureModule(*m_pModule);
        
        for (int c =0; c < 64; c++) {
            auto actualTriggers = m_pModule->getWaveTriggerSource(c);
            EQ(size_t(1), actualTriggers.size());
            EQ(enumsrcs[i][0], actualTriggers[0]);
        }
    }
    
    for (int i =0; i < 64; i++) {
        
        m_pModule->setWaveTriggerSource(i, prior[i]);
    }
}
// multi wave trigger source

void cfgtest::wavetrigger_2() {
    std::vector<std::vector<std::string>> sources = {
        {"ITLB", "ITLA"}, {"GlobalTriggerSource", "TRGIN", "ExternalInhibit"},
          {"ADCUnderSaturation", "ADCOverSaturation", "SWTrg", "ChSelfTrigger",
          "Ch64Trigger"}
    };
    std::vector<std::vector<VX2750Pha::WaveTriggerSource>> enumsrcs = {
        {VX2750Pha::WaveTrigger_InternalA, VX2750Pha::WaveTrigger_InternalB},
        {VX2750Pha::WaveTrigger_GlobalTriggerSource,
        VX2750Pha::WaveTrigger_TRGIN, VX2750Pha::ExternalInhibit},
        {VX2750Pha::ADCUnderSaturation, VX2750Pha::ADCOverSaturation,
        VX2750Pha::WaveTrigger_Software, VX2750Pha::WaveTrigger_ChannelSelfTrigger,
        VX2750Pha::WaveTrigger_AnyChannelSelfTrigger}
    };
    EQ(sources.size(), enumsrcs.size());               // Test the test data :-)
    
    std::vector<std::vector<VX2750Pha::WaveTriggerSource>> prior;
    for (int i =0; i < 64; i++) {
        prior.push_back(m_pModule->getWaveTriggerSource(i));
    }
    
    for (int i =0; i < sources.size(); i++) {
        auto cfgValue = vecToListOfIdenticalLists(sources[i]);
        m_pConfig->configure("wavetriggersrc", cfgValue);
        m_pConfig->configureModule(*m_pModule);
        
        for (int c =0; c <64; c++) {
            auto actual = m_pModule->getWaveTriggerSource(i);
            EQ(enumsrcs[i].size(), actual.size());
            
            // All of the sources in enumsrcs[i] must be in actual
            
            std::set<VX2750Pha::WaveTriggerSource> sactual(actual.begin(), actual.end());
            for (auto src : enumsrcs[i]) {
                EQ(size_t(1), sactual.count(src));
            }
        }
    }
    
    for (int i =0; i < 64; i++) {
        m_pModule->setWaveTriggerSource(i, prior[i]);
    }
}


// single event trigger.
void cfgtest::evttrigger_1() {
    std::vector<std::vector<std::string>> sources = {
        {"ITLA"}, {"ITLB"}, {"GlobalTriggerSource"}, {"TRGIN"}, {"SWTrg"},
        {"ChSelfTrigger"},
        {"Ch64Trigger"}, {"Disabled"}
    };
    
    std::vector<std::vector<VX2750Pha::EventTriggerSource>> enumsrcs = {
        {VX2750Pha::EventTrigger_InternalA}, {VX2750Pha::EventTrigger_InternalB},
        {VX2750Pha::EventTrigger_GlobalTriggerSource}, {VX2750Pha::EventTrigger_TRGIN},
        {VX2750Pha::EventTrigger_Software}, {VX2750Pha::EventTrigger_ChannelSelfTrigger},
        {VX2750Pha::EventTrigger_AnyChannelSelfTrigger}, {VX2750Pha::EventTrigger_Disabled}
    };
    
    std::vector<std::vector<VX2750Pha::EventTriggerSource>> prior;
    for (int i =0; i < 64; i++) {
        prior.push_back(m_pModule->getEventTriggerSource(i));
    }
    EQ(sources.size(), enumsrcs.size());
    
    for (int i =0; i < sources.size(); i++) {
        auto cfgValue = vecToListOfIdenticalLists(sources[i]);
        m_pConfig->configure("eventtriggersrc", cfgValue);
        m_pConfig->configureModule(*m_pModule);
        
        for (int c = 0; c < 64; c++) {
            auto actual = m_pModule->getEventTriggerSource(c);
            EQ(enumsrcs[i].size(), actual.size());
            EQ(enumsrcs[i][0], actual[0]);
        }
    }
    
    
    for (int i =0; i < 64; i++) {
        m_pModule->setEventTriggerSource(i, prior[i]);
    }
}
// multi source event trigger:

void cfgtest::evttrigger_2() {
    std::vector<std::vector<std::string>> sources = {
        {"ITLA", "ITLB"}, {"GlobalTriggerSource", "TRGIN", "SWTrg"},
        {"ChSelfTrigger",
        "Ch64Trigger"}
    };
    
    std::vector<std::vector<VX2750Pha::EventTriggerSource>> enumsrcs = {
        {VX2750Pha::EventTrigger_InternalA, VX2750Pha::EventTrigger_InternalB},
        {VX2750Pha::EventTrigger_GlobalTriggerSource, VX2750Pha::EventTrigger_TRGIN,
        VX2750Pha::EventTrigger_Software},
        {VX2750Pha::EventTrigger_ChannelSelfTrigger,
        VX2750Pha::EventTrigger_AnyChannelSelfTrigger}
    };
    
    std::vector<std::vector<VX2750Pha::EventTriggerSource>> prior;
    for (int i =0; i < 64; i++) {
        prior.push_back(m_pModule->getEventTriggerSource(i));
    }
    EQ(sources.size(), enumsrcs.size());
    
    for (int i =0; i < sources.size(); i++) {
        auto cfgValue = vecToListOfIdenticalLists(sources[i]);
        m_pConfig->configure("eventtriggersrc", cfgValue);
        m_pConfig->configureModule(*m_pModule);
        
        for (int c = 0; c < 64; c++) {
            auto actual = m_pModule->getEventTriggerSource(c);
            EQ(enumsrcs[i].size(), actual.size());
            
            std::set<VX2750Pha::EventTriggerSource> sactual(actual.begin(), actual.end());
            for (auto src : enumsrcs[i]) {
                EQ(size_t(1), sactual.count(src));
            }
        }
    }
    
    
    for (int i =0; i < 64; i++) {
        m_pModule->setEventTriggerSource(i, prior[i]);
    }
}