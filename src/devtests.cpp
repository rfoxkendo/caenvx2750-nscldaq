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
    CPPUNIT_TEST(test_1);
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
    void test_1();
};

CPPUNIT_TEST_SUITE_REGISTRATION(devtest);

void devtest::test_1()
{
}