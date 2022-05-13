/*
*-------------------------------------------------------------
 
 CAEN SpA 
 Via Vetraia, 11 - 55049 - Viareggio ITALY
 +390594388398 - www.caen.it

------------------------------------------------------------

**************************************************************************
* @note TERMS OF USE:
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation. This program is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The user relies on the 
* software, documentation and results solely at his own risk.
*
* @file     VX2750PHAConfiguration.cpp
* @brief    Implement the PHA configuration class.
* @author   Ron Fox
*
*/

#include "VX2750PHAConfiguration.h"
#include <string>

namespace caen_nscldaq {
    
/**
 *  constructor (default)
 * @param name - name of the object to hand off to the base class.
 *               this is used by various frameworks to look up the object.
 */
VX2750PHAModuleConfiguration::VX2750PHAModuleConfiguration(const char* name) :
    XXUSB::XXUSBConfigurableObject(std::string(name))
{
    defineReadoutOptions();
    defineGeneralOptions();
    defineAcqTriggerOptions();
}

/**
 * constructor (copy)
 *    Since we have no data of our own, this is delegated fully to the base
 *    class
 *
 *  @param rhs - the object we're trying to copy construct.
 */
VX2750PHAModuleConfiguration::VX2750PHAModuleConfiguration(
    const VX2750PHAModuleConfiguration& rhs
) : XXUSB::XXUSBConfigurableObject(rhs)
{}

 /**
  * destructor
  *    Only need this for destructor chaining (to fill in the virtual
  *    method table).
  */
VX2750PHAModuleConfiguration::~VX2750PHAModuleConfiguration()
{}
    
 /**
  * assignment - again can delegate to the base class
  * @param rhs - the object being assigned to this.
  * @return VX2750PHAModuleConfiguration& (*this).
  */
VX2750PHAModuleConfiguration&
VX2750PHAModuleConfiguration::operator=(const VX2750PHAModuleConfiguration& rhs)
{
   XXUSB::XXUSBConfigurableObject::operator=(rhs);
   return *this;
}
/**
 * equality comparison
 *   @param rhs - object we're comparing to.
 *   @return int - nonzero if the two are equal.
 */
int
VX2750PHAModuleConfiguration::operator==(const VX2750PHAModuleConfiguration& rhs)
{
  return XXUSB::XXUSBConfigurableObject::operator==(rhs);
}
/**
 * inequality comparison.
 *   Rather than delegating we reuturn the boolean inverse of operator==
 *   in this way we're still good if we decide we need our own local data:
 * @param rhs - object we're comparing to.
 * @return int - nonzero if rhs is not equal to this.
 */
int
VX2750PHAModuleConfiguration::operator!=(const VX2750PHAModuleConfiguration& rhs)
{
   return !operator==(rhs);
}
////////////////////////////////////////////////////////////////////////////////
// Private methods define the various options:


/**
 * defineReadoutOptions
 *    Define options that control the readout.  These options default to false
 *    resulting in a minimal event if not otherwise configured.
 */
void
VX2750PHAModuleConfiguration::defineReadoutOptions()
{
    addBoolParameter("readrawtimes", false);
    addBoolParameter("readfinetimestamps", false);
    addBoolParameter("readtimedownsampling", false);
    addBoolListParameter("readanalogprobes", 2,2, false);
    addBoolListParameter("readdigitalprobes", 4,4, false);
    addBoolParameter("readsamplecount", false);
    addBoolParameter("readeventsize", false);
}
/**
 * defineGeneralOptions
 *    Define the general options.
 *
 */
void
VX2750PHAModuleConfiguration::defineGeneralOptions()
{
    const char* clockSources[] = {
       "Internal", "FPClkIn", "P0ClkIn", "Link", "DIPswitchSel",
       nullptr
    };
    addEnumParameter("clocksource", clockSources, "Internal");
    addBoolParameter("outputp0clock", false);
    addBoolParameter("outputfpclock", false);
    
    
}
/**
 *  defineAcqTriggerOptions
 *  
 * define the options in the CAEN Acquisition Trigger and veto section
 */
void
VX2750PHAModuleConfiguration::defineAcqTriggerOptions()
{
     const char* startSrces[]  = {
         "EncodedClkIn", "SINlevel", "SINedge", "SWcmd", "LVDS", "P0", nullptr
     };
     addEnumParameter("startsource", startSrces, "SINedge");
     
     const char* globalTriggerSrcs[] = {
         "TrgIn", "P0", "SwTrg", "LVDS", "ITLA", "ITLB",
         "ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedCLkIn", "GPIO", "TestPulse"
          nullptr
     };
     addEnumParameter("gbltriggersrc", globalTriggerSrces, "TrgIn");
     
     const char* triggerSources[] = {
          "ITLB", "ITLA", "GlobalTriggerSource", "TRGIN", "ExternalInhibit",
          "ADCUnderSaturation", "ADCOverSaturation", "SWTrigger", "ChSelfTrigger",
          "Ch64Trigger", "Disabled"
          nullptr
     };
     addEnumParameter("wavetriggersrc", triggerSources, "TRGIN");
     addEnumParameter("eventtriggersrc", triggerSources, "TRGIN");
     
     const char* resetsrcs[] = {
        "Start", "SIN", "GPIO", "EncodedClkIn"
         nullptr
     };
     addEnumParameter("tstampresetsrc", resetsrcs, "Start");
     addIntListParameter("channeltriggermasks", 0, 64);
     addBoolListParameter("savetraces", 0, 64, false);
     
     const char* trgoutmodes[] = {
        "TRGIN", "P0", "SwTrg", "LVDS", "ITLA", "ITLB",
        "ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedClkIn", "Run", "RefClk", "TestPulse",
        "Busy", "Fixed0", "Fixed1", "SyncIn", "SIN", "GPIO", "AcceptTrg", "TrgClk"
        nullptr
     }
     addEnumParameter("triggeroutmode", trgoutmodes, "TRGIN");
     
     const char* gpiomodes[] = {
       "Disabled", "TrgIn", "P0", "SIN", "LVDS", "ITLA", "ITLB",
       "ITLA_AND_ITLB", "ITLA_OR_ITLB", "EncodedClkIn", "SwTrg",
       "Run", "RefClk", "TestPulse", "Busy", "Fixed0", "Fixed1",
       nullptr
     };
     addEnumParameter("gpiomode", gpiomodes, "Disabled");
     
     
}

}                                   // caen_nscldaq namespace.