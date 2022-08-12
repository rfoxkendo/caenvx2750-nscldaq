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