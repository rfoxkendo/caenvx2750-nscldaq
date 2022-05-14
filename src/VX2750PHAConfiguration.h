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
 *  ### General Parameters:
 *     -  clocksource - enumerated "Internal", "FPClkIn", "P0ClkIn", "Link", "DIPswitchSel"
 *     -  outputp0clock - bool  Output clock on backplane.
 *     -  outputfpclock - bool  Output clock on front panel.
 *
 *  ### Acquisition, Trigger and Veto Parameters:
 *     -  startsource - enum EncodedClkIn, SINLevel, SINedge, SWcmd, LVDS, P0
 *     -  gbltriggersrc - enum TrgIn, P0, SwTrg, LVDS, ITLA, ITLB,
 *                             ITLA_AND_ITLB, ITLA_OR_ITLB, EncodedClkIn,
 *                             GPIO, TestPulse
 *     - wavetriggersrc - enum "ITLB", "ITLA", "GlobalTriggerSource", "TRGIN",
 *                              "ExternalInhibit", "ADCUnderSaturation",
 *                              "ADCOverSaturation", :SWTrigger", "ChSelfTrigger",
 *                              "Ch64Trigger", "Disabled"
 *     - eventtriggersrc - enum - same as wavetriggersrc.
 *     - tstampresetsrc  - enum - "Start", "SIN", "GPIO", "EncodedClkIn"
 *     - channeltriggermasks- list of up to 64 uint64 masks of channel triggers.
 *                           that can trigger the associated channel.
 *     - savetraces       - list of up to 64 bool values true to save traces from
 *                          the corresponding channel.
 *     - triggeroutmode   -  enum TRGIN, P0, SwTrg, LVDS, ITLA, ATLB, ITLA_AND_ITLB,
 *                                ITLA_OR_ITLB, EncodedClkIn, Run, RefClk, TestPulse,
 *                                Busy, Fixed0, Fixed1, SyncIn, SIN, GPIO,
 *                                AcceptTrg, TrgClk
 *     - gpiomode   - Enum: Disabled, TrgIn, P0, SIN, LVDS, ITLA, ITLB,
 *                          ITLA_AND_ITLB, EncodedClkIn, SwTrg, Run, RefClk,
 *                          TestPulse, Busy, Fixed0, Fixed1
 *     - busyinsrc - enum - SIN, GPIO, LVDS, Disabled
 *     - syncoutmode - enum - Disabled, SyncIn, TestPulse, Run
 *     - boardvetosrc - enum SIN, LVDS, GPIO, P0, EncodedClkIn, Disabled
 *     - boardvetowidth - integer ns 0 - 34359738360 inclusive.
 *     - boardvetopolarity - enum ActiveHigh, ActiveLow
 *     - chanvetosrc  - list of up to 64 enums: BoardVeto, ADCOverSaturation,
 *                      ADCUnderSaturation, Disabled
 *     - chanvetowidth - List of up to 64 integers 0-524280
 *     - rundelay     - integer 0 - 54280
 *     - autodisarm   - bool
 *     - multiwindow - bool
 *     - pausestamp  - enum hold, run
 *     - volclkoutdelay - float -18888.888 - +18888.888
 *     - permclkoutdelay - float -18888.888 - +18888.888
 *     
 *  ### Waveform inspection parameters:
 *      -  wavesource - list of enums: "ADC_DATA", "ADC_TEST_TOGGLE",
 *                  "ADC_TEST_RAMP", "ADC_TEST_SIN", "IPE", "Ramp", "SquareWave",
 *                  "ADC_TEST_PRBS"
 *      -  recordsamples - list of integer record lengths in sampls: 4-8100
 *      -  waveresolutions - list of 16 waveform resolution enums:
 *                  "Res8", "Res16", "Res32", "Res64"
 *      -  analogprobe0 = list of enums "ADCInput", "TimeFilter", "EnergyFilter",
 *                          "EnergyFilterBaseline", "EnergyFilterMinusBaseline"
 *      -  analogprobe1 = list of enums as above but for analog probe 1.
 *      -  digitalprobe{1-4} list of enums describing the digital probes.
 *                  values: "Trigger", "TimeFilterArmed", "RetriggerGuard",
 *                  "EnerygFilterBaslineFreeze","EnergyFilterPeaking",
 *                  "EnergyFilterPileUpGuard", "EventPileup", "ADCSaturation",
 *                  "ADCSaturationProtection", "PostSaturationEvent",
 *                  "EnergyFilterSaturation", "AcqusitionInhibit"
 *      - pretriggersamples - samples prior to trigger list of integers 4-4000
 *
 *   ### Service Parameters
 *
 *      - testpulseperiod Integer 0 - 34359738360  Default 100000 (100usec).
 *      - testpulsewidth  Integer 0 - 34359738360 Default 50000 (50usec).
 *      - testpulselowlevel Integer 0 - 66535 - Default 0
 *      - testpulseHighLevel Integer 0- 65535 - Default 65535
 *      - iolevel  Enum NIM, TTL
 *      - errorflagmask - integer - 0 - 65535 - default 0
 *      - errorflagdatamask - integer - 65535 - default 0
 *      
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
private:
    void defineReadoutOptions();
    void defineGeneralOptions();
    void defineAcqTriggerOptions();
    void defineWfInspectionOptions()
    void defineServiceOptions();
};
    
}                                             // caen_nscldaq namespace



#endif