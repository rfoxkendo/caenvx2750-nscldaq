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

/** @file:  devtests.cpp
 *  @brief: Test suite for Dig2Device class.
 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Asserter.h>
#include "Asserts.h"

#include "Dig2Device.h"
// Connection parameters:
extern std::string connection;
extern bool        isUsb;

class devtest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(devtest);
  CPPUNIT_TEST(serial);    // Base parameter getting checks.
  CPPUNIT_TEST(getnum);
  CPPUNIT_TEST(getfloat);
  CPPUNIT_TEST(isPHA);

  CPPUNIT_TEST(devstring); // Convenience functions for
  CPPUNIT_TEST(devnum);    // device level parameters.
  CPPUNIT_TEST(devfloat);

  CPPUNIT_TEST(chanstring); // Convenience functions for
  CPPUNIT_TEST(channum);    // channel level params.
  CPPUNIT_TEST(chanfloat);

  // Set at top level (need full path).

  CPPUNIT_TEST(setclocksrc);
  CPPUNIT_TEST(setvetowidth);
  CPPUNIT_TEST(enclockFP);       // boolean
  
  // Set at device level:
  
  CPPUNIT_TEST(devsetclocksrc);
  CPPUNIT_TEST(devsetvetowidth);
  CPPUNIT_TEST(devenclockFP);
  CPPUNIT_TEST_SUITE_END();
    
private:
    caen_nscldaq::Dig2Device* m_pConnection;
public:
    void setUp() {
        m_pConnection = new caen_nscldaq::Dig2Device(connection.c_str(), isUsb);
    }
    void tearDown() {
        delete m_pConnection;
    }
protected:
  void serial();
  void getnum();
  void getfloat();
  void isPHA();

  void devstring();
  void devnum();
  void devfloat();

  void chanstring();
  void channum();
  void chanfloat();

  void setclocksrc();
  void setvetowidth();
  void enclockFP();
  
  void devsetclocksrc();
  void devsetvetowidth();
  void devenclockFP();
};

CPPUNIT_TEST_SUITE_REGISTRATION(devtest);

// Read serial - it's the same as the PID -- assumes
// a usb connection.

void devtest::serial()
{
  
    std::string ser;
    CPPUNIT_ASSERT_NO_THROW(
	 ser = m_pConnection->GetValue("/par/SerialNum")
    );
  if (isUsb)  EQ(connection, ser);
}

// Number of channels is an integer:

void devtest::getnum()
{
  int chans;
  CPPUNIT_ASSERT_NO_THROW(
     chans = m_pConnection->GetInteger("/par/NumCh")
  );
  EQ(64, chans);
}


// The volatile and permanent clock delays  is a float but
// damned if I know what to check it against.
void devtest::getfloat()
{
  float vol, perm;
  CPPUNIT_ASSERT_NO_THROW(
      vol = m_pConnection->GetReal("/par/VolatileClockOutDelay")
  );
  
}

// We're going to use dpp-pha parameter so we need to know
// that's the firmware type:

void devtest::isPHA()
{
  std::string fwType = m_pConnection->GetValue("/par/FwType");
  EQ(std::string("DPP_PHA"), fwType);
}

void devtest::devstring()
{
  std::string ser;
  CPPUNIT_ASSERT_NO_THROW(
    ser = m_pConnection->GetDeviceValue("SerialNum")
  );
  if (isUsb) EQ(connection ,ser);
}
void devtest::devnum() {
  int chans;
  CPPUNIT_ASSERT_NO_THROW(
     chans = m_pConnection->GetDeviceInteger("NumCh")
  );
  EQ(64, chans);
}
void devtest::devfloat()
{
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->GetDeviceReal("VolatileClockOutDelay"));
}
void devtest::chanstring()
{
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->GetChanValue(0,"EventTriggerSource"));
}

void devtest::channum()
{
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->GetChanInteger(0, "ADCVetoWidth"));
}

void devtest::chanfloat()
{
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->GetChanReal(0, "DCOffset"));
}

void devtest::setclocksrc()
{
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetValue("/par/ClockSource", "FPClkIn"));
  EQ(std::string("FPClkIn"), m_pConnection->GetDeviceValue("ClockSource"));

  // Set it to internal:

  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetValue("/par/ClockSource", "Internal"));
  EQ(std::string("Internal"), m_pConnection->GetDeviceValue("ClockSource"));
}
void devtest::setvetowidth()
{
  int original = m_pConnection->GetDeviceInteger("BoardVetoWidth");

  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetValue("/par/BoardVetoWidth", 0));
  EQ(0, m_pConnection->GetDeviceInteger("BoardVetoWidth"));
  m_pConnection->SetValue("/par/BoardVetoWidth", 1232); // granular
  EQ(1232, m_pConnection->GetDeviceInteger("BoardVetoWidth"));
  m_pConnection->SetValue("/par/BoardVetoWidth", original);
  EQ(original, m_pConnection->GetDeviceInteger("BoardVetoWidth"));
			  
  
}

void devtest::enclockFP()
{
  bool original = m_pConnection->GetDeviceBool("EnClockOutFP");
  
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetValue("/par/EnClockOutFP", true));
  EQ(true, m_pConnection->GetDeviceBool("EnClockOutFP"));
  
  m_pConnection->SetValue("/par/EnClockOutFP", false);
  EQ(false, m_pConnection->GetDeviceBool("EnClockOutFP"));
  
  m_pConnection->SetValue("/par/EnClockOutFP", original);
}


//
void devtest::devsetclocksrc()
{
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetDeviceValue("ClockSource", "FPClkIn"));
  EQ(std::string("FPClkIn"), m_pConnection->GetDeviceValue("ClockSource"));

  // Set it to internal:

  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetDeviceValue("ClockSource", "Internal"));
  EQ(std::string("Internal"), m_pConnection->GetDeviceValue("ClockSource"));
}
void devtest::devsetvetowidth()
{
  int original = m_pConnection->GetDeviceInteger("BoardVetoWidth");

  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetDeviceValue("BoardVetoWidth", 0));
  EQ(0, m_pConnection->GetDeviceInteger("BoardVetoWidth"));
  m_pConnection->SetDeviceValue("BoardVetoWidth", 1232); // granular
  EQ(1232, m_pConnection->GetDeviceInteger("BoardVetoWidth"));
  m_pConnection->SetDeviceValue("BoardVetoWidth", original);
  EQ(original, m_pConnection->GetDeviceInteger("BoardVetoWidth"));
			  
  
}

void devtest::devenclockFP()
{
  bool original = m_pConnection->GetDeviceBool("EnClockOutFP");
  
  CPPUNIT_ASSERT_NO_THROW(m_pConnection->SetDeviceValue("EnClockOutFP", true));
  EQ(true, m_pConnection->GetDeviceBool("EnClockOutFP"));
  
  m_pConnection->SetDeviceValue("EnClockOutFP", false);
  EQ(false, m_pConnection->GetDeviceBool("EnClockOutFP"));
  
  m_pConnection->SetDeviceValue("EnClockOutFP", original);
}