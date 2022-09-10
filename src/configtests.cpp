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
#include <cstdint>
#include <vector>
#include <string>
#include <set>

extern std::string connection;
extern bool        isUsb;


using namespace caen_nscldaq;

class cfgtest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(cfgtest);
    CPPUNIT_TEST(volclkdelay);
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
    CPPUNIT_TEST(chtrigmasks);
    CPPUNIT_TEST(savetraces);
    CPPUNIT_TEST(trgoutmodes);    
    CPPUNIT_TEST(gpiomode);
    CPPUNIT_TEST(busyinsrc);
    CPPUNIT_TEST(syncoutmode);
    CPPUNIT_TEST(boardvetosrc);
    CPPUNIT_TEST(boardvetopolarity);
    CPPUNIT_TEST(chanvetosrc);
    CPPUNIT_TEST(chanvetowidth);
    CPPUNIT_TEST(rundelay);
    CPPUNIT_TEST(autodisarm);
    CPPUNIT_TEST(volclkdelay);
    CPPUNIT_TEST(permckldelay);
    
    CPPUNIT_TEST(wavesource);
    CPPUNIT_TEST(recordsamples);
    CPPUNIT_TEST(waveres);
    CPPUNIT_TEST(analogprobes);
    CPPUNIT_TEST(digitalprobes);
    CPPUNIT_TEST(pretrigger);
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
    void chtrigmasks();
    void savetraces();
    void trgoutmodes();
    void gpiomode();
    void busyinsrc();
    void syncoutmode();
    void boardvetosrc();
    void boardvetopolarity();
    void chanvetosrc();
    void chanvetowidth();
    void rundelay();
    void autodisarm();
    void volclkdelay();
    void permckldelay();
    
    void wavesource();
    void recordsamples();
    void waveres();
    void analogprobes();
    void digitalprobes();
    void pretrigger();
private:
    static std::string vecToList(const std::vector<std::string>& strings);
    static std::string vecToListOfIdenticalLists(const std::vector<std::string>& strings, size_t numReps = 64);
    static std::string itemToList(const std::string& string, size_t numReps=64);
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
// Convert a single string into a list of identicals:

std::string
cfgtest::itemToList(const std::string& item, size_t numReps)
{
    std::vector<std::string> strings;
    for (int i=0; i < numReps; i++) {
        strings.push_back(item);
    }
    // reduced to a previously solved problem:
    
    return vecToList(strings);
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
//   Set the channel trigger masks for Ch64 trigger conditions:

void cfgtest::chtrigmasks() {
    std::vector<uint64_t> oldmasks;
    for (int i =0; i < 64; i++) {
        oldmasks.push_back(m_pModule->getChannelTriggerMask(i));
    }
    
    m_pConfig->configure("channeltriggermasks", itemToList("0"));
    m_pConfig->configureModule(*m_pModule);
    for (int i =0; i < 64; i++) {
        EQ(std::uint64_t(0), m_pModule->getChannelTriggerMask(i));
    }
    
    m_pConfig->configure("channeltriggermasks", itemToList("0xffffffffffffffff"));
    m_pConfig->configureModule(*m_pModule);
    
    for(int i =0; i < 64; i++) {
        EQ(std::uint64_t(0xffffffffffffffff), m_pModule->getChannelTriggerMask(i));
        m_pModule->setChannelTriggerMask(i, oldmasks[i]);         // Restore
    }
}
void cfgtest::savetraces() {
    std::vector<std::string> modes = {"Always", "OnRequest"};
    std::vector<VX2750Pha::TraceRecordMode> enumModes = {
        VX2750Pha::Always, VX2750Pha::OnRequest
    };
    
    std::vector<VX2750Pha::TraceRecordMode> saved;
    for (int i= 0; i < 64; i++) {
        saved.push_back(m_pModule->getTraceRecordMode(i));
    }
    
    for (int m = 0; m < modes.size(); m++) {
        m_pConfig->configure("savetraces", itemToList(modes[m]));
        m_pConfig->configureModule(*m_pModule);
        for (int c = 0; c < 64; c++) {
            EQ(enumModes[m], m_pModule->getTraceRecordMode(c));
        }    
    }
    for (int c =0; c < 64; c++) {
        m_pModule->setTraceRecordMode(c, saved[c]);
    }
    
}
// Trigger out modes (only one):

void cfgtest::trgoutmodes() {
    std::vector<std::string> modes = {
        "TRGIN", "P0", "SwTrg", "LVDS", "ITLA", "ITLB",
        "ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedClkIn", "Run", "RefClk", "TestPulse",
        "Busy", "Fixed0", "Fixed1", "SyncIn", "SIN", "GPIO", "AcceptTrg", "TrgClk"
    };
    std::vector<VX2750Pha::TRGOUTMode>  enummodes =  {
        VX2750Pha::TriggerOut_TRGIN, VX2750Pha::TriggerOut_P0,
        VX2750Pha::TriggerOut_Software, VX2750Pha::TriggerOut_LVDS,
        VX2750Pha::TriggerOut_InternalA,
        VX2750Pha::TriggerOut_InternalB,
        VX2750Pha::TriggerOut_InternalAandInternalB,
        VX2750Pha::TriggerOut_InternalAorInternalB,
        VX2750Pha::TriggerOut_EncodedClockIn,
        VX2750Pha::TriggerOut_Run, VX2750Pha::TriggerOut_ReferenceClock,
        VX2750Pha::TriggerOut_TestPulse,
        VX2750Pha::TriggerOut_Busy, VX2750Pha::TriggerOut_Zero,
        VX2750Pha::TriggerOut_One, VX2750Pha::TriggerOut_SynchIn,
        VX2750Pha::TriggerOut_SIN, VX2750Pha::TriggerOut_GPIO,
        VX2750Pha::AcceptedTrigger, VX2750Pha::TriggerClock
    };
    auto oldmode = m_pModule->getTRGOUTMode();
    
    EQ(modes.size(), enummodes.size());
    
    for (int m =0; m < modes.size(); m++) {
        m_pConfig->configure("triggeroutmode", modes[m]);
        m_pConfig->configureModule(*m_pModule);
        
        EQ(enummodes[m], m_pModule->getTRGOUTMode());
    }
    m_pModule->setTRGOUTMode(oldmode);
    
}
// GPIO modes:

void cfgtest::gpiomode() {
    std::vector<std::string> modes = {
        "Disabled", "TrgIn", "P0", "SIN", "LVDS", "ITLA", "ITLB",
       "ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedClkIn", "SwTrg",
       "Run", "RefClk", "TestPulse", "Busy", "Fixed0", "Fixed1"
    };
    std::vector<VX2750Pha::GPIOMode> enummodes = {
        VX2750Pha::GPIOMode_Disabled, VX2750Pha::GPIOMode_TriggerIn,
        VX2750Pha::GPIOMode_P0, VX2750Pha::GPIOMode_SIN,
        VX2750Pha::GPIOMode_LVDS, VX2750Pha::GPIOMode_InternalA,
        VX2750Pha::GPIOMode_InternalB, VX2750Pha::GPIOMode_InternalAandInternalB,
        VX2750Pha::GPIOMode_InternalAorInternalB, VX2750Pha::GPIOMode_EncodedClockIn,
        VX2750Pha::GPIOMode_SoftwareTrigger, VX2750Pha::GPIOMode_Run,
        VX2750Pha::GPIOMode_ReferenceClock,
        VX2750Pha::GPIOMode_TestPulse, VX2750Pha::GPIOMode_Busy,
        VX2750Pha::GPIOMode_Zero, VX2750Pha::GPIOMode_One
    };
    
    auto old = m_pModule->getGPIOMode();
    
    EQ(modes.size(), enummodes.size());
    for (int m =0; m < modes.size(); m++) {
        m_pConfig->configure("gpiomode", modes[m]);
        m_pConfig->configureModule(*m_pModule);
        EQ(enummodes[m], m_pModule->getGPIOMode());
    }
    m_pModule->setGPIOMode(old);
}
void cfgtest::busyinsrc() {
    std::vector<std::string> modes = {
        "SIN", "GPIO", "LVDS", "Disabled"
    };
    std::vector<VX2750Pha::BusyInSource> enummodes = {
        VX2750Pha::BusyIn_SIN, VX2750Pha::BusyIn_GPIO, VX2750Pha::BusyIn_LVDS,
        VX2750Pha::BusyIn_Disabled 
    };
    
    auto old = m_pModule->getBusyInputSource();
    
    EQ(modes.size(), enummodes.size());
    for (int m = 0; m < modes.size(); m++) {
        m_pConfig->configure("busyinsrc", modes[m]);
        m_pConfig->configureModule(*m_pModule);
        EQ(enummodes[m], m_pModule->getBusyInputSource());
    }
    m_pModule->setBusyInputSource(old);
}
void cfgtest::syncoutmode() {
    std::vector<std::string> modes = {
        "Disabled", "SyncIn", "TestPulse", "IntClk", "Run"
    };
    std::vector<VX2750Pha::SyncOutMode> enummodes = {
        VX2750Pha::SyncOut_Disabled, VX2750Pha::SyncOut_SynchIn,
        VX2750Pha::SyncOut_TestPulse, VX2750Pha::InternalClock, VX2750Pha::SyncOut_Run
    };
    auto old = m_pModule->getSyncOutMode();
    
    EQ(modes.size(), enummodes.size());
    
    for (int m = 0; m < modes.size(); m++) {
        m_pConfig->configure("syncoutmode", modes[m]);
        m_pConfig->configureModule(*m_pModule);
        EQ(enummodes[m], m_pModule->getSyncOutMode());
    }
    
    m_pModule->setSyncOutMode(old);
    
}
void cfgtest::boardvetosrc() {
    std::vector<std::string> modes = {
        "SIN", "LVDS", "GPIO", "P0", "EncodedClkIn", "Disabled"
    };
    std::vector<VX2750Pha::VetoSource> enummodes = {
        VX2750Pha::Veto_SIN, VX2750Pha::Veto_LVDS, VX2750Pha::Veto_GPIO,
        VX2750Pha::Veto_P0, VX2750Pha::Veto_EncodedClock,
        VX2750Pha::Veto_Disabled
    };
    
    EQ(modes.size(), enummodes.size());
    
    auto old = m_pModule->getBoardVetoSource();
    
    for (int m=0; m < modes.size(); m++) {
        m_pConfig->configure("boardvetosrc", modes[m]);
        m_pConfig->configureModule(*m_pModule);
        EQ(enummodes[m], m_pModule->getBoardVetoSource());
    }
    m_pModule->setBoardVetoSource(old);
}
void cfgtest::boardvetopolarity() {
    std::vector<std::string> polarities = {
        "ActiveHigh", "ActiveLow"
    };
    std::vector<VX2750Pha::VetoPolarity> enumpols = {
           VX2750Pha::ActiveHigh, VX2750Pha::ActiveLow 
    };
    auto old = m_pModule->getBoardVetoPolarity();
    
    for (int m = 0; m < polarities.size(); m++) {
        m_pConfig->configure("boardvetopolarity", polarities[m]);
        m_pConfig->configureModule(*m_pModule);
        EQ(enumpols[m], m_pModule->getBoardVetoPolarity());
    }
    m_pModule->setBoardVetoPolarity(old);
}
void cfgtest::chanvetosrc() {
    std::vector<std::string> source = {
        "BoardVeto", "ADCOverSaturation", "ADCUnderSaturation", "Disabled"
    };
    std::vector<VX2750Pha::ChannelVetoSource> enumsrcs = {
        VX2750Pha::BoardVeto, VX2750Pha::OverSaturation,
        VX2750Pha::UnderSaturation, VX2750Pha::ChanVeto_Disabled
    };
    std::vector<VX2750Pha::ChannelVetoSource> olds;
    
    for (int i =0; i < 64; i++) {
        olds.push_back(m_pModule->getChannelVetoSource(i));
    }
    EQ(source.size(), enumsrcs.size());
    for (int m = 0; m < source.size(); m++) {
        m_pConfig->configure("chanvetosrc", itemToList(source[m]));
        m_pConfig->configureModule(*m_pModule);
        for (int c = 0; c < 64; c++) {
            EQ(enumsrcs[m], m_pModule->getChannelVetoSource(c));
        }
    }
    for (int c =0; c < 64; c++) {
        m_pModule->setChannelVetoSource(c, olds[c]);
    }
}
void cfgtest::chanvetowidth() {
    std::uint32_t high = 524280;
    std::string strhigh = "524280";
    
    std::vector<std::uint32_t> olds;
    for (int i=0; i < 64; i++ ) {
        olds.push_back(m_pModule->getChannelVetoWidth(i));
    }
    
    m_pConfig->configure("chanvetowidth", itemToList("0"));
    m_pConfig->configureModule(*m_pModule);
    for (int c =0; c < 64; c++) {
        EQ(std::uint32_t(0), m_pModule->getChannelVetoWidth(c));
    }
    
    m_pConfig->configure("chanvetowidth", itemToList(strhigh));
    m_pConfig->configureModule(*m_pModule);
    for (int c =0; c < 64; c++) {
        EQ(std::uint32_t(high), m_pModule->getChannelVetoWidth(c));
    }
    
    for (int i=0; i < 64; i++ ) {
        m_pModule->setChannelVetoWidth(i, olds[i]);
    }
}
void cfgtest::rundelay() {
    std::uint32_t high = 54280;
    std::string strhigh = "54280";
    
    auto old = m_pModule->getRunDelay();
    
    m_pConfig->configure("rundelay", "0");
    m_pConfig->configureModule(*m_pModule);
    EQ(std::uint32_t(0), m_pModule->getRunDelay());
    
    m_pConfig->configure("rundelay", strhigh);
    m_pConfig->configureModule(*m_pModule);
    EQ(std::uint32_t(high), m_pModule->getRunDelay());    
    
    m_pModule->setRunDelay(old);
}
void cfgtest::autodisarm() {
    auto old = m_pModule->isAutoDisarmEnabled();
    
    m_pConfig->configure("autodisarm", "on");     // Many strings a valid trues.
    m_pConfig->configureModule(*m_pModule);
    ASSERT(m_pModule->isAutoDisarmEnabled());
    
    m_pConfig->configure("autodisarm", "disabled");  // Many different strings are valid falses
    m_pConfig->configureModule(*m_pModule);
    ASSERT(!m_pModule->isAutoDisarmEnabled());
    
    m_pModule->setAutoDisarmEnabled(old);
}

void cfgtest::volclkdelay() {
    std::string strlow  = "0.0";
    std::string strhigh = "18888.0";
    
    double low  = 0.0;
    double high =  18888.0;
    
    
    double old = m_pModule->getVolatileClockDelay();
    
    m_pConfig->configure("volclkoutdelay", strlow);
    m_pConfig->configureModule(*m_pModule);
    ASSERT(std::abs(low - m_pModule->getVolatileClockDelay()) < 1.0);
    
    m_pConfig->configure("volclkoutdelay", strhigh);
    m_pConfig->configureModule(*m_pModule);
    ASSERT(std::abs(high - m_pModule->getVolatileClockDelay()) < 1.0);

    
    
    m_pModule->setVolatileClockDelay(old);
    
}
void cfgtest::permckldelay() {
    std::string strlow  = "0.0";
    std::string strhigh = "18888.0";
    
    double low  = 0.0;
    double high =  18888.0;
    
    
    double old = m_pModule->getPermanentClockDelay();
    
    m_pConfig->configure("permclkoutdelay", strlow);
    m_pConfig->configureModule(*m_pModule);
    ASSERT(std::abs(low - m_pModule->getPermanentClockDelay()) < 1.0);
    
    m_pConfig->configure("permclkoutdelay", strhigh);
    m_pConfig->configureModule(*m_pModule);
    ASSERT(std::abs(high - m_pModule->getPermanentClockDelay()) < 1.0);

    
    
    m_pModule->setPermanentClockDelay(old);
    
}

void cfgtest::wavesource()  {
    std::vector<std::string> sources = {
      "ADC_DATA", "ADC_TEST_TOGGLE", "ADC_TEST_RAMP", "ADC_TEST_SIN", "IPE",
      "Ramp", "SquareWave", "ADC_TEST_PRBS"
    };
    std::vector<VX2750Pha::WaveDataSource> enumsrcs = {
        VX2750Pha::ADC_DATA, VX2750Pha::ADC_TEST_TOGGLE, VX2750Pha::ADC_TEST_RAMP,
        VX2750Pha::ADC_SIN, VX2750Pha::WaveSource_IPE,
        VX2750Pha::WaveSource_Ramp, VX2750Pha::SquareWave,
        VX2750Pha::ADC_TEST_PRBS
    };
    
    std::vector<VX2750Pha::WaveDataSource> old;
    for (int c = 0; c < 64; c++) {
        old.push_back(m_pModule->getWaveDataSource(c));
    }
    for (int m = 0; m < sources.size(); m++) {
        m_pConfig->configure("wavesource", itemToList(sources[m]));
        m_pConfig->configureModule(*m_pModule);
        
        for (int c =0; c < 64; c++) {
            EQ(enumsrcs[m], m_pModule->getWaveDataSource(c));
        }
    }
    
    for (int c = 0; c < 64; c++) {
        m_pModule->setWaveDataSource(c, old[c]);
    }
}
void cfgtest::recordsamples() {
    std::uint32_t low = 4;
    std::string strlow = std::to_string(low);
    
    std::uint32_t high = 8100;
    std::string strhigh = std::to_string(high);
    
    std::vector<std::uint32_t> old;
    for (int c =0; c < 64; c++) {
        old.push_back(m_pModule->getRecordSamples(c));
    }
    
    m_pConfig->configure("recordsamples", itemToList(strlow));
    m_pConfig->configureModule(*m_pModule);
    for (int c =0; c < 64; c++) {
        EQ(low, m_pModule->getRecordSamples(c));
    }
    
    m_pConfig->configure("recordsamples", itemToList(strhigh));
    m_pConfig->configureModule(*m_pModule);
    for (int c =0; c < 64; c++) {
        EQ(high, m_pModule->getRecordSamples(c));
    }
    
    for (int c =0; c < 64; c++) {
        m_pModule->setRecordSamples(c, old[c]);
    }
    
}
void cfgtest::waveres()  {}
void cfgtest::analogprobes()  {}
void cfgtest::digitalprobes()  {}
void cfgtest::pretrigger()  {}
