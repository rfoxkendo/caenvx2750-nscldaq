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
#include "VX2750Pha.h"
#include <string>
#include <stdlib.h>
#include <map>

// Local lookup tables:

static const std::map<unsigned, VX2750Pha::EnergyPeakingAverage> peakingAvgs =
{
  {1, VX2750Pha::Average1},
  {4, VX2750Pha::Average4},
  {16, VX2750Pha::EPeakAvg_Average16},
  {64, VX2750Pha::EPeakAvg_Average64}
};
static const std::map<unsigned, VX2750Pha::EnergyFilterBaselineAverage> blaverage=
{
   {0, VX2750Pha::Fixed},
   {16, VX2750Pha::EFilterBlineAvg_Average16},
   {64, VX2750Pha::EFilterBlineAvg_Average64},
   {256, VX2750Pha::Average256},
   {1024,VX2750Pha::Average1024},
   {4096, VX2750Pha::Average4K},
   {16384, VX2750Pha::Average16K}
};
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
  configureWfInspectionOptions(module);
  configureServiceOptions(module);
  configureITLOptions(module);
  configureLVDSOptions(module);
  configureInputConditioning(module);
  configureEventSelection(module);
  configureFilter(module);
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
  auto chanVetoWidths = getIntegerListParameter("chanvetowidth");
  
  for (int i =0; i < nch; i++) {
      module.setWaveTriggerSource(i, VX2750Pha::stringToWaveTrigger[waveTriggers[i]]);
      module.setEventTriggerSource(i, VX2750Pha::stringToEventTrigger[evtTriggers[i]]);
      module.setChannelTriggerMask(i, triggermasks[i]);
      module.setTraceRecordMode(i, saveTraces[i]);
      module.setChannelVetoSource(i, VX2750::StringToChannelVeto[cget("chanvetosrc")]);
      module.setChannelVetoWidth(i, chanVetoWidths[i]);
  }
  module.setTimestampResetSource(VX2750Pha::stringToTimestampReset[cget("tstampresetsrc")]);
  module.setTRGOUTMode(VX2750Pha::stringToTRGOUT[cget("triggeroutmode")]);
  module.setGPIOMode(VX2750Pha::stringToGPIO[cget("gpiomode")]);
  module.setBusyInSource(VX2750Pha::stringToBusyIn[cget("busyinsrc")]);
  module.setSyncOutMode(VX2750Pha::stringToSyncOut[cget("syncoutmode")]);
  module.setBoardVetoSource(sVX2750Pha::tringToVeto[(cget("boardvetosrc")]);
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
 * configureWfInspectionOptions
 *    Configure the wave form inspection parameters from the internal configuration
 * @param module - reference to the module to configure.
 * @note these are all per channel configurations.
 */
 void
 VX2750PHAModuleConfiguration::configureWfInspectionOptions(VX2750Pha& module)
 {
    int nch = module.channelCount();            // # of channels in the module.
    auto wfsources = getList("wfsource");
    auto samples   = getIntegerList("recordsamples");
    auto resolutions = getList("waveresolutions");
    std::vector<std::string> analogProbes[2];
    analogProbes[0] = getList("analogprobe1");
    analogProbes[1] = getList("analogprobe2");
    std::vector<std::string> digitalProbes[4];
    digitalProbes[0] = getList("digitlaprobe1");
    digitalProbes[1] = getList("digitlaprobe2");
    digitalProbes[2] = getList("digitlaprobe3");
    digitalProbes[3] = getList("digitlaprobe4");
    auto pretrigger = getIntegerList("pretriggersamples");
    
    // Now loop over the channels setting the parameters:
    
    for (int i =0; i < nch; i++) {
      module.setWaveDataSource(
          i, VX2750Pha::stringToWaveDataSource[(wfsources[i])]
      );
      module.setRecordSamples(i, samples[i]);
      module.setWaveResolutions(
          i, VX2750Pha::stringToWaveResolution[resolutins[i]]
      );
      for (int p = 0; p < 2; p++) {
        auto& probes = analogProbes[p];
        module.setAnalogProbe(
            i, p+1, V2750Pha::stringToAnalogProbe[probes[i]]
        );
      }
      for (int p = 0; p < 4; p++) {
        auto& probes = digitalProbes[p];
        module.setDigitalProbe(
          i, p+1, V2750Pha::stringToDigitalProbes[probes[i]]
        );
      }
      module.setPretriggerSamples(i, pretrigger[i]);
    }
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
  addEnumParameter("iolevel", iolevels, "NIM");
  addIntParameter("errorlflagmask", 0, 65535, 0);
  addIntParameter("errorflagdatamask", 0, 65535, 0);
}
/**
 * configureServiceOptions
 *    Configure the service options from the internal option database.
 * @param module - reference to the module being configured.
 */
void
VX2750PHAModuleConfiguration::configureServiceOptions(VX2750Pha& module)
{
    module.setTestPulsePeriod(getIntegerParameter("testpulseperiod"));
    module.setTestPulseWidth(getIntegerParameter("testpulsewidth"));
    module.setTestPulseLowLevel(getIntegerPrameter("testpulselowlevel"));
    module.setTestPulseHighLevel(getIntegerParameter("testpulsehighlevel"));
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
  
  addIntegerParameter("itlamask");
  addIntegerParameter("itlbmask");
  addIntParameter("itlagatewidth", 0, 524280, 100);
  addIntParameter("itlbgatewidth", 0, 524280, 100);
  
}
/**
 * configureITLOptions
 *    Configure the module internal trigger logic from the internal configuration
 *    database.
 * @param module - references the module to be configured.
 */
void
VX2750PHAModuleConfiguration::configureITLOptions(VX2750Pha& module)
{
     module.setITLAMainLogic(VX2750Pha::stringToIndividualTriggerLogic[cget("itlalogic")]);
     module.setITLBMainLogic(VX2750Pha::stringToIndividualTriggerLogic[cget("itlblogic")]);
     module.setITLAMajorityLevel(getIntegerParameter("itlamajoritylevel"));
     module.setITLBMajorityLevel(getIntegerParameter("itlbmajoritylevel"));
     module.setITLAPairLogic(VX2750Pha::stringToPariLogic[cget("itlapairlogic")]);
     module.setITLBPairLogic(VX2750Pha::stringToPariLogic[cget("itlbpairlogic")]);
     module.setITLAInverted(cget("itlapolarity") == "Inverted" ? true : false);
     module.setITLBInverted(cget("itlbpolarity") == "Inverted" ? true : false);
     
     int nch= module.channelCount();
     auto connections = getList("itlconnect");
     for (int i = 0; i < nch; i++) {
        module.setITLConnect(i, VX2750Pha::stringToITLConnect[cget("itlconnect")]);
  
     }
     module.setITLAMask(getUnsignedParameter("itlamask"));
     module.setITLBMask(getUnsingedParameter("itlbmask")):
     module.setITLAGateWidth(getUnsignedParameter("itlagatewidth"));
     mdoule.setITLBGateWidth(getUnsignedParameter("itlbgatewidth"));
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
 * configureLVDSOptions
 *    COnfigures the LVDS options in a module from our internalconfiguration.
 * @param module -references the module to configure.
 */
void
VX2750PHAModuleConfiguration::configureLVDSOptions(VX2750Pha& module)
{
  auto modes = getList("lvdsmode");
  auto direction = getList("lvdsdirection");
  auto masks   = getUnsignedList("lvdstrgmask");
  
  // First set the direction and mode of each quartet.
  
  for (int i = 0; i < 4; i++) {
    module.setLVDSMode(i, VX2750Pha::stringToLVDSMode[modes[i]]);
    module.setLVDSDirection(i, direction[i] == "Input" ? VX2750Pha::Input : VX2750Pha::Output);
  }
  
  // Now set the per pin trigger masks.
  
  for (int i =0; i < 16; i++) {
    module.setLVDSTirggerMask(i, masks[i]);
  }
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
 * configureDACOptions
 *    @param module references a module that is to be configured.
 */
void
VX2750PHAModuleConfiguration::configureDACOptions(VX2750Pha& module)
{
  module.setDACOutMode(VX2750Pha::stringToGDACOutMode[cget("dacoutmode")]);
  module.setDACOutValue(getIntegerParameter("dacoutputlevel"));
  module.setDACChannel(getIntegerParameter("dacoutchannel"));
}
/**
 * defineInputConditioningOptions
 *    See the header comments for these, and section 2.8 of the
 *    PHA parameter manual.
 */
void
VX2750PHAModuleConfiguration::defineInputConditioningOptions()
{
    addIntListParameter("vgagain", 0, 40, 0, 4, 4, 0);
    addBoolListParameter("offsetcalibrationenable", 0, 64, true, 64);
    addBoolListParameter("chanelenables", 0, 64, true, 64);
    addFloatListParameter("dcoffsets", 0.0, 100.0, 0, 64, 64, 50.0);
    addIntListParameter("triggerthresholds", 0, 8191, 0, 64, 64, 1023);
    
    const char* polarities[] = {"Positive", "Negative", nullptr};
    addEnumListParameter("inputpolarities", polarities, 0, 64, "Negative", 64);
}
/**
 * conifigureInputConditioning
 *    Configure how the analog inputs are conditioned prior to being
 *    presented to the FADC.
 * @param module - reference to module being configured.
 */
void
VX2750PHAModuleConfiguration::configureInputConditioning(VX2750Pha& module)
{
  
  
  // Sinc eonly 2745 digitizers have a variable gain amp:
  
  if (module.getFamilyCode() == 2745) {
    auto vgaGains = getIntegerList("vgagain");
    for (int i =0; i < 4; i++) {
         module.setVGAGain(i, vgaGains[i]); 
    }
  }
  // The remainder are all per channel parameters:
  
  auto enableOffsetCalibration = getBoolList("offsetcalibrationenable");
  auto channelEnables  = getBoolList("channelenables");
  auto dcOffsets  = getFloatList("dcoffsets");
  auto thresholds = getIntegerList("triggerthresholds");
  auto inputPolarities = getList("inputpolarities");
  
  int nch = module.channelCount();
  for (int i =0; i < nch; i++) {
    module.enableOffsetCalibration(i, enableOffsetCalibration[i]);
    module.enableChannel(i, channelEnables[i]);
    module.setDCOffset(i, dcOffsets[i]);
    module.setTriggerThreshold(i, thresholds[i]);
    module.setPulsePolarity(
        i,
        (inputPolarities == "Positive") ?
          VX2750Pha::Positive : VX2750Pha::Negative
    );
  }
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
  
  addIntListParameter("coincidencelength", 8,  524280, 0, 64, 64, 100);
}
/**
 * configureEventSelection
 *   Apply the event selection configuration parameters to a module.
 * @param module -the module that will be configured.
 */
void
VX2750PHAModuleConfiguration::configureEventSelection(VX2750Pha& module)
{
  auto lowskims = getIntegerList("energyskimlow");
  auto hiskims  = getIntegerList("energyskimhigh");
  auto eventselectors = getList("eventselector");
  auto waveselectors = getList("waveSelector");
  auto coincMasks = getList("coincidencemask");
  auto anticoincMask = getList("anticoincidencemasks");
  auto coincidencewindow = getIntegerList("coincidencelength");
  
  int nch = module.channelCount();
  for(int i =0; i < nch; i++) {
    module.setEnergySkimLowDiscriminator(i, lowskims[i]);
    module.setEnergySkimHighDiscriminator(i, hiskims[i]);
    module.setEventSelector(i, VX2750Pha::stringToEventSelector[eventselectors[i]]);
    module.setWaveformSelector(i, VX2750Pha::stringToEventSelector[eventselectors[i]]);
    module.setCoincidenceNs(chan, coincidencewindow[i]);
    module.setCoincidenceMask(chan, VX2750Pha::stringToCoincidenceMask[coincMask[i]]);
    module.setAntiCoincidenceMask(chan, VX2750Pha::stringToCoincidenceMask[anticoincMask[i]]);
  }
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
    addIntListParameter("efflattoptime", 80, 3000, 0,64, 64, 80);
    addIntListParameter("efpeakingpos", 0, 100, 0, 64, 64, 50);
    
    const char* peakingaverages[] = {
      "1", "4", "16", "64", nullptr
    };
    addEnumListParameter("efpeakingavg", peakingaverages, 0, 64, "1", 64);
    addIntListParameter("efpolezero", 80, 524000, 0, 64, 64, 80);
    addFloatListParameter("effinegain", 0, 0.0, 10.0, 64, 64, 1.0);
    addBoolListParameter("eflflimitation", 0, 64, false, 64);
    
    const har* baslineaverages[] = {
      "0", "16", "64", "256", "1024", "4096", "16384", nullptr
    };
    addEnumListParameter("efbaselineavg", baselineaverages, 0, 64, "0",64);
    addIntListParameter("efbaselineguardt", 0, 8000, 0, 64, 64, 0);
    addIntListParameter("efpileupguardt", 0, 80000, 0, 64, 64, 0);
    
}
/**
 * configureFilter
 *    Set the module filter options in accordance with our option database.
 * @parm module - module to configure.
 */
void
VX2750PHAModuleConfiguration::configureFilter(VX2750Pha& module)
{
    auto triggerRiseTimes = getIntegerList("trfisetime");
    auto triggerRetriggerGuards = getIntegerList("tfretriggerguard");
    auto energyRiseTimes = getIntegerList("efrisetime");
    auto energyFlatTopTimes = getIntegerList("efflattoptime");
    auto energyPeakingPos = getIntegerList("efpieakingpos");
    auto peakingAverages  = getIntegerList("efpeakingavg");  // The enums all translate as integers.
    auto poleZeros       = getIntegerList("efpolezero");
    auto fineGains      =getFloatList("efpolezero");
    auto lfEliminations = getBoolList("eflflimitation");
    auto blAveraging    = getIntegerList("efbaselineavg"); // again they're all integers
    auto blGuardTimes   = getIntegerList("efbaselineguardt");
    auto pupGuardTimes  = getIntegerList("efpileupguardt");
    
    int nch = module.channelCount();
    for (int i = 0; i < nch; i++) {
      module.setTimeFilterRiseTime(i, triggerRiseTimes[i]);
      module.setTimeFilterRetriggerGuardTime(i, triggerRetriggerGuards[i]);
      module.setEnergyFilterRiseTime(i, energyRiseTimes[i]);
      modulee.setEnergyFilterFlattopTime(i, energyFlatTopTimes[i]);
      module.setEnergyFilterFlatTopTime(i, energyPeakingPos[i]);
      module.setEnergyFilterPeakingAverage(i, peakingAvgs[peakingAverages[i]]);
      module.setEnergyFilterPoleZeroTime(i, poleZeros[i]);
      module.setEnergyFilterFineGain(i, fineGains[i]);
      module.setEnergyFilterFLimitationEnabled(i, lfEliminations[i]);
      module.setEnergyFilterBaselineAverage(i, blaverage[blAveraging[i]]);
      module.setEnergyFilterBaselineGuardTime(i, blGuardTimes[i]);
      module.setEnergyFilterPileupGuardTime(i, pupGuardTimes[i]);
    }
}

}                                   // caen_nscldaq namespace.



