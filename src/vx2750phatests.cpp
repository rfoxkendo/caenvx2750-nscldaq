/*
   

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

     Authors:
             Ron Fox
        

/** @file:  vx2750phatests.cpp
 *  @brief: Test for VX2750Pha class.
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include <string>

#include "Asserts.h"
#include "VX2750Pha.h"
#include <cstdint>
extern std::string connection;
extern bool        isUsb;

using namespace caen_nscldaq;

class vx2750phatest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(vx2750phatest);
    CPPUNIT_TEST(getcup);
    CPPUNIT_TEST(getfpgafw);
    CPPUNIT_TEST(getfwType);
    CPPUNIT_TEST(getmodelCode);
    CPPUNIT_TEST(getpiggycode);
    CPPUNIT_TEST(getmodel);
    
    CPPUNIT_TEST(getform);
    CPPUNIT_TEST(getfamcode);
    CPPUNIT_TEST(getserial);
    CPPUNIT_TEST(getmotherrev);
    CPPUNIT_TEST(getpiggyrev);
    CPPUNIT_TEST(getlicense);
    CPPUNIT_TEST(isLicensed);
    
    CPPUNIT_TEST(timebomb);
    CPPUNIT_TEST(channelCount);
    CPPUNIT_TEST(bits);
    CPPUNIT_TEST(samplerate);
    CPPUNIT_TEST(inputRange);
    CPPUNIT_TEST(inputsDifferential);
    CPPUNIT_TEST(inputImpedance);
    CPPUNIT_TEST(ip);
    CPPUNIT_TEST(netmask);
    CPPUNIT_TEST(gateway);
    
    CPPUNIT_TEST(clocksource);
    CPPUNIT_TEST(clockout);
    CPPUNIT_TEST(startsource);
    CPPUNIT_TEST(gbltrigger);
    CPPUNIT_TEST(wavetrigger);
    CPPUNIT_TEST(eventtrigger);
    CPPUNIT_TEST(chtrigmask);
    CPPUNIT_TEST(tracerecordmode);
    CPPUNIT_TEST(trgoutmode);
    CPPUNIT_TEST(gpiomode);
    CPPUNIT_TEST(busyinsrc);
    
    CPPUNIT_TEST(syncout);
    CPPUNIT_TEST(bvetosrc);
    CPPUNIT_TEST(bvetowidth);
    CPPUNIT_TEST(bvetopolarity);
    CPPUNIT_TEST(chvetosrc);
    CPPUNIT_TEST(chvetowid);
    CPPUNIT_TEST(rundelay);
    CPPUNIT_TEST(autodisarm);
    CPPUNIT_TEST(multiwindow);
    CPPUNIT_TEST(pausts);
    CPPUNIT_TEST(getters);
    
    CPPUNIT_TEST(vclockdelay);
    CPPUNIT_TEST(pclockdelay);
    CPPUNIT_TEST(wdatasource);
    CPPUNIT_TEST(recl);
    CPPUNIT_TEST(wresolution);
    CPPUNIT_TEST(aprobe);
    CPPUNIT_TEST(dprobe);
    CPPUNIT_TEST(pretrigger);
    CPPUNIT_TEST(tpulseperiod);
    CPPUNIT_TEST(tpulsewidth);
    CPPUNIT_TEST(tpulselow);
    CPPUNIT_TEST(tpulsehigh);
    CPPUNIT_TEST(iolvel);
    CPPUNIT_TEST(monitor);
    CPPUNIT_TEST(errorflags);
    CPPUNIT_TEST(itlab);
    CPPUNIT_TEST(pairlogic);
    CPPUNIT_TEST(itllevel);
    CPPUNIT_TEST(itlconnect);
    CPPUNIT_TEST(itlmask);
    CPPUNIT_TEST(itlwidth);
    
    // Note, can't yet test the LVDS stuff because it does not work
    // in a way I can comprehend (setting getting).
    
    CPPUNIT_TEST(dac);
    
    // Only 2745's support vgagain so we don't test it.
    
    CPPUNIT_TEST(offcalib);
    CPPUNIT_TEST(chen);
    CPPUNIT_TEST(chanstat);
    CPPUNIT_TEST(dcoffset);
    CPPUNIT_TEST(chgain);
    CPPUNIT_TEST(threshold);
    CPPUNIT_TEST(polarity);
    CPPUNIT_TEST(egate);
    CPPUNIT_TEST_SUITE_END();
    
private:
    VX2750Pha* m_pModule;
public:
    void setUp() {
        m_pModule = new VX2750Pha(connection.c_str(), isUsb);
    }
    void tearDown() {
        delete m_pModule;
    }
protected:
    void getcup();
    void getfpgafw();
    void getfwType();
    void getmodelCode();
    void getpiggycode();
    void getmodel();
    
    void getform();
    void getfamcode();
    void getserial();
    void getmotherrev();
    void getpiggyrev();
    void getlicense();
    void isLicensed();
    
    void timebomb();
    void channelCount();
    void bits();
    void samplerate();
    void inputRange();
    void inputsDifferential();
    void inputImpedance();
    void ip();
    void netmask();
    void gateway();
    
    void clocksource();
    void clockout();
    void startsource();
    void gbltrigger();
    void wavetrigger();
    void eventtrigger();
    void tsresetsrc();
    void chtrigmask();
    void tracerecordmode();
    void trgoutmode();
    void gpiomode();
    void busyinsrc();
    
    void syncout();
    void bvetosrc();
    void bvetowidth();
    void bvetopolarity();
    void chvetosrc();
    void chvetowid();
    void rundelay();
    void autodisarm();
    void multiwindow();
    void getters();
    void pausets();
    
    void vclockdelay();
    void pclockdelay();
    void wdatasource();
    void recl();
    void wresolution();
    void aprobe();
    void dprobe();
    void pretrigger;
    void tpulseperiod();
    void tpulsewidth();
    void tpulselow();
    void tpulsehigh();
    void iolevel();
    void monitor();
    void errorflags();
    void itlab();
    void pairlogic();
    void itllevel();
    void itlconnect();
    void itlmask();
    void itlwidth();
    
    void dac();
    
    void offcalib();
    void chen();
    void chanstat();
    void dcoffset();
    void chgain();
    void threshold();
    void polarity();
    void egate();
};

CPPUNIT_TEST_SUITE_REGISTRATION(vx2750phatest);

void vx2750phatest::getcup()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getCupVersion());
}
void vx2750phatest::getfpgafw()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getFpgaFWVersion());
}
void vx2750phatest::getfwType()
{
    VX2750Pha::FwType type;
    CPPUNIT_ASSERT_NO_THROW(type = m_pModule->getFirmwareType());
    ASSERT(type == VX2750Pha::DPP_PHA);
}
void vx2750phatest::getmodelCode()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getModelCode());
}
void vx2750phatest::getpiggycode()
{
    
    CPPUNIT_ASSERT_NO_THROW( m_pModule->getPiggyBackCode());
    
}
void vx2750phatest::getmodel()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getModelName());
}

void vx2750phatest::getform()
{
    VX2750Pha::FormFactor form;
    CPPUNIT_ASSERT_NO_THROW(form = m_pModule->getFormFactor());
    ASSERT(form == VX2750Pha::VME64X);
}
void vx2750phatest::getfamcode()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getFamilyCode());
}
void vx2750phatest::getserial()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getSerialNumber());
}
void vx2750phatest::getmotherrev()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getMotherboardRev());
}
void vx2750phatest::getpiggyrev()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getPiggybackRev());
}
void vx2750phatest::getlicense()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getLicense());
}
void vx2750phatest::isLicensed()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->isLicensed());    
}

void vx2750phatest::timebomb()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->remainingUnlicensedTime());
}
void vx2750phatest::channelCount()
{
    int nchan;
    CPPUNIT_ASSERT_NO_THROW(nchan = m_pModule->channelCount());
    EQ(64, nchan);
}
void vx2750phatest::bits()
{
    int bits;
    CPPUNIT_ASSERT_NO_THROW(bits = m_pModule->bitsOfResolution());
    EQ(16, bits);
}
void vx2750phatest::samplerate()
{
    int rate;
    CPPUNIT_ASSERT_NO_THROW(rate = m_pModule->sampleRate());
    EQ(125, rate);
}
void vx2750phatest::inputRange()
{
    int range;
    CPPUNIT_ASSERT_NO_THROW(range = m_pModule->inputRange());
    EQ(2, range);
}
void vx2750phatest::inputsDifferential()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->isDifferentialInput());
}
void vx2750phatest::inputImpedance()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->inputImpedance());
}

void vx2750phatest::ip()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->ipAddress());
}
void vx2750phatest::netmask()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->netMask());
}
void vx2750phatest::gateway()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->gateway());
}
/// get/set clock source:

void vx2750phatest::clocksource()
{
    // Gotta check 'em all.
    std::vector<VX2750Pha::ClockSource>  possibleSources =
        {VX2750Pha::Internal, VX2750Pha::FrontPanel 
        //, VX2750Pha::Clock_P0,   // Unimplemented.
        //VX2750Pha::DIPSelected
    };
    VX2750Pha::ClockSource original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getClockSource());
    
    for (auto source : possibleSources) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setClockSource(source));
        EQ(source, m_pModule->getClockSource());
    }
    m_pModule->setClockSource(original);
}


// Test get/set clock output (P0 and FP):
// P0 appears unimplemented?!?
void vx2750phatest::clockout()
{
    bool originalP0;
    bool originalFP;
    std::vector<bool> possible = {true, false};
    
    // CPPUNIT_ASSERT_NO_THROW(originalP0 = m_pModule->isClockOutOnP0());
    CPPUNIT_ASSERT_NO_THROW(originalFP = m_pModule->isClockOutOnFP());
    
    for (bool state : possible) {
        //CPPUNIT_ASSERT_NO_THROW(m_pModule->setClockOutOnP0(state));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setClockOutOnFP(state));
        //EQ(state, bool(m_pModule->isClockOutOnP0()));
        EQ(state, bool(m_pModule->isClockOutOnFP()));
    }
    // m_pModule->setClockOutOnP0(originalP0);
    m_pModule->setClockOutOnFP(originalFP);
}

// test get/set start source

void vx2750phatest::startsource()
{
    std::vector<VX2750Pha::StartSource> possible = {
        VX2750Pha::Start_EncodedClockIn, VX2750Pha::SINLevel, VX2750Pha::SINEdge,
        VX2750Pha::SWCommand, VX2750Pha::Start_LVDS, VX2750Pha::Start_P0
    };
    
    VX2750Pha::StartSource original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getStartSource());
    
    for (auto source : possible) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setStartSource(source));
        EQ(source, m_pModule->getStartSource());
    }
    m_pModule->setStartSource(original);
}
// set/get global trigger source.

void vx2750phatest::gbltrigger()
{
    std::vector<VX2750Pha::GlobalTriggerSource> possibles = {
        VX2750Pha::GlobalTrigger_TriggerIn, VX2750Pha::GlobalTrigger_P0,
        VX2750Pha::GlobalTrigger_Software, VX2750Pha::GlobalTrigger_LVDS,
        VX2750Pha::GlobalTrigger_InternalA,
        VX2750Pha::GlobalTrigger_InternalB, VX2750Pha::GlobalTrigger_InternalAandB,
        VX2750Pha::GlobalTrigger_InternalAorB, VX2750Pha::GlobalTrigger_EncodedClockIn,
        VX2750Pha::GlobalTrigger_GPIO,
        VX2750Pha::GlobalTrigger_TestPulse
    };
    VX2750Pha::GlobalTriggerSource original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getGlobalTriggerSource());
    for (auto source : possibles) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setGlobalTriggerSource(source));
        EQ(source, m_pModule->getGlobalTriggerSource());
    }
    m_pModule->setGlobalTriggerSource(original);
}

// set/get wave trigger source.

void vx2750phatest::wavetrigger()
{
    std::vector<VX2750Pha::WaveTriggerSource> possibles = {
        VX2750Pha::WaveTrigger_InternalA, VX2750Pha::WaveTrigger_InternalB,
        VX2750Pha::WaveTrigger_GlobalTriggerSource, VX2750Pha::WaveTrigger_TRGIN,
        VX2750Pha::ExternalInhibit,
        VX2750Pha::ADCUnderSaturation, VX2750Pha::ADCOverSaturation,
        // VX2750Pha::WaveTrigger_Software,   // Evidently uimplemented?
        VX2750Pha::WaveTrigger_ChannelSelfTrigger,
        VX2750Pha::WaveTrigger_AnyChannelSelfTrigger, VX2750Pha::WaveTrigger_Disabled
    };
    
    int nch = m_pModule->channelCount();
    VX2750Pha::WaveTriggerSource original;
    for (int i = 0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getWaveTriggerSource(i));
        for (auto source: possibles) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setWaveTriggerSource(i, source));
            EQ(source, m_pModule->getWaveTriggerSource(i));
        }
        m_pModule->setWaveTriggerSource(i, original);
    }
}

// Test set/get event trigger source.

void vx2750phatest::eventtrigger()
{
    std::vector<VX2750Pha::EventTriggerSource> possibles = {
        VX2750Pha::EventTrigger_InternalB, VX2750Pha::EventTrigger_InternalA,
        VX2750Pha::EventTrigger_GlobalTriggerSource, VX2750Pha::EventTrigger_TRGIN,
        // VX2750Pha::EventTrigger_Software,    // evidently unimplemented?
        VX2750Pha::EventTrigger_ChannelSelfTrigger,
        VX2750Pha::EventTrigger_AnyChannelSelfTrigger, VX2750Pha::EventTrigger_Disabled
    };
    // per channel parameter so:
    int nch = m_pModule->channelCount();
    VX2750Pha::EventTriggerSource original;
    
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getEventTriggerSource(i));
        for (auto source : possibles) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setEventTriggerSource(i, source));
            EQ(source, m_pModule->getEventTriggerSource(i));
        }
        m_pModule->setEventTriggerSource(i, original);
    }
}

// set/get per channel timestamp trigger mask.

void vx2750phatest::chtrigmask()
{
    int nch = m_pModule->channelCount();
    for (int i =0; i < nch; i++) {
        uint64_t original;
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getChannelTriggerMask(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setChannelTriggerMask(i, 0));
        EQ(std::uint64_t(0), m_pModule->getChannelTriggerMask(i));
        
        m_pModule->setChannelTriggerMask(i, 0xffffffffffffffff);
        EQ(std::uint64_t(0xffffffffffffffff), m_pModule->getChannelTriggerMask(i));
        
        m_pModule->setChannelTriggerMask(i, original);
        
    }
    
}
// set/get trace record mode:

void vx2750phatest::tracerecordmode()
{
    std::vector<VX2750Pha::TraceRecordMode> modes = {
        VX2750Pha::Always, VX2750Pha::OnRequest
    };
    
    int nch = m_pModule->channelCount();   // per channel parameter.
    VX2750Pha::TraceRecordMode original;
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getTraceRecordMode(i));
        
        for (auto mode : modes) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setTraceRecordMode(i, mode));
            EQ(mode, m_pModule->getTraceRecordMode(i));
        }
        m_pModule->setTraceRecordMode(i, original);
    }
    
}


// set/get triggerout mode.

void vx2750phatest::trgoutmode()
{
    std::vector<VX2750Pha::TRGOUTMode> modes = {
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
    VX2750Pha::TRGOUTMode original;

    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getTRGOUTMode());
    for (auto mode : modes) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setTRGOUTMode(mode));
        EQ(mode, m_pModule->getTRGOUTMode());
    }
    m_pModule->setTRGOUTMode(original);
}
// set/get/GPIO mode.

void vx2750phatest::gpiomode()
{
    std::vector<VX2750Pha::GPIOMode> modes = {
        VX2750Pha::GPIOMode_Disabled, VX2750Pha::GPIOMode_TriggerIn,
        VX2750Pha::GPIOMode_P0, VX2750Pha::GPIOMode_SIN, VX2750Pha::GPIOMode_LVDS,
        VX2750Pha::GPIOMode_InternalA,
        VX2750Pha::GPIOMode_InternalB, VX2750Pha::GPIOMode_InternalAandInternalB,
        VX2750Pha::GPIOMode_InternalAorInternalB, VX2750Pha::GPIOMode_EncodedClockIn,
        VX2750Pha::GPIOMode_SoftwareTrigger, VX2750Pha::GPIOMode_Run,
        VX2750Pha::GPIOMode_ReferenceClock,
        VX2750Pha::GPIOMode_TestPulse, VX2750Pha::GPIOMode_Busy,
        VX2750Pha::GPIOMode_Zero, VX2750Pha::GPIOMode_One
    };
    
    VX2750Pha::GPIOMode original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getGPIOMode());
    for (auto mode : modes) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setGPIOMode(mode));
        EQ(mode, m_pModule->getGPIOMode());
    }
}
// set/get busy input source.

void vx2750phatest::busyinsrc()
{
    std::vector<VX2750Pha::BusyInSource> sources = {
        VX2750Pha::BusyIn_SIN, VX2750Pha::BusyIn_GPIO,
        VX2750Pha::BusyIn_LVDS, VX2750Pha::BusyIn_Disabled 
    };
    
    VX2750Pha::BusyInSource original;
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getBusyInputSource());
    for (auto s : sources) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setBusyInputSource(s));
        EQ(s, m_pModule->getBusyInputSource());
    }
    m_pModule->setBusyInputSource(original);
}
// set/get sync out mode:

void vx2750phatest::syncout()
{
    std::vector<VX2750Pha::SyncOutMode> modes = {
        VX2750Pha::SyncOut_Disabled, VX2750Pha::SyncOut_SynchIn,
        VX2750Pha::SyncOut_TestPulse, VX2750Pha::InternalClock,
        VX2750Pha::SyncOut_Run
    };
    
    VX2750Pha::SyncOutMode original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getSyncOutMode());
    
    for (m : modes) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setSyncOutMode(m));
        EQ(m, m_pModule->getSyncOutMode());
    }
    m_pModule->setSyncOutMode(original);
}

// get/set board veto source:

void vx2750phatest::bvetosrc()
{
    std::vector<VX2750Pha::VetoSource> sources = {
        VX2750Pha::Veto_SIN, VX2750Pha::Veto_LVDS, VX2750Pha::Veto_GPIO,
        VX2750Pha::Veto_P0, VX2750Pha::Veto_EncodedClock,
        VX2750Pha::Veto_Disabled
    };
    VX2750Pha::VetoSource original;
    
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getBoardVetoSource());
    
    for (auto s : sources) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setBoardVetoSource(s));
        EQ(s, m_pModule->getBoardVetoSource());
    }
    m_pSetBoardVetoSource(original);
}
// board veto width - likely granular:
void vx2750phatest::bvetowidth()
{
    std::uint64_t original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getBoardVetoWidth());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setBoardVetoWidth(std::uint64_t(0)));
    EQ(std::uint64_t(0), m_pModule->getBoardVetoWidth());
    
    m_pModule->setBoardVetoWidth(std::uint64_t(34359738359));
    ASSERT(std::llabs(std::uint64_t(34359738359) - m_pModule->getBoardVetoWidth()) < 8);                        
    
    m_pModule->setBoardVetoWidth(original);
}
// board veto polarity:

void vx2750phatest::bvetopolarity()
{
    std::vector<VX2750Pha::VetoPolarity> pols = {
        VX2750Pha::ActiveHigh, VX2750Pha::ActiveLow
    };
    VX2750Pha::VetoPolarity original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getBoardVetoPolarity());
    for (auto p : pols) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setBoardVetoPolarity());
        EQ(p, m_pModule->getBoardVetoPolarity());
    }
    m_pModule->setBoardVetoPolarity(original);
}

// channel veto source

void vx2750phatest::chvetosrc()
{
    std::vector<VX2750Pha::ChannelVetoSource> sources = {
        VX2750Pha::BoardVeto, VX2750Pha::OverSaturation,
        VX2750Pha::UnderSaturation, VX2750Pha::ChanVeto_Disabled
    };
    VX2750Pha::ChannelVetoSource original;
    
    // per channel obviously:
    
    int nch = m_pModule->channelCount();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getChannelVetoSource(i));
        for (auto s : sources) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setChannelVetoSource(i, s));
            EQ(s, m_pModule->getChannelVetoSource(i));
        }
        m_pModule->setChannelVetoSource(i, original);
    }
}
// ADC Veto width:

void vx2750phatest::chvetowid()
{
    std::uint32_t original;
    int nch = m_pModule->channelCount();
    
    for (int i = 0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getChannelVetoWidth(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setChannelVetoWidth(i, 0));
        EQ(0, m_pModule->getChannelVetoWidth());
        // maybe granular so:
        
        m_pModule->setChannelVetoWidth(i, 524279);
        ASSERT(std::labs(std::uint32_t(524279) -  m_pModule->getChannelVetoWidth(i)) < 8);
        
        m_pModule->setChannelVetoWidth(i, original);
    }
    
}
// get/set run delay. 0 is probably 0 but other values might be granular.

void vx2750phatest::rundelay();
{
    std::uint32_t original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getRunDelay());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setRunDelay(0));
    EQ(0, m_pModule->getRunDelay());
    
    m_pModule->setRunDelay(524279);
    ASSERT(labs(524279 - m_pModule->getRunDelay()) < 8);
}

/// get/set auto disarm enable.

void vx2750phatest::autodisarm()
{
    bool original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->isAutoDesarmEnabled());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setAutoDisarmEnabled(true));
    ASSERT(m_pModule->isAutoDisarmEnabled());
    
    m_pModule->setAutoDisarmEnabled(false);
    ASSERT(!m_pModule->isAutoDisarmEnabled());
    
    m_pModule->setAutoDisarmEnabled(original);
}
// multiwindow mode:

void vx2750phaTest::multiwindow()
{
    bool original;
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->isMultiWindowRunEnabled());
    
    CPPUNIT_ASSERT_NO_THROW(m_Module->setMultiWindowModeEnable(true));
    ASSERT(m_pModule->isMultiWindowRunEnabled());
    
    m_pModule->setMultiWindowModeEnable(false);
    ASSERT(!m_pModule->isMultiWindowRunEnabled());
}
// pause timestamp hold enabled:

void vx2750phatest::pausets()
{
    bool original
    CPPUNIT_ASSERT_NO_THROW(original = m_pModule->isPauseTimestampHoldEnabled());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setPauseTimestampHold(true));
    ASSERT(m_pModule->isPauseTimestampHoldEnabled());
    
    m_pModule->setPauseTimestampHold(false);
    ASSERT(!m_pModule->isPauseTimestampHoldEnabled());
    
    m_pModule->setPauseTimestampHold(original);
}


// Geters we don't have much control over:

void vx2750phatest::getters()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getLEDStatus());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getAcquisitionStatus());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getMaxRawDataSize());
}
// clock delays

void vx2750phatest::vclockdelay()
{
    double max = 18888.888;
    double old;
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getVolatileClockDelay());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setVolatileClockDelay(-max));
    ASSERT(std::abs((-max) - m_pModule->getVolatileClockDelay()) < 1.0);
    
    m_pModule->setVolatileClockDelay(max);
    ASSERT(std::abs(max - m_pModule->getVolatileClockDelay()) < 1.0);
    
    m_pModule->setVolatileClockDelay(old);
}


void vx2750phatest::pclockdelay()
{
    double max = 18888.888;
    double old;
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getPermanentClockDelay());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setPermanentClockDelay(-max));
    ASSERT(std::abs((-max) - m_pModule->getPermanentClockDelay()) < 1.0);
    
    m_pModule->setPermanentClockDelay(max);
    ASSERT(std::abs(max - m_pModule->getPermanentClockDelay()) < 1.0);
    
    m_pModule->setPermanentClockDelay(old);
}

// wave data source
void vx2750phatest::wdatasource()
{
    std::vector<VX2750Pha::WaveDataSource> sources = {
        ADC_DATA, ADC_TEST_TOGGLE, ADC_TEST_RAMP, ADC_SIN, WaveSource_IPE,
        WaveSource_Ramp, SquareWave,
        ADC_TEST_PRBS
    };
    VX2750Pha::WaveDataSource old;
    int nch = m_pModule->channelCount;
    for  (int i = 0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getWaveDataSource(i));
        for (auto s: sources) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setWaveDataSource(i, s));
            EQ(s, m_pModule->getWaveDataSource(i));
        }
        m_pModule->setWaveDataSource(old);
    }
    
}
// Record length (samples/ns).

void vx2750phatest::recl()
{
    int original;
    int nch = m_pModule->channelCount();
    for (int i = 0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getRecordSamples(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setRecordSamples(i, 4));
        EQ(4, m_pModule->getRecordSamples(i));
        m_pSetRecordSamples(i, 8100);
        EQ(8100, m_pModule->getRecordSamples(i));
        m_pModule->setRecordSamples(i, original);
        
        // ns might be granular guessing 8's because of the multiplier
        // between Min samples/ns.
        
        CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getRecordNs(i));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setRecordNs(i, 32));
        ASSERT(std::labs(32 - m_pModule->getRecordNs(i)) < 8);
        m_pModule->setRecordNs(i, 64800);
        ASSERT(std::labs(64800 - m_pModule->getRecordNs(i)) < 8);
        m_pModule->setRecordNs(i, original);
    }
}
// wave resolution

void vx2750phatest::wresolution()
{
    std::vector<VX2750Pha::WaveResolution> options = {
        Res8, Res16, Res32, Res64
    };
    VX2750Pha::WaveResolution old;
    int nch = m_pModule->channelCount();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getWaveResolution(i));
        for (auto o : options) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setWaveResolution(i, o));
            EQ(o, m_pModule->getWaveResolution(i));
        }
        m_pModule->setWaveResolution(i, old);
    }
}
// probes:

void vx2750phatest::aprobe()
{
    std::vector<VX2750Pha::AnalogProbe> options = {
        ADCInput, TimeFilter, EnergyFilter, EnergyFilterBaseline,
        EnergyFilterMinusBaseline;

    };
    VX2750Pha::AnalogProbe original;
    int nch = m_pModule->channelCount();
    const int numProbes=2;
    
    for (int i =0; i < nch; i++) {
        for (int p = 0; p < numProbes; p++) {
            CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getAnalogProbe(i, p));
            for (auto o : options) {
                CPPUNIT_ASSERT_NO_THROW(m_pModule->setAnalogProbe(i, p, o));
                EQ(o, m_pModule->getAnalogProbe(i, p)):
            }
            m_pModule->setAnalogProbe(i, p, original);
        }
    }
}
void vx2750phatest::dprobe()
{
    std::Vector<VX2750Pha::DigitalProbe> options = {
        DProbe_Trigger, TimeFilterArmed, ReTriggerGuard, EneryFilterBaselineFreeze,
        EnergyFilterPeaking, EnergyFilterPeakReady, EnergyFilterPileupGuard,
        EventPileup, ADCSaturation, ADCSaturationProtection, PostSaturationEvent,
        EnergyFilterSaturation, AcquisitionInhibit
    };
    VX2750Pha::DigitalProbe original;
    int nch = m_pModule->channelCount();
    const int nprobes=4;
    
    for (int i = 0; i < nch; i++) {
        for (int p = 0; p < nprobes; p++) {
            CPPUNIT_ASSERT_NO_THROW(original = m_pModule->getDigitalProbe(i, p));
            for (auto o : options) {
                CPPUNIT_ASSERT_NO_THROW(m_pModue->setDigitalProbe(i, p, o));
                EQ(o, m_pModule->getDigitalProbe(i, p));
            }
            m_pModule->setDigitalProbe(i, p, original);
        }
    }
}
//Pretrigger (samples and ns).

void vx2750phatest::pretrigger()
{
    int minsamples = 4;
    int maxsamples = 4000;
    int minns      = 8;
    int maxns      = 32000;
    
    int old;
    int nch = m_pModule->channelCount();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getPreTriggerSamples(i));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setPreTriggerSamples(i, minsamples));
        EQ(minsamples, m_pModule->getPreTriggerSamples(i));
        m_pModule->setPreTriggerSamples(i, maxsamples);
        EQ(maxsamples, m_pModule->getPreTriggerSamples(i));   // samples should be precise.
        m_pModule->setPreTriggerSamples(i, old);
        
        // time might be granular in 8 ns clumps?
        
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getPreTriggerNs(i));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setPreTriggerNs(i, minns));
        EQ(minns, m_pModule->getPreTriggerNs(i));
        m_pModule->setPreTriggerNs(i, maxns);
        ASSERT(std::labs(maxns - m_pModule->getPreTriggerNs(i)) < 8);
        m_pModule->setPreTriggerns(i, old);
        
    }
}

// Test pulse properties:

void vx2750phatest::tpulseperiod()
{
    const std::unint64_t max = 34359738360;
    
    // Granular in 8ns units.
    
    std::uint64_t old;
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getTestPulsePeriod());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setTestPulsePeriod(0));
    EQ(std::uint64_t(0), m_pModule->getTestPulsePeriod());
    m_pModule->setTestPulsePeriod(max);
    EQ(max, m_pModule->getTestPulsePeriod());
    m_pModule->setTestPulsePeriod(old);
}
void vx2750phatest::tpulsewidth();
{
    const std::uint64_t max = 34359738360;
    std::uint64_t old;
    
    // Granular in 8ns units but 0 is 0.
    
    CPPUNIT_ASSERT_NO_THORW(old = m_pModule->getTestPulseWidth());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setTestPulseWidth(0));
    EQ(std::uint64_t(0), m_pModule->getTestPulseWidth());
    m_pModule->setTestPulseWidth(max);
    EQ(max, m_pModule->getTestPulseWidth());
    m_pModule->setTestPulseWidth(old);
}

void vx2750phatest::tpulselow()
{
    const std::uint32_t max = 65535;
    std::uint32_t old;
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getTestPulseLowLevel());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setTestPulseLowLevel(0));
    EQ(std::uint32_t(0), m_pModule->getTestPulseLowLevel());
    m_pModule->setTestPulseLowLevel(max);
    EQ(max, m_pModule->getTestPulseLowLevel());
    m_pModule->setTestPulseLowLevel(old);
}

void vx2750phatest::tpulsehigh()
{
    const std::uint32_t max = 65535;
    std::uint32_t old;
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getTestPulseHighLevel());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setTestPulseHighLevel(0));
    EQ(std::uint32_t(0), m_pModule->getTestPulseHighLevel());
    m_pModule->setTestPulseHighLevel(max);
    EQ(max, m_pModule->getTestPulseHighLevel());
    m_pModule->setTestPulseHighLevel(old);
}
// I/O level for lemmos

void vx2750phatest::iolevel()
{
    std::vector<VX2750Pha::IOLevel> levels = {
        TTL, NIM
    };
    VX2750Pha::IOLevel old;
    CPPUNIT_ASSERT_NO_THROW(old = m_Module->getIOLevel());
    for (auto l : levels) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setIOLevel(l));
        EQ(l, m_pModule->getIOLevel());
    }
    m_pModule->setIOLevel(old);
}

// Ensure we can get stuff from the various health monitors:

void vx2750phatest::monitor()
{
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getAirInTemp());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getAirOutTemp());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getCoreTemp());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getFirstADCTemp());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getLastADCTemp());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getHottestADCTemp());
    for (int i = 0; i < 8; i++) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->getADCTemp(i));
    }
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDCDCConverterTemp());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDCDCConverterInputVoltage());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDCDCConverterOutputVoltage());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDCDCConverterAmps());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDCDCConverterHz());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDCDCConverterDutyCycle());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getFanSpeed(0));
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getFanSpeeed(1));
    
}
// Error flag masks etc... note there are 17 bits.

void vx2750phatest::errorflags()
{
    const std::uint32_t allbits = 0x1ffff;  // 17 bits set.
    std::uint32_t old;
    
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getErrorFlagMask());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setErrorFlagMask(0));
    EQ(std::uint32_t(0), m_pModule->getErrorFlagMask());
    m_pModule->setErrorFlagMask(allbits);
    EQ(allbits, m_pModule->getErrorFlagMask());
    m_pModule->setErrorFlagMask(old);
    
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getErrorFlagDataMask());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setErrorFlagDataMask(0));
    EQ(std::uint32_t(0), m_pModule->getErrorFlagDataMask());
    m_pModule->setErrorFlagDataMask(allbits);
    EQ(allbits, m_pModule->getErrorFlagDataMask());
    m_pModule->setErrorFlagDataMask(old);
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getErrorFLags());
    bool ready;
    CPPUNIT_ASSERT_NO_THROW(ready = m_pModule->isBoardReady());
    ASSERT(ready);
    
}
// ITL blocks (trigger logic).

void vx2750phatest::itlab()
{
    std::vector<VX2750PHa::IndividualTriggerLogic> modes = {
        ITL_OR, ITL_AND, Majority
    };
    VX2750PHa::IndividualTriggerLogic old;
    
    // Mainlogic:
    
    for (auto m : modes) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getITLAMainLogic());
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAMainLogic(m));
        EQ(m, m_pModule->getITLAMainLogic());
        m_pModule->setITLAMainLogic(old);
        
        CPPUNIT_ASSERT_NO_THROW(old - m_pModule->getITLBMainLogic());
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBMainLogic(m));
        EQ(m, m_pModule->getITLAMainLogic());
        m_pModule->setITLBMainLogic(old);
    }
    // Majority level a:
    
    unsigned oldmaj;
    CPPUNIT_ASSERT_NO_THROW(oldmaj = m_pModule->getITLAMajorityLevel());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAMajorityLevel(0));
    EQ(0, m_pModule->getITLAMajorityLevel());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAMajorityLevel(63));
    EQ(63, m_pModule->getITLAMajorityLevel());
    
    // Majority level b:
    
    CPPUNIT_ASSERT_NO_THROW(oldmaj = m_pModule->getITLBMajorityLevel());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBMajorityLevel(0));
    EQ(0, m_pModule->getITLBMajorityLevel());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBMajorityLevel(63));
    EQ(63, m_pModule->getITLBMajorityLevel());
    
    
}
// Pair trigger logic:

void vx2750phatest::pairlogic()
{
    std::vector<VX2750Pha::PairTriggerLogic> modes = {
        PTL_AND, PTL_OR, NONE
    };
    VX2750Pha::PairTriggerLogic olda;
    VX2750Pha::PairTriggerLogic oldb;
    
    CPPUNIT_ASSERT_NO_THROW(olda = m_pModule->getITLAPairLogic);
    CPPUNIT_ASSERT_NO_THROW(oldb = m_pModule->getITLBPairLogic);
    
    for (auto m : modes) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAPairLogic(m));
        EQ(m, m_pModule->getITLAPairLogic());
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBPairLogic(m));
        EQ(m, m_pModule->getITLBPairLogic());
    }
    m_pModule->setITLAPairLogic(olda);
    m_pModule->setITLBPairLogic(oldb);
    
}

void vx2750phatest::itllevel()
{
    bool now;
    CPPUNIT_ASSERT_NO_THROW(now = m_pModule->isITLAInverted());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAInverted(true));
    ASSERT(m_pModule->isITLAInverted());
    m_pModule->setITLAInverted(false);
    ASSERT(!m_pModule->isITLAInverted());
    m_pModule->setITLAInverted(now);
    
    CPPUNIT_ASSERT_NO_THROW(now = m_pModule->isITLBInverted());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBInverted(true));
    ASSERT(m_pModule->isITLBInverted());
    m_pModule->setITLBInverted(false);
    ASSERT(!m_pModule->isITLBInverted());
    m_pModule->setITLBInverted(now);
    
}
void vx2750phatest::itlconnect()
{
    std::vector<VX2750Pha::ITLConnect> modes = {
        ITL_Disabled, ITL_ITLA, ITL_ITLB
    };
    VX2750Pha::ITLConnect old;
    unsigned nch = m_pModule->channelCount();
    
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getITLConnect(ch));
        for (auto m : modes) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLConnect(ch, m));
            EQ(m, m_pModule->getITLConnect(ch));
        }
        m_pModule->setITLConnect(ch, old);
    }
}
void vx2750phatest::itlmask()
{
    // Setting the mask, ovewrites the ITLConnect settings according to the
    // manual so we need to save/restore those for all channels!!
    
    std::vector<VX2750Pha::ITLConnect> old;
    unsigned nch;
    nch = m_pModule->numChannels();
    for (int i =0; i < nch; i++) {
        old.push_back(m_pModule->getITLConnect(i));
    }
    // Play withthe ITL Masks:
    
    std::uint64_t oldmaska;
    std::uint64_t oldmaskb;
    CPPUNIT_ASSERT_NO_THROW(oldmaska = m_pModule->getITLAMask());
    CPPUNIT_ASSERT_NO_THROW(oldmaskb = m_pModule->getITLBMask());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAMask(0));
    EQ(std::uint64_t(0), m_pModule->getITLAMask());
    m_pModule->setITLAMask(0xffffffffffffffff);
    EQ(std::uint64_t(0xffffffffffffffff), m_pModule->getITLAMask());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBMask(0));
    EQ(std::uint64_t(0), m_pModule->getITLBMask());
    m_pModule->setITLBMask(0xffffffffffffffff);
    EQ(std::uint64_t(0xffffffffffffffff), m_pModule->getITLBMask());
    
    m_pModule->setITLAMask(oldmaska);
    m_pModule->setITLBMask(oldmaskb);
    
    // Restore the ITLConnect values:
    
    for (int i=  0; i < nch; i++) {
        m_pModule->setITLConnect(i, old[i]);
    }
}
void vx2750phatest::itlwidth()
{
    const std::uint32_t max = ns524280;
    
    std::uint32_t olda;
    std::uint32_t oldb;
    
    CPPUNIT_ASSERT_NO_THROW(olda = m_pModule->getITLAGateWidth());
    CPPUNIT_ASSERT_NO_THROW(oldb = m_pModule->getITLBGateWidth());
    
    // 8ns granlarity:
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAGateWidth(0));
    EQ(std::uint32_t(0), m_pModule->getITLAGateWidth());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLAGateWidth(max));
    ASSERT(std::labs(max - m_pModule->getITLAGateWidth()) < 8);
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBGateWidth(0));
    EQ(std::uint32_t(0), m_pModule->getITLBGateWidth());
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setITLBGateWidth(max));
    ASSERT(std::labs(max - m_pModule->getITLBGateWidth()) < 8);
    
    
    m_pModule->setITLAGateWidth(olda);
    m_pModule->setITLBGateWidth(oldb);
}
// Test various DAC control functions.

void vx2750phatest::dac()
{
    std::vector<VX2750Pha::DACOutputMode> modes = {
        Static, DACOut_IPE, DACOut_ChInput, MemOccupancy, ChSum, OverThrSum,
        DACOut_Ramp, Sine5MHz, Square
    };
    VX2750Pha::DACOutputMode old;
    
    // Mode
    
    CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getDACOutputMode());
    
    for (auto m : modes) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setDACOutputMode(m));
        EQ(m, m_pModule->getDACOutputMode());
    }
    
    m_pModule->setDACOutputMode(old);
    
    // Value [static level]
    
    const std::uint16_t maxvalue = 16383;
    std::uint16_t oldlevel;
    CPPUNIT_ASSERT_NO_THROW(m_pModule->getDACOutValue());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setDACOutValue(0));
    EQ(std::uint16_t(0), m_pModule->getDACOutValue());
    m_pModule->setDACOutValue(maxvalue);
    EQ(maxvalue, m_pModule->getDACOutValue());
    
    m_pModule->setDACOutValue(oldlevel);
    
    // Channel the DAC outputs reproduce in that mode:
    
    unsigned ch;
    unsigned nch = m_pModule->numChannels();
    CPPUNIT_ASSERT_NO_THROW(ch = m_pModule->getDACChannel());
    
    CPPUNIT_ASSERT_NO_THROW(m_pModule->setDACChannel(0));
    EQ(unsigned(0), m_pModule->getDACChannel());
    m_pModule->setDACChannel(nch-1);
    EQ(nch-1, m_pModule->getDACChannel());
    
    m_pModule->setDACChannel(ch);
    
    // Just in case mucking with levels and channels resets the mode:
    
    m_pModule->setDACOutputMode(old);
}
// offset calibration on/off.

void vx2750phatest::offcalib()
{
    bool was;
    unsigned nch = m_pModule->numChannels();
    for (int i = 0; i < nch; i++ ) {
        CPPUNIT_ASSERT_NO_THROW(was = m_pModule->isOffsetCalbrationEnabled(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->enableOffsetCalibration(i, true));
        ASSERT(m_pModule->isOffsetCalibrationEnabled(i));
        m_pModule->enableOffsetCalibration(i, false);
        ASSERT(!m_pModule->isOffsetCalibrationEnabled(i));
        
        m_pModule->enbleOffsetCalibration(i, was);
    }
}
// Enable/disable channel

void vx2750phatest::chen()
{
    bool old;
    unsigned nch = m_pModule->numChannels();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->isChannelEnabled(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->enableChannel(i, true));
        ASSERT(m_pModule->isChannelEnabled(i));
        m_pModule->enableChannel(i, false);
        ASSERT(!m_pModule->isChannelEnabled(i));
        
        m_pModule->enableChannel(i, old);
    }
}

// readonly channel status:

void vx2750phatest::chanstat() {
    unsigned nch = m_pModule->numChannels();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->getChannelStatus(i));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->getSelfTriggerRate(i));
    }
}
// set/get the DC offset added to the input signal:
// Likely granular (1% level?)
void vx2750phatest::dcoffset() {
    double  oldpct;
    unsigned nch = m_pModule->numChannels();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(oldpct = m_pModule->getDCOffset(i));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setDCOffset(i, 0.0)); // probably exact.
        EQ(0.0, m_pModule->getDCOffset(i));
        
        m_pModule->setDCOffset(i, 50.0);
        ASSERT(std::abs(50.0 - m_pModule->getDCOffset(i)) <= 1.0);
        
        m_pModule->setDCOffset(i, oldpct);
    }
}
// gain factors are readonly so:
void vx2750phatest::chgain() {
    unsigned nch = m_pModule->numChannels();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(m_pModule->getGainFactor(i));
    }
}
// Trigger threshold.

void vx2750phatest::threshold() {
    unsigned nch = m_pModule->numChannels();
    std::uint32_t max = 8191;
    std::uint32_t old;
    
    for (int i = 0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(old = m_pModule->getTriggerThreshold(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setTriggerThreshold(i, 0));
        EQ(std::uint32_t(0), m_pModule->getTriggerThreshold(i));
        
        m_pModule->setTriggerThreshold(i, max);
        EQ(max, m_pModule->getTriggerThreshold(i));
        
        m_pModule->setTriggerThreshold(old);
        
    }
}
void vx2750phatest::polarity() {
    std::vector<VX2750Pha::Polarity> pols = {
        Positive, Negative
    };
    VX2750Pha::Polarity old;
    unsigned nch = m_pModule->numChannels();
    
    for (int i =0; i < nch; i++) {
        CPUNIT_ASSERT_NO_THROW(old =m_pModule->getPulsePolarity(i));
        
        for (auto p : pols) {
            CPPUNIT_ASSERT_NO_THROW(m_pModule->setPulsePolarity(i, p));
            EQ(p, m_pModule->getPulsePolarity(i));
        }
        
        m_pModule->setPulsePolarity(i, old);
    }
    
}
// energy skim is essentially an energy acceptance window.
void vx2750phatest::egate() {
    std::uint16_t max = 65534;
    std::uint16_t oldlow, oldhigh;
    
    unsigned nch = m_pModule->numChannels();
    for (int i =0; i < nch; i++) {
        CPPUNIT_ASSERT_NO_THROW(oldlow = m_pModule->getEnergySkimLowDiscriminator(i));
        CPPUNIT_ASSERT_NO_THROW(oldhigh = m_pModule->getEnergySkimHighDiscrimnator(i));
        
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setEnergySkimLowDiscriminator(i, 0));
        CPPUNIT_ASSERT_NO_THROW(m_pModule->setEnergySkimHighDiscriminator(i, 0));
        EQ(std::uint16_t(0), m_pModule->getEnergySkimLowDicriminator(i));
        EQ(std::uint16_t(0), m_pModule->getEnergyHighLowDicriminator(i));
        
        m_pModule->setEnergySkimLowDiscriminator(i, max);
        m_pModule->setEnergySkimHighDiscriminator(i, max);
        EQ(max, m_pModule->getEnergySkimLowDiscriminator(i)):
        EQ(max, m_pModule->getEnergySkimHighDiscriminator(i)):
        
        m_pModule->setEnergySkimHighDiscrimnator(i, oldhigh);
        m_pModule->setEnergySkimLowDiscriminator(i, oldlow);
    }
}