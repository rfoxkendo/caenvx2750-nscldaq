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