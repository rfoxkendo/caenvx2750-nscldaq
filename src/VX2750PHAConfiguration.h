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
* @file     VX2750PHAConfiguration.h
* @brief    Header for the configuration associated with a 
* @author   Ron Fox
*
*/

#ifndef CAENVX2750_PHAMODULECONFIGURATION_H
#define CAENVX2750_PHAMODULECONFIGURATION_H


#include "XXUSBConfigurableObject.h"      // nice base class from NSCLDAQ.


namespace caen_nscldaq {

/**
 * @class V2750PHAModuleConfiguration
 *   Provides a configuration for the readout of a VX2750 (e.g.) module
 *   with PHA firmware.
 *   constructing this class (unlike how the XXUSB readouts work) constructs
 *   the configuration of the module with a default configuration.
 *  Configurable parameters; these divide into sections:
 *
 *  ### Controlling readout:
 *     -  readrawtimestamps - bool enable readout of raw timestamps
 *     -  readfinetimestamps - bool enable readout of the CFD fine timestamp.
 *     -  readflags          - bool enable readout of the high and low priority flags.
 *     -  readtimedownsampling - bool read the time downsampling factor.
 *     -  readanalogprobes    - list of two bools:  {probe1enable probe2enable}
 *     -  readdigitalprobes   - list of four bools {probe1 probe2 probe3 probe4}
 *     -  readsamplecount     - bool -enable read of number of samples in fragment.
 *     -  readeventsize        - bool enable read of event size.
 *     
 */
class VX2750PHAModuleConfiguration : public ::XXUSB::XXUSBConfigurableObject
{
public:
    VX2750PHAModuleConfiguration(const char* name);
    VX2750PHAModuleConfiguration(const VX2750PHAModuleConfiguration& rhs);
    virtual ~VX2750PHAModuleConfiguration();
    
    VX2750PHAModuleConfiguration& operator=(const VX2750PHAModuleConfiguration& rhs);
    int operator==(const VX2750PHAModuleConfiguration& rhs);
    int operator!=(const VX2750PHAModuleConfiguration& rhs);
    
    // Everything else public is done by the base class.
    
};
    
}                                             // caen_nscldaq namespace



#endif