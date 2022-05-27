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
#include "VX2750PHa.h"
#include <string>
#include <stdlib.h>

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
    defineWfInspectionOptions();
    defineServiceOptions();
    defineITLOptions();
    defineLVDSOptions();
    defineDACOptions();
    defineInputConditioningOptions();
    defineEventSelectionOptions();
    defineFilterOptions();
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

/**
 * configureModule
 *    Configure the module in accordance with the current state of the configuration
 *    database.
 * @param module - module to configure.  The module object must already have
 *                 been connected to he physical hardware.
 */
void
VX2750PHAModuleConfiguration::configureModule(VX2750Pha& module)
{
  configureReadoutOptions(module);
  configureGeneralOptions(module);
  configureAcquisitionTriggerOptions(module);
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
    addBoolParameter("readflags", false);
    addBoolParameter("readtimedownsampling", false);
    addBoolListParameter("readanalogprobes", 2,2, false);
    addBoolListParameter("readdigitalprobes", 4,4, false);
    addBoolParameter("readsamplecount", false);
    addBoolParameter("readeventsize", false);
}
/**
 * configureReadoutOptions
 *    Configure the readout options in a module
 *  @param module - Te 
 */
void
VX2750PHAModuleConfiguration::configureReadoutOptions(VX2750Pha& module)
{
    // start from the default format:
    
    module.setDefaultFormat();
    module.enableRawTimestamp(getBoolParameter("readrawtimes"));
    module.enableFineTimestamp(getBoolParameter("readfinetimestamps"));
    module.enableFlags(getBoolParameter("readflags"));
    module.enableDownSampling(getBoolParameter("readtimedownsampling"));
    module.enableAnalogProbes(getBoolParameter("readanalogprobes"));
    module.enableDigitalProbes(getBoolParameter("readdigitalprobes"));
    module.enableSampleSize(getBoolParameter("readsamplecount"));
    module.enableRawEventSize(getBoolParameter("readeventsize"));
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
 * configureGeneralOptions
 *     Configure general options in a real module
 *   @param module -the module to configure.
 */
void
VX2750PHAModuleConfiguration::configureGeneralOptions(VX2750Pha& module)
{
  module.setClockSource(V2750Pha::StringToClockSource[cget("clocksource")]);
  module.setClockOutOnP0(getBoolParameter("outputp0clock"));
  module.setClokcOutOnFP(getBoolParameter("outputfpclock"));
                                                      
                                                      
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
     addEnumListParameter("wavetriggersrc", triggerSources, "TRGIN", 0,64,64);
     addEnumListParameter("eventtriggersrc", triggerSources, "TRGIN", 0,64,64);
     
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
     
     const char* busyinSources[] = {
        "SIN", "GPIO", "LVDS", "Disabled"
        nullptr
     };
     addEnumParameter("busyinsrc", busyinSources, "Disabled");
     
     const char* synoutmodes = {
        "Disabled", "SyncIn", "TestPulse", "Run",
        nullptr
     };
     addEnumParameter("syncoutmode", syncoutmodes, "Disabled");
     
     const char* boardvetosources[] = {
        "SIN", "LVDS", "GPIO", "P0", "EncodedClkIn", "Disabled",
        nullptr
     };
     addEnumParameter("boardvetosrc", boardvetosources, "Disabled");
     addIntParameter("boardvetowidth", 0, 34359738360, 200);
     const char* vetopolarities[] = {
      "ActiveHigh", "ActiveLow", nullptr
                    
     };
     addEnumParameter("boardvetopolarity", vetopolarities, "ActiveLow");
     
     
     const char* chanvetosources[ ] = {
        "BoardVeto", "ADCOverSaturation", "ADCUnderSaturation", "Disabled",
          nullptr
     };
     addEnumListParameter("chanveotsrc", chanvetosources, "Disabled", 0, 64, 64);
     addIntListParameter("chanvetowidth", 0, 524280, 0, 64, 64, 200);
     
     addIntParameter("rundelay", 0, 54280, 0);
     addBoolParameter("autodisarm", true);
     addBoolParameter("multiwindow", false);
     
     const char* pausetsvalues[] = {
        "hold", "run", nullptr
     };
     addBoolParameter("pausetimestamp", pausetsvalues, "run");
     
     addFloatParameter("volclkoutdelay", -18888.888, 18888.888, 0);
     addFloatParameter("permclkoutdelay", -18888.888, 18888.888, 0);
    
}
/**
 * configureAcquisitionTriggerOptions
 *    @param module -the module to configure.
 */
void
VX2750PHAModuleConfiguration::configureAcquisitionTriggerOptions(VX2750Pha& module)
{
  int nch = module.getChannelCount();
  
  module.setStartSource(VX2750Pha::stringToStartSource[cget("startsource")]);
  module.setGlobalTriggerSource(VX2750Pha::stringToGlobalTriggerSource[cget("gbltriggersrc")]);
  
  auto waveTriggers = getList("wavetriggersrc");
  auto evtTriggers  = getList("eventtriggersrc");
  auto triggerMasks = getUnsignedList("channeltriggermasks");   // since they're uint64_ts.
  auto saveTraces   = getBoolList("savetraces");
  auto chanVetoSrcs = getList("chnvetosrc");
  auto chanVetoWidths = getIntListParameter("chanvetowidth");
  
  for (int i =0; i < nch; i++) {
      module.setWaveTriggerSource(i, VX2750Pha::stringToWaveTrigger[waveTriggers[i]]);
      module.setEventTriggerSource(i, VX2750Pha::stringToEventTrigger[evtTriggers[i]]);
      module.setChannelTriggerMask(i, triggermasks[i]);
      module.setTraceRecordMode(i, saveTraces[i]);
      module.setChannelVetoSource(i, VX2750::StringToChannelVeto.get(cget("chanvetosrc")));
      module.setChannelVetoWidth(i, chanVetoWidths[i]);
  }
  module.setTimestampResetSource(VX2750Pha::stringToTimestampReset[cget("tstampresetsrc")]);
  module.setTRGOUTMode(VX2750Pha::stringToTRGOUT[cget("triggeroutmode")]);
  module.setGPIOMode(VX2750Pha::stringToGPIO[cget("gpiomode")]);
  module.setBusyInSource(VX2750Pha::stringToBusyIn.get(cget("busyinsrc")));
  module.setSyncOutMode(VX2750Pha::stringToSyncOut.get(cget("syncoutmode")));
  module.setBoardVetoSource(sVX2750Pha::tringToVeto.get(cget("boardvetosrc")));
  module.setBoardVetoWidth(getIntParam("boardvetowidth"));
  module.setBoardVetoPolarity(VX2750Pha::stringToVetoPolarity[cget("boardvetopolarity")]);
  module.setRunDelay(getIntParam("rundelay"));
  module.setAutoDisarmEnabled(getBoolParam("autodisarm"));
  module.setMultiWindowRunEnabled(getBoolParam("multiwindow"));
  bool hold;
  if (cget("pausetimestamp") == "hold") {
    hold = true;
  } else {
    hold = false;
  }
  module.setPauseTimestampHoldEnabled(hold);
  module.setVolatileClockDelay(getFloatParameter("volclkoutdelay"));
  module.setPermanentClockDelay(getFloatParameter("permclkoutdelay"));
                                                 
  
}
/**
 * defineWfInspectionOptions
 *   Define options that control how waveform inspection (including
 *   digital probes) operate.
 *   See main class comments for more inforation.
 */
void
VX2750PHAModuleConfiguration::defineWfInspectionOptions()
{
  const char* wavesources[] = {
      "ADC_DATA", "ADC_TEST_TOGGLE", "ADC_TEST_RAMP", "ADC_TEST_SIN", "IPE",
      "Ramp", "SquareWave", "ADC_TEST_PRBS",
      nullptr
  };
  addEnumListParameter("wavesource", wavesources, "ADC_DATA", 0, 64, 64);
  addIntListParameter("recordsamples", 4, 8100, 0 64, 64);
  
  const char* wfresolutions[] = {
      "Res8", "Res16", "Res32", "Res64",
      nullptr
  };
  addEnumListParameter("waveresolutions", wfresolutions, "Res8", 0, 64, 64);
  
  const char* analogProbes[] = {
    "ADCInput", "TimeFilter", "EnergyFilter", "EnergyFilterBaseline",
    "EnergyFilterMinusBaseline"
    nullptr
  };
  addEnumListParameter("analogprobe1", analogProbes, "ADCInput", 0, 64, 64);
  addenumListParameter("analogprobe2", analogProbes, "TimeFilter", 0, 64, 64);
  
  const char* digitalProbes[] = {
    "Trigger", "TimeFilterArmed", "RetriggerGuard", "EnergyFilterBaselineFreeze",
    "EnergyFilterPeaking", "EnergyFilterPileUpGuard", "EventPileUp", "ADCSaturation",
    "ADCSaturationProtection", "PostSaturationEvent", "EnergyFilterSaturation",
    "AcquisitionInhibit",
    nullptr
  };
  addEnumListParameter("digitalprobe1", digitalProbes, "Trigger", 0, 64, 64);
  addEnumListParameter("digitalprobe2", digitalProbes, "TimeFilterArmed", 0, 64, 64);
  addEnumListParameter("digitalprobe3", digitalProbes, "RetriggerGuard", 0, 64, 64);
  addEnumListParameter("digitalprobe4", digitalProbes, "EnergyBaselineFreeze", 0, 64, 64);
  
  addIntListParameter("pretriggersamples", 4, 4000,  0, 64, 64, 100);
  
}
/**
 * defineServiceOptions
 *    Define options that control the writable digitizer service parameters.
 */
void
VX2750PHAModuleConfiguration::defineServiceOptions()
{
  addIntParameter("testpulseperiod", 0, 34359738360, 100000);
  addIntParameter("testpulsewidth", 0, 34359738360, 1000);
  addIntParameter("testpulselowlevel", 0, 65535, 0);
  addIntParameter("testpulsehighlevel", 0, 65535, 65535);
  
  const char* iolevels[] = {
    "NIM", "TTL", nullptr
  };
  addEnumParameter("iolvel", iolevels, "NIM");
  addIntParameter("errorlflagmask", 0, 65535, 0);
  addIntParameter("errorflagdatamask", 0, 65535, 0);
}
/**
 * defineITLOptions
 *    Define the options that control the two individual trigger logic
 *    blocks.
 */
void
VX2750PHAModuleConfiguration::defineITLOptions()
{
  const char* mainlogicoptions[] = {
    "OR", "AND", "Majority", nullptr
  };
  addEnumParameter("itlalogic", mainlogicoptions, "OR");
  addEnumParameter("itlblogic", mainlogicoptions, "OR");
  addIntParameter("itlamajoritylevel", 1, 63, 1);
  addIntParameter("itlbmajoritylevel", 1, 63, 1);
  
  const char* pairlogicoptions[] =  {
    "AND", "OR", "None", nullptr
  };
  addEnumParameter("itlapairlogic", pairlogicoptions, "None");
  addEnumParameter("itlbpairlogic", pairlogicoptions, "None");
  
  const char* itlpolarity[] = {
    "Direct", "Inverted", nullptr
  };
  addEnumParameter("itlapolarity", itlpolarity, "Direct");
  addEnumParameter("itlbpolarity", itlpolarity, "Direct");
  
  const char* itlconnection[] = {
    "Disabled", "ITLA", "ITLB", nullptr
  };
  addEnumListParameter("itlconnect", itlconnection, "Disabled", 0, 64, 64);
  
  addIntParameter("itlagatewidth", 0, 524280, 100);
  addIntParameter("itlbgatewidth", 0, 524280, 100);
  
}
/**
 * defineLVDSOptions
 *     CReate settable options for the LVDS I/O pins.
 */
void
VX2750PHAModuleConfiguration::defineLVDSOptions()
{
  const char* lvdsmodes[] = {
    "SelfTriggers", "Sync", "IORegister", nullptr
  };
  addEnumList("lvdsmode", lvdsmodes, "IORegister", 4, 4, 4);
  const char* lvdsdirections[] ={
    "Input", "Output", nullptr
  };
  addEnumList("lvsdirection", lvdsdirections, "Output", 4, 4, 4);
  addIntListParameter("lvdstrgmask", 0, 0xffffffffffffffff, 16, 16, 16, 0);
}
/**
 * defineDACOptions
 *    Define the options for the digial to analog converter output.
 */
void
VX2750PHAModuleConfiguration::defineDACOptions()
{
  const char* dacoutmodes[]  = {
    "Static", "IPE", "ChInput", "MemOccupancy", "ChSum", "OverThrSum",
    "Ramp", "Sin5MHz", "Square",
    nullptr
  };
  addEnumParameter("dacoutmode", daqoutmodes, "ChSum");
  addIntParameter("dacoutputlevel", 0, 16383, 0);
  addIntParameter("dacoutchannel", 0, 63, 0);
}
/**
 * defineInputConditioningOptions
 *    See the header comments for these, and section 2.8 of the
 *    PHA parameter manual.
 */
void
VX2750PHAModuleConfiguration::defineInputConditioningOptions()
{
    addIntParameter("vgagain", 0, 40, 0);
    addBoolListParameter("offsetcalibrationenable", 0, 64, true, 64);
    addBoolListParameter("chanelenables", 0, 64, true, 64);
    addFloatListParameter("dcoffsets", 0.0, 100.0, 0, 64, 64, 50.0);
    addIntListParameter("triggerthresholds", 0, 8191, 0, 64, 64, 1023);
    
    const char* polarities[] = {"Positive", "Negative", nullptr};
    addEnumListParameter("inputpolarities", polarities, 0, 64, "Negative", 64);
}

/**
 *  defineEventSelectionOptions
 *      Define the options for event selection and coincidences.
 *      see the header comments for the actual set of
 *      parameters.
 */
void
VX2750PHAModuleConfiguration::defineEventSelectionOptions()
{
  addIntListParameter("energyskimlow", 0, 65534, 0, 64, 64, 0);
  addIntListParameter("energyskimhigh", 0, 65534, 0, 64, 64, 65534);
  
  const char* selectors[] = {
    "All", "Pileup", "EnergySkim", nullptr
  };
  addEnumListParameter("eventselector", selectors, 0, 64, "All", 64);
  addEnumListParameter("waveselector", selectors, 0, 64, "All", 64);
  
  const char* masks[] = {
    "Disabled", "Ch64Trigger", "TRGIN", "GlobalTriggerSource", "ITLA", "ITLB",
    nullptr
  };
  addEnumListParameter("coincidencemask", masks, 0, 64,"Disabled", 64);
  addEnumListParameter("anticoincidencemask", masks, 0, 64,"Disabled", 64);
  
  addIntListParameter("coincidencelength", 8,  524280, 100);
}
/**
 * defineFilterOptions
 *    Defines the time and energy filter configuration options.
 *    See header comments for parameter names.
 *    
 */
void
VX2750PHAModuleConfiguration::defineFilterOptions()
{
    addIntListParameter("tfrisetime", 80, 2000, 0, 64, 64, 80);
    addIntListParameter("tfretriggerguard", 0, 8000, 0, 64, 64, 0);
    addIntListParameter("efrisetime", 80, 13000, 0, 64, 64, 80);
    addIntListParameter("efpeakingpos", 0, 100, 0, 64, 64, 50);
    
    const char* peakingaverages[] = {
      "1", "4", "16", "64", nullptr
    };
    addEnumListParameter("efpeakingavg", peakingaverages, 0, 64, "1", 64);
    addIntListParameter("efpolezero", 80, 524000, 0, 64, 64, 80);
    addFloatListParameter("effinegain", 0, 0.0, 10.0, 64, 64, 1.0);
    addBoolListParameter("eflflimitation", 0, 64, false, 64);
    
    const har* baslineaverages[] = {
      "0", "16", "64", "64", "256", "1024", "4096", "16384", nuillptr
    };
    addEnumListParameter("efbaselineavg", baselineaverages, 0, 64, "0",64);
    addIntListParameter("efbaselinegaurdt", 0, 8000, 0, 64, 64, 0);
    addIntListParameter("efpileupguardt", 0, 80000, 0, 64, 64, 0);
    
}


}                                   // caen_nscldaq namespace.



