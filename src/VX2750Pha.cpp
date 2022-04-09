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
* @file     VX2750Pha.cpp
* @brief    Implements the PHA class layered on Dig2Device.cpp
* @author   Ron Fox
*
*/
#include "VX2750Pha.h"
#include <stdexcept>
#include <sstream>


namespace caen_nscldaq {

// Enumerator mappings.  Readonly have string->enum maps.  R/W have
// both a string to enum and enum to string map.  

static const std::map<std::string, VX2750Pha::FwType> stringToFwType = {
    { "DPP_PHA", VX2750Pha::DPP_PHA},
    { "DPP_ZLE", VX2750Pha::DPP_ZLE},
    { "DPP_PSD", VX2750Pha::DPP_PSD},
    { "DPP_DAQ", VX2750Pha::DPP_DAQ},
    { "DPP_OPEN", VX2750Pha::DPP_OPEN},
    {"Scope", VX2750Pha::Scope}
}

static const std::map<std::string, VX2750Pha::FormFactors> stringToFormFactor = {
    {"0", VX2750Pha::VME},
    {"1", VX2750Pha::VME64X},
    {"2", VX2750Pha::DT}
}

static const std::map<std::string, VX2750Pha::ClockSource> stringToClockSource = {
    {"Internal", VX2750Pha::Internal},
    {"FPClkIn", VX2750Pha::FrontPanel},
    {"P0ClkIn",   VX2750Pha::P0},
    {"Link",   VX2750Pha::Link},
    {"DIPswitchSel", VX2750Pha::DIPSelected}
};
static const std::map<VX2750Pha::ClockSource, std::string> clockSourceToString = {
    {VX2750Pha::Internal, "Internal"},
    {VX2750Pha::FrontPanel, "FPClkIn"},
    {VX2750Pha::P0, "P0ClkIn"},
    {VX2750Pha::Link, "Link"},
    {VX2750Pha::DIPSelected, "DIPswitchSel"}
};

static const std::map<std::string, VX2750Pha::StartSource> stringToStartSource = {
    {"EncodedClkIn", VX2750Pha::EncodedClockIn},
    {"SINlevel", VX2750Pha::SINLevel},
    {"SINedge", VX2750Pha::SINEdge},
    {"SWcmd", VX2750Pha::SWCommand},
    {"LDVDS", VX2750Pha::LVDS},
    {"P0",   VX2750Pha::P0}
};

static const std::map<VX2750Pha::StartSource, std::string> startSourceToString = {
    {VX2750Pha::EncodedClockIn, "EncodedClkIn"},
    {VX2750Pha::SINLevelm, "SINlevel"},
    {VX2750Pha::SINEdge "SINedge",
    {VX2750Pha::SWCommand, "SWcmd"},
    {VX2750Pha::LVDS, "LVDS"},
    {VX2750Pha::P0, "P0"}
};

static const std::map<std::string, VX2750Pha::GlobalTriggerSource> stringToGlobalTriggerSource = {
    {"TrgIn", VX2750Pha::TriggerIn},
    {"P0", VX2750Pha::P0},
    {"SwTrg", VX2750Pha::Software},
    {"LVDS", VX2750Pha::LVDS},
    {"ITLA", VX2750Pha::InternalA},
    {"ITLB", VX2750Pha::InternalB},
    {"ITLA_AND_ITLB", VX2750Pha::InternalAandB},
    {"ITLA_OR_ITLB", VX2750Pha::InternalAorB},
    {"EncodedClkIn", VX2750Pha::EncodedClockIn},
    {"GPIO", VX2750Pha::GPIO},
    {"TestPulse", VX2750Pha::TestPulse}
};
static const std::map<VX2750Pha::GlobalTriggerSource, std::string> globalTriggerSourceToString = {
    {VX2750Pha::TriggerIn, "TrgIn"},
    {VX2750Pha::P0, "P0"},
    {VX2750Pha::Software, "SwTrg"},
    {VX2750Pha::LVDS, "LVDS"},
    {VX2750Pha::InternalA, "ITLA"},
    {VX2750Pha::InternalB, "ITLB"},
    {VX2750Pha::InternalAandB, "ITLA_AND_ITLB"},
    {VX2750Pha::InternalAorB, "ITLA_OR_ITLB"},
    {VX2750Pha::EncodedClockIn, "EncodedClkIn"},
    {VX2750Pha::GPIO, "GPIO"},
    {VX2750Pha::TestPulse, "TestPulse"}    
};

static const std::map<std::string, VX2750Pha::WaveTriggerSource> stringToWaveTrigger= {
     {"ITLB", VX2750Pha::InternalA},
     {"ITLA", VX2750Pha::InternalB},
     {"GlobalTriggerSource", VX2750Pha::GlobalTriggerSource},
     {"TRGIN", VX2750Pha::TRGIN},
     {"ExternalInhibit", VX2750Pha::ExternalInhibit},
     {"ADCUnderSaturation", VX2750Pha::ADCUnderSaturation},
     {"ADCOverSaturation", VX2750Pha::ADCOverSaturation},
     {"SWTrigger", VX2750Pha::Software},
     {"ChSelfTrigger", VX2750Pha::ChannelSelfTrigger},
     {"Ch64Trigger", VX2750Pha::AnyChannelSelfTrigger},
     {"Disabled", VX2750Pha::Disabled}
};
static const std::map<VX2750Pha::WaveTriggerSource, std::string> waveTriggerToString= {
     {VX2750Pha::InternalA, "ITLB"},
     {VX2750Pha::InternalB, "ITLA"},
     {VX2750Pha::GlobalTriggerSource, "GlobalTriggerSource"},
     {VX2750Pha::TRGIN, "TRGIN"},
     {VX2750Pha::ExternalInhibit, "ExternalInhibit"},
     {VX2750Pha::ADCUnderSaturation, "ADCUnderSaturation"},
     {VX2750Pha::ADCOverSaturation, "ADCOverSaturation"},
     {VX2750Pha::Software, "SWTrigger"},
     {VX2750Pha::ChannelSelfTrigger, "ChSelfTrigger"},
     {VX2750Pha::AnyChannelSelfTrigger, "Ch64Trigger"},
     {VX2750Pha::Disabled, "Disabled"}
};

static const std::map<std::string, VX2750Pha::EventTriggerSource> stringToEventTrigger = {
    {"ITLB", VX2750Pha::InternalB},
    {"ITLA", VX2750Pha::InternalA},
    {"GlobalTriggerSource", VX2750Pha::GlobalTriggerSource},
    {"TRGIN", VX2750Pha::TRGIN},
    {"SWTrigger", VX2750Pha::Software},
    {"ChSelfTrigger", VX2750Pha::ChannelSelfTrigger},
    {"Ch64Trigger", VX2750Pha::AnyChannelSelfTrigger},
    {"Disabled", VX2750Pha::Disabled}
};
static const std::map<VX2750Pha::EventTriggerSource, std::string> eventTriggerToString = {
    {VX2750Pha::InternalB, "ITLB"},
    {VX2750Pha::InternalA, "ITLA"},
    {VX2750Pha::GlobalTriggerSource, "GlobalTriggerSource"},
    {VX2750Pha::TRGIN, "TRGIN"},
    {VX2750Pha::Software, "SWTrigger"},
    {VX2750Pha::ChannelSelfTrigger, "ChSelfTrigger"},
    {VX2750Pha::AnyChannelSelfTrigger, "Ch64Trigger"},
    {VX2750Pha::Disabled, "Disabled"}    
};

static const std::map<std::string, VX2750Pha::TimestampResetSource> stringToTimestampReset = {
    {"Start", VX2750Pha::Start},
    {"SIN", VX2750Pha::SIN},
    {"GPIO", VX2750Pha::GPIO},
    {"EncodedClkIn", VX2750Pha::EncodedClockIn}
};
static const std::map<VX2750Pha::TimestampResetSource, std::string> TimestampResetToString = {
    {VX2750Pha::Start, "Start"},
    {VX2750Pha::SIN, "SIN"},
    {VX2750Pha::GPIO, "GPIO"},
    {VX2750Pha::EncodedClockIn, "EncodedClkIn"}
};

static const std::map<std::string, V2750Pha::TraceRecordMode> stringToTraceRecord = {
    {"Always", VX2750Pha::Always},
    {"On Request", VX2750Pha::OnRequest}
};
static const std::map<V2750Pha::TraceRecordMode, std::string> traceRecordToString = {
    {VX2750Pha::Always, "Always"},
    {VX2750Pha::OnRequest, "On Request"}
};

static const std::map<std::string, VX2750Pha::TRGOUTMode> stringToTRGOUT = {
    {"TRGIN", VX2750Pha::TRGIN},
    {"P0", VX2750Pha::P0},
    {"SwTrg", VX2750Pha::Software},
    {"LVDS", VX2750Pha::LVDS},
    {"ITLA", VX2750Pha::InternalA},
    {"ITLB", VX2750Pha::InternalB},
    {"ITLA_AND_ITLB", VX2750Pha::InternalAandInternalB},
    {"ITLA_OR_ITLB", VX2750Pha::InternalAorInternalB},
    {"EncodedClkIn", VX2750Pha::EncodedClockIn},
    {"Run", VX2750Pha::Run},
    {"RefClk", VX2750Pha::ReferenceClock},
    {"TestPulse", VX2750Pha::TestPuse},
    {"Busy", VX2750Pha::Busy},
    {"Fixed0", VX2750Pha::Zero},
    {"Fixed1", VX2750Pha::One},
    {"SyncIn", VX2750Pha::SynchIn},
    {"SIN", VX2750Pha::SIN},
    {"GPIO", VX2750Pha::GPIO},
    {"AcceptTrg", VX2750Pha::AcceptedTrigger},
    {"TrgClk", VX2750Pha::TriggerClock}
};

static const std::map<VX2750Pha::TRGOUTMode, std::string> TRGOUTToString = {
    {VX2750Pha::TRGIN, "TRGIN"},
    {VX2750Pha::P0, "P0"},
    {VX2750Pha::Software, "SwTrg"},
    {VX2750Pha::LVDS, "LVDS"},
    {VX2750Pha::InternalA, "ITLA"},
    {VX2750Pha::InternalB, "ITLB"},
    {VX2750Pha::InternalAandInternalB, "ITLA_AND_ITLB"},
    {VX2750Pha::InternalAorInternalB, "ITLA_OR_ITLB"},
    {VX2750Pha::EncodedClockIn, "EncodedClkIn"},
    {VX2750Pha::Run, "Run"},
    {VX2750Pha::ReferenceClock, "RefClk"},
    {VX2750Pha::TestPuse, "TestPulse"},
    {VX2750Pha::Busy, "Busy"},
    {VX2750Pha::Zero, "Fixed0"},
    {VX2750Pha::One, "Fixed1"},
    {VX2750Pha::SynchIn, "SyncIn"},
    {VX2750Pha::SIN, "SIN"},
    {VX2750Pha::GPIO, "GPIO"},
    {VX2750Pha::AcceptedTrigger, "AcceptTrg"},
    {VX2750Pha::TriggerClock, "TrgClk"}
};

static const std::map<std::string, VX2750Pha::GPIOMode>  stringToGPIO = {
    {"Disabled", VX2750Pha::Disabled},
    {"TrgIn", VX2750Pha::TriggerIn},
    {"P0", VX2750Pha::P0},
    {"SIN", VX2750Pha::SIN},
    {"LVDS", VX2750Pha::LVDS},
    {"ITLA", VX2750Pha::InternalA},
    {"ITLB", VX2750Pha::InternalB},
    {"ITLA_AND_ITLB", VX2750Pha::InternalAandInternalB},
    {"ITLA_OR_ITLB", VX2750Pha::InternalAorInternalB},
    {"EncodedClkIn", VX2750Pha::EncodedClockIn},
    {"SwTrg", VX2750Pha::SoftwareTrigger},
    {"Run", VX2750Pha::Run},
    {"RefClk", VX2750Pha::ReferenceClock},
    {"TestPulse", VX2750Pha::TestPulse},
    {"Busy", VX2750Pha::Busy},
    {"Fixed0", VX2750Pha::Zero},
    {"Fixed1", VX2750Pha::One}
};
static const std::map<VX2750Pha::GPIOMode,std::string> GPIOToString = {
    {VX2750Pha::Disabled, "Disabled"},
    {VX2750Pha::TriggerIn, "TrgIn"},
    {VX2750Pha::P0, "P0"},
    {VX2750Pha::SIN, "SIN"},
    {VX2750Pha::LVDS, "LVDS"},
    {VX2750Pha::InternalA, "ITLA"},
    {VX2750Pha::InternalB, "ITLB"},
    {VX2750Pha::InternalAandInternalB, "ITLA_AND_ITLB"},
    {VX2750Pha::InternalAorInternalB, "ITLA_OR_ITLB"},
    {VX2750Pha::EncodedClockIn, "EncodedClkIn"},
    {VX2750Pha::SoftwareTrigger, "SwTrg"},
    {VX2750Pha::Run, "Run"},
    {VX2750Pha::ReferenceClock, "RefClk"},
    {VX2750Pha::TestPulse, "TestPulse"},
    {VX2750Pha::Busy, "Busy"},
    {VX2750Pha::Zero, "Fixed0"},
    {VX2750Pha::One, "Fixed1"}
};

static const std::map<std::string, VX2750Pha::BusyInSource> stringToBusyIn = {
    {"SIN", VX2750Pha::SIN},
    {"GPIO", VX2750Pha::GPIO},
    {"LVDS", VX2750Pha::LVDS},
    {"Disabled", VX2750Pha::Disabled }
};
static const std::map<VX2750Pha::BusyInSource, std::string> BusyInTostring = {
    {VX2750Pha::SIN, "SIN"},
    {VX2750Pha::GPIO, "GPIO"},
    {VX2750Pha::LVDS, "LVDS"},
    {VX2750Pha::Disabled, "Disabled"}
};

static const std::map<std::string, VX2750PHA::SyncOutMode> stringToSyncOut = {
    {"Disabled", VX2750Pha::Disabled},
    {"SyncIn", VX2750Pha::SynchIn},
    {"TestPulse", VX2750Pha::TestPulse},
    {"IntClk", VX2750Pha::InternalClock},
    {"Run", VX2750Pha::Run}
};
static const std::map<VX2750Pha::SyncOutMode, std::string> syncOutToString = {
    {VX2750Pha::Disabled, "Disabled"},
    {VX2750Pha::SynchIn, "SyncIn"},
    {VX2750Pha::TestPulse, "TestPulse"},
    {VX2750Pha::InternalClock, "IntClk"},
    {VX2750Pha::Run, "Run"}
};

static const std::map<std::string , VX2750Pha::VetoSource> = stringToVeto = {
    {"SIN", VX2750Pha::SIN},
    {"LVDS", VX2750Pha::LVDS},
    {"GPIO", VX2750Pha::GPIO},
    {"P0", VX2750Pha::P0},
    {"EncodedClkIn", VX2750Pha::EncodedClock},
    {"Disabled", VX2750Pha::Disabled}
}
static const std::map<VX2750Pha::VetoSource, std::string> = vetoToString = {
    {VX2750Pha::SIN, "SIN"},
    {VX2750Pha::LVDS, "LVDS"},
    {VX2750Pha::GPIO, "GPIO"},
    {VX2750Pha::P0, "P0"},
    {VX2750Pha::EncodedClock, "EncodedClkIn"},
    {VX2750Pha::Disabled, "Disabled"}
};

static const std::map<std::string, VX2750Pha::VetoPolarity> stringToVetoPolarity = {
    {"ActiveHigh", VX2750PHa::ActiveHigh},
    {"ActiveLow", VX2750PHa::ActiveLow}
};
static const std::map<VX2750Pha::VetoPolarity, std::string> vetoPolarityToString = {
    {VX2750PHa::ActiveHigh, "ActiveHigh"},
    {VX2750PHa::ActiveLow, "ActiveLow"}
};

static const std::map<std::string, VX2750Pha::ChannelVetoSource> stringToChannelVeto = {
    {"BoardVeto", VX2750Pha::BoardVeto},
    {"ADCOverSaturation", VX2750Pha::OverSaturation},
    {"ADCUnderSaturation", VX2750Pha::UnderSaturation},
    {"Disabled", VX2750Pha::Disabled}
};
static const std::map<VX2750Pha::ChannelVetoSource, std::string> channelVetoToString = {
    {VX2750Pha::BoardVeto, "BoardVeto"},
    {VX2750Pha::OverSaturation, "ADCOverSaturation"},
    {VX2750Pha::UnderSaturation, "ADCUnderSaturation"},
    {VX2750Pha::Disabled, "Disabled"}
};

static const std::map<std::string, VX2750Pha::WaveDataSource> stringToWaveDataSource = {
    {"ADC_DATA", VX2750Pha::ADC_DATA},
    {"ADC_TEST_TOGGLE", VX2750Pha::ADC_TEST_TOGGLE},
    {"ADC_TEST_RAMP", VX2750Pha::ADC_TEST_RAMP},
    {"ADC_TEST_SIN", VX2750Pha::ADC_SIN},
    {"IPE", VX2750Pha::IPE},
    {"Ramp", VX2750Pha::Ramp},
    {"SquareWave", VX2750Pha::SquareWave},
    {"ADC_TEST_PRBS", VX2750Pha::ADC_TEST_PRBS}
};
static const std::map<VX2750Pha::WaveDataSource, std::string> waveDataSourceToString = {
    {VX2750Pha::ADC_DATA, "ADC_DATA"},
    {VX2750Pha::ADC_TEST_TOGGLE, "ADC_TEST_TOGGLE"},
    {VX2750Pha::ADC_TEST_RAMP, "ADC_TEST_RAMP"},
    {VX2750Pha::ADC_SIN, "ADC_TEST_SIN"},
    {VX2750Pha::IPE, "IPE"},
    {VX2750Pha::Ramp, "Ramp"},
    {VX2750Pha::SquareWave, "SquareWave"},
    {VX2750Pha::ADC_TEST_PRBS, "ADC_TEST_PRBS"}
};

static const std::map<std::string, VX2750Pha::WaveResolution> stringToWaveResolution = {
    {"Res8", VX2750Pha::Res8},
    {"Res16", VX2750Pha::Res16},
    {"Res32", VX2750Pha::Res32},
    {"Res64", VX2750Pha::Res64} 
};
static const std::map<VX2750Pha::WaveResolution, std::string> waveResolutionToSTring = {
    {VX2750Pha::Res8, "Res8",},
    {VX2750Pha::Res16, "Res16"},
    {VX2750Pha::Res32, "Res32"},
    {VX2750Pha::Res64, "Res64"} 
};

static const std::map<std::string, VX2750Pha::AnalogProbe> stringToAnalogProbe = {
    {"ADCInput", VX2750Pha::ADCInput},
    {"TimeFilter", VX2750Pha::TimeFilter},
    { "EnergyFilter", VX2750Pha::EnergyFilter},
    {"EnergyFilterBaseline", VX2750Pha::EnergyFilterBaseline},
    {"EnergyFilterMinusBaseline", VX2750Pha::EnergyFilterMinusBaseline}
};
static const std::map<VX2750Pha::AnalogProbe, std::string> analogProbeToString = {
    {VX2750Pha::ADCInput, "ADCInput"},
    {VX2750Pha::TimeFilter,"TimeFilter"},
    {VX2750Pha::EnergyFilter, "EnergyFilter"},
    {VX2750Pha::EnergyFilterBaseline, "EnergyFilterBaseline"},
    {VX2750Pha::EnergyFilterMinusBaseline, "EnergyFilterMinusBaseline"} 
};

static const std::map<std::string, VX2750Pha::DigitalProbe> stringToDigitalProbe = {
    {"Trigger", VX2750Pha::Trigger},
    {"TimeFilterArmed", VX2750Pha::TimeFilterArmed},
    {"ReTriggerGuard", VX2750Pha::ReTriggerGuard},
    {"EnergyFilterBaslineFreeze", VX2750Pha::EneryFilterBaselineFreeze},
    {"EnergyFilterPeaking", VX2750Pha::EnergyFilterPeaking},
    {"EnergyFilterPeakReady", VX2750Pha::EnergyFilterPeakReady},
    {"EnergyFilterPileUpGuard", VX2750Pha::EnergyFilterPileupGuard},
    {"EventPileup", VX2750Pha::EventEventPileup},
    {"ADCSaturation", VX2750Pha::ADCSaturation},
    {"ADCSaturationProtection", VX2750Pha::ADCSaturationProtection},
    {"PostSaturationEvent", VX2750Pha::PostSaturationEvent},
    {"EnergyFilterSaturation" VX2750Pha::EnergyFilterSaturation},
    {"AcquisitionInhibit", VX2750Pha::AcquisitionInhibit}
};
static const std::map<VX2750Pha::DigitalProbe, std::string> digitalProbeToString = {
    {VX2750Pha::Trigger, "Trigger"},
    {VX2750Pha::TimeFilterArmed, "TimeFilterArmed"},
    {VX2750Pha::ReTriggerGuard, "ReTriggerGuard"},
    {VX2750Pha::EneryFilterBaselineFreeze, "EnergyFilterBaslineFreeze"},
    {"VX2750Pha::EnergyFilterPeaking, EnergyFilterPeaking"},
    {"EnergyFilterPeakReady", VX2750Pha::EnergyFilterPeakReady},
    {"EnergyFilterPileUpGuard", VX2750Pha::EnergyFilterPileupGuard},
    {"EventPileup", VX2750Pha::EventEventPileup},
    {"ADCSaturation", VX2750Pha::ADCSaturation},
    {"ADCSaturationProtection", VX2750Pha::ADCSaturationProtection},
    {"PostSaturationEvent", VX2750Pha::PostSaturationEvent},
    {"EnergyFilterSaturation" VX2750Pha::EnergyFilterSaturation},
    {"AcquisitionInhibit", VX2750Pha::AcquisitionInhibit}
};

static const std::map<std::string, VX2750Pha::IOLevel> stringToIOLevel = {
    {"TTL", VX2750Pha::TTL},
    {"NIM", VX2750Pha::NIM}
};
static const std::map<VX2750Pha::IOLevel, std::string> ioLevelToString = {
    {VX2750Pha::NIM, "NIM"},
    {VX2750Pha::TTL, "TTL"}
};

static const std::map<std::string, VX2750Pha::IndividualTriggerLogic> stringToIndividualTriggerLogic = {
    {"OR", VX2750Pha::OR},
    {"AND", VX2750Pha::AND},
    {"Majority", VX2750Pha::Majority}
};
static const std::map<VX2750Pha::IndividualTriggerLogic> individualTriggerLogicToString = {
    {VX2750Pha::OR, "OR"},
    {VX2750Pha::AND, "AND"},
    {VX2750Pha::Majority, "Majority"}
};

static const std::map<std::string, VX2750Pha::PairTriggerLogic> stringToPairLogic = {
    {"AND", VX2750Pha::AND},
    {"OR", VX2750Pha::OR},
    {"NONE", VX2750Pha::NONE}
};
static const std::map<VX2750Pha::PairTriggerLogic, std::string> pairLogicToString = {
    {VX2750Pha::AND, "AND"},
    {VX2750Pha::OR, "OR"},
    {VX2750Pha::NONE, "NONE"}
};

static const std::map<std::string, VX2750Pha::ITLConnect> stringToITLConnect = {
    {"Disabled", VX2750Pha::Disabled},
    {"ITLA", VX2750Pha::ITLA},
    {"ITLB", VX2750Pha::ITLB}
};
static const std::map<VX2750Pha::ITLConnect, std::string> ITLConnectToString = {
    {VX2750Pha::Disabled, "Disabled"},
    {VX2750Pha::ITLA, "ITLA"},
    {VX2750Pha::ITLB, "ITLB"}
};

static const std::map<std::string, VX2750Pha::LVDSMode> stringToLVDSMode = {
     {"SelfTriggers", VX2750Pha::SelfTriggers},
     {"Sync", VX2750Pha::Sync},
     {"IORegister", VX2750Pha::IORegister}
};
static const std::map<VX27850::LVDSMode, std::string> LVDSModeToString = {
     {VX2750Pha::SelfTriggers, "SelfTriggers"},
     {VX2750Pha::Sync, "Sync"},
     {VX2750Pha::IORegister, "IORegister"}    
};

static const std::map<std::string, VX2750Pha::LVDSDirection> stringToLVDSDirection = {
    {"Input", VX2750Pha::Input},
    {"Output", VX2750Pha::Output}
};
static const std::map<VX2750Pha::LVDSDirection, std::string> LVDSDirectionToString = {
    {VX2750Pha::Input, "Input"},
    {VX2750Pha::Output, "Output"}    
};

static const std::map<std::string, VX2750Pha::DACOutMode> stringToDACOutMode = {
    {"Static", VX2750Pha::Static},
    {"IPE", VX2750Pha::IPE},
    {"ChInput", VX2750Pha::ChInput},
    {"MemOccupancy", VX2750Pha::MemOccupancy},
    {"ChSum", VX2750Pha::ChSum},
    {"OverThrSum", VX2750Pha::OverThrSum},
    {"Ramp", VX2750Pha::Ramp},
    {"Sin5MHz", VX2750Pha::Sine},
    {"Square", VX2750Pha::Square}
};
static const std::map<VX2750Pha::DACOutMode, std::string> DACOutModeToString = {
    {VX2750Pha::Static, "Static"},
    {VX2750Pha::IPE, "IPE"},
    {VX2750Pha::ChInput, "ChInput"},
    {VX2750Pha::MemOccupancy, "MemOccupancy"},
    {VX2750Pha::ChSum, "ChSum"},
    {VX2750Pha::OverThrSum, "OverThrSum"},
    {VX2750Pha::Ramp, "Ramp"},
    {VX2750Pha::Sine, "Sin5MHz"},
    {VX2750Pha::Square, "Square"}
};

static const std::map<std::string, VX2750Pha::Polarity> stringToPolarity = {
    {"Positive", VX2750Pha::Positive},
    {"Negative", VX2750Pha::Negative}
};
static const std::map<VX2750Pha::Polarity, std::string> polarityToString = {
    {VX2750Pha::Positive, "Positive"},
    {VX2750Pha::Negative, "Negative"}
};

static const std::map<std::string, VX2750Pha::EventSelection> stringtoEventSelection = {
    {"All", VX2750Pha::All},
    {"Pileup", VX2750Pha::Pileup},
    {"EnergySkim", VX2750Pha::EnergySkim}
};
static const std::map<VX2750Pha::EventSelection, std::string> eventSelectionToString = {
    {VX2750Pha::All, "All"},
    {VX2750Pha::Pileup, "Pileup"},
    {VX2750Pha::EnergySkim, "EnergySkim"}
};

static const std::map<std::string, VX2750Pha::CoincidenceMask> stringToCoincidenceMask = {
    {"Disabled", VX2750Pha::Disabled},
    {"Ch64Trigger", VX2750Pha::Ch64Trigger},
    {"TRGIN", VX2750Pha::TRGIn},
    {"GlobalTriggerSource", VX2750Pha::GlobalTriggerSource},
    {"ITLA", VX2750Pha::ITLA},
    {"ITLB", VX2750Pha::ITLB}
};
static const std::map<VX2750Pha::CoincidenceMask, std::string> coincidenceMaskToString = {
    {VX2750Pha::Disabled, "Disabled"},
    {VX2750Pha::Ch64Trigger, "Ch64Trigger"},
    {VX2750Pha::TRGIn, "TRGIN"},
    {VX2750Pha::GlobalTriggerSource}, "GlobalTriggerSource",
    {VX2750Pha::ITLA, "ITLA"},
    {VX2750Pha::ITLB, "ITLB"}
};
////////////////////////////////////////////////////////////////////////////////
// Public methods

    /**
     * constructor
     *    @param hostOrPid - Host or PID specification
     *    @param isUsb     - True if the connection is via direct USB.
     */
    VX2750Pha::VX2750Pha(const char* hostOrPid, bool isUsb) :
        Dig2Device(hostOrPid, isUsb)
    {
        
    }
    /**
     * destructor - ensures chaining to the base class.
     */
    VX2750Pha::~VX2750Pha() {}
    
    /**
     * getCupVersion
     *    Return the CupVersion string (I wonder is this supposed to be CPU?).
     * @return std::string
     */
    std::string
    VX2750Pha::getCupVersion()
    {
        return getDeviceValue("CupVer");
    }
    /**
     * getFPGA_FwVer
     *     Return firmware version type.
     *  @return std::string
     */
    std::string
    VX2750Pha::getFpgaFWVersion()
    {
        return getDeviceValue("FPGA_FwVer");
    }
    /**
     * Get the firmware type. Note that if the firmware type is anything other
     * than DPP_PHA this class is worthless.
     *
     * @return FWType
     */
    VX2750Pha::FWType
    VX2750Pha::getFirmwareType()
    {
        std::string strFwType = getDeviceValue("FwType");
        return stringToEnum(stringToFwType, strFwType);
    }
    /**
     * getModelCode
     *     Return the module model code.
     * @return std::string
     */
    std::string
    VX2750Pha::getModelCode()
    {
        return getDeviceValue("ModelCode");
    }
    /**
     * getPiggyBackCode
     *    @return std::string - the piggy back card type code.
     */
    std::sring
    VX2750Pha::getPiggyBackCode()
    {
        return getDeviceValue("PBCode");
    }
    /**
     * getModelName
     *    @return std::string - module model code.
     *    
     */
    std::string
    VX2750Pha::getModelName()
    {
        return getDeviceValue("ModelName");
    }
    /**
     * getFormFactor
     * 
     * @return FormFactor - The form factor of the module.
     */
    VX2750Pha::FormFactor
    VX2750Pha::getFormFactor()
    {
        return stringToEnum(stringToFormFactor, getDeviceValue("FormFactor"));
    }
    /**
     * getFamilyCode
     *   @return int - family code number.
     */
    int
    VX2750Pha::getFamilyCode()
    {
        return getDeviceInteger("FamilyCode");
    }
    /**
     * getSerialNumber
     *    @return std::string - serial string.
     */
    std::string
    VX2750Pha::getSerialNumber()
    {
        return getDeviceValue("SerialNum");
    }
    /**
     *   getMotherboardRev
     * @return int - motherboard revision level
     */
    int
    VX2750Pha::getMotherboardRev()
    {
        return getDeviceInteger("PCBrev_MB");
    }
    /**
     * getPiggybackRev
     *    @return int - daughterboard revision level
     */
    int
    VX2750Pha::getPiggybackRev()
    {
        return getDeviceInteger("PCBrev_PB");
    }
    /**
     *  getLicense
     *  @return std::string - license code.
     */
    std::string
    VX2750Pha::getLicense()
    {
        return getDeviceValue("License");    
    }
    /**
     * isLicensed
     * @return bool  True if this board is licensed.
     * @throw std::runtime_error - if the value is not recognized. 
     */
    bool
    VX2750Pha::isLicensed()
    {
        auto value = getDeviceValue("LicenseStatus");
        if (value == "Licensed") {
            return true;
        } else if (value == "Not Licensed") {
            return false;
        } else {
            std::stringstream s;
            s << "isLicensed - invalid value from device: '" << value << "'";
            std::string msg = s.str();
            throw std::runtime_error(msg);
        }
    }
    /**
     * remainingUnlicensedTime
     *   @return int - number of minutes remaining to run on an unlicensed board.
     */
    int
    VX2750Pha::remainingUnlicensedTime()
        {
            return getDeviceInteger("LicenseRemainigTime");
        }
    )
    /**
     * channelCount
     * @return int - number of channels the board has available.
     */
    int
    VX2750Pha::channelCount()
    {
        return getDeviceInteger("NumCh");
    }
    /**
     * bitsOfResolution
     *    @return int -number of bits of resolution th adc has.
     */
    int
    VX2750Pha::bitsOfResolution()
    {
        return getDeviceInteger("ADC_Nbit");
    }
    /**
     * sampleRate
     *   @return int The ADC sample rate in MHz
     */
    int
    VX2750Pha::sampleRate()
    {
        return getDeviceInteger("ADC_SamplRate");
    }
    /**
     * inputRange
     *    @return int   - Volts peak-to-peak of input range.
     */
    int
    VX2750Pha::inputRange()
    {
        return getDeviceInteger("InputRange"); 
    }
    /**
     *  isDifferential
     *     @return bool - true of inputs are diff if single ended, false.
     *     @throw std::runtime_error if the value of the InputType parameter is
     *     unrecognizable.
     */
    bool
    VX2750Pha::isDifferential()
    {
        auto value = getDeviceInteger("InputType");
        switch (value) {
        case 0:
            return false;
        case 1:
            return true;
        default:
            {
                std::stringstream s;
                s << "isDifferential unrecognized value from digitizer: '" << value << "'";
                std::string msg = s.str();
                throw std::runtime_error(msg);
            }
        }
        
    }
    /**
     * inputImpedance
     *    @return int - input impetance in ohms.
     */
    int
    VX2750Pha::inputImpedance()
    {
        return getDeviceInteger("Zin");
    }
    /**
     * ipAddress
     *   @return int  - uint32_t actually with IP address.
     */
    int
    VX2750Pha::ipAddress()
    {
        std::string ip = getDeviceValue("IPAddress");
        return dottedtToInt(ip);
    }
    /**
     * netMask
     *  @return int  Network mask.
     */
    int
    VX2750Pha::netMask()
    {
        std::string netmask = getDeviceValue("Netmask");
        return dottedToInt(netmask);
    }
    /**
     * gateway
     *    @return int - gateway IP
     */
    int
    VX2750Pha::gateway()
    {
        std::string gw = getDeviceValue("Gateway");
        return dottedToInt(gw);
    }
    /**
     *  getClockSource
     *     @return V2750::Pha::ClockSource - The clock source.
     */
    VX2750Pha::getClockSource()
    {
        std::string strSource = getDeviceValue("ClockSource");
        return stringToEnum(stringToClockSource, strSource);
    }
    /**
     * setClockSou8rce
     *    Set the source of the unit's clock:
     *  @param source - a VX2750Pha::ClockSource value selecting the source.
     */
    void
    VX2750Pha::setClockSource(ClockSource selection)
    {
        std::string src = enumToString(clockSourceToString, selecction);
        SetValue("ClockSource", src.c_str());
    }
    /**
     * isClockOutOnP0
     *    @return bool - true if the clock output is enabled on the backplane P0 connector.
     */
    void
    VX2750Pha::isClockOutOnP0()
    {
        return  getDeviceBool("EnClockOutP0");
        

    }
    /**
     * setClockOutOnP0
     *    Enable/disable the clock to be output on the P0 connector.
     *  @param bool - Desired state, true to enable.
     */
    void
    VX2750Pha::setClockOutOnP0(bool state)
    {
        SetDeviceValue("EnClockOutP0", state);
    }
    /**
     * isClockOutOnP0
     *    @return bool - true if the clock is enabled to be output on the front
     *                   panel.
     */
    bool
    VX2750Pha::isClockOutOnFP()
    {
        return getDeviceBool("EnClockOutFP");
        
    }
    /**
     * setCLockoutOnFP
     *     Enable/disable the clock to be output to the front panel.
     * @param bool  - desired state - true means output false don't.
     */
    void
    VX2750Pha::setCLockOutOnFP(bool state)
    {
        
        setDeviceValue("EnClockOutFP", state);
    }
    /**
     * getStartSource
     *   @return VX2750Pha::StartSource - describes the source selected to start
     *          digitizer runs.
     */
    VX2750Pha::StartSource
    VX2750Pha::getStartSource()
    {
        std::string strValue = GetDeviceValue("StartSource");
        return stringToEnum(stringToStartSource, strValue);
    }
    /**
     * setStartSource
     *    @param src - VX2750Pha::StartSource that selects what will start
     *       data acquisition on a digitizer.
     */
    void
    VX2750Pha::setStartSource(StartSource, selection)
    {
        std::string strValue =  enumToString(startSourceToString, selection);
        SetDeviceValue("StartSource", strValue.c_str());
    }
    /**
     * getGLobalTriggerSource
     *   @return GlobalTriggerSource
     */
    VX2750Pha::GlobalTriggerSource
    VX2750Pha::getGlobalTriggerSource()
    {
        std::string strValue = GetDeviceValue("GlobalTriggerSource");
        return stringToEnum(stringToGlobalTriggerSource, strValue);
    }
    /**
     * setGlobalTriggerSource
     *    @param sel - VX2750Pha::GlobalTriggerSource that selects the global
     *                  trigger source.
     */
    void
    VX2750Pha::setGlobalTriggerSource(GlobalTriggerSource sel)
    {
        std::string strValue = enumToString(globalTriggerSourceToString, sel);
        SetDeviceValue("GlobalTriggerSource", strValue.c_str());
    }
    /**
     * getWaveTriggerSource
     *     @param ch  - A digitizer channel number
     *     @return VX2750Pha::WaveTriggerSource - the wave trigger source of ch.
     */
    VX2750Pha::WaveTriggerSource
    VX2750Pha::getWaveTriggerSource(unsigned ch)
    {
        std::string strValue GetChanValue(ch, "WaveTriggerSource");
        return stringToEnum(stringToWaveTrigger, strValue);
    }
    /**
     * setWaveTriggerSource
     *
     *    @param ch - channel number
     *    @param selection - WaveTriggerSource  to set as thee selected value.
     */
    void
    VX2750Pha::setWaveTriggerSource(unsigned ch, WaveTriggerSource selection)
    {
        std::string value = enumToString(waveTriggerToString, selection);
        SetChanValue(ch, "WaveTriggerSource", value.c_str());
    }
    /**
     * getEventTriggerSource
     *    @param ch   - a digitizer channel
     *    @return VX2750Pha::EventTriggerSource - source of ch's event trigger.
     */
    VX2750Pha::EventTriggerSource
    VX2750Pha::getEventTriggerSource(unsigned ch)
    {
        std::string strValue = GetChanValue(ch, "EventTriggerSource");
        return strintToEnum(stringToEventTrigger, strValue);
    }
    /**
     * setEventTriggerSource
     *    @param ch - a digitizer channel number.
     *    #param selection - the VX2750Pha::EventTriggerSource selected for that channel.
     */
    void
    VX2750Pha::setEventTriggerSource(unsigned ch, EventTriggerSource selection)
    {
        std::string sel = enumToString(EventTriggerToString, selection);
        SetChanValue(ch, "EventTriggerSource", sel.c_str());
    }
    /**
     * getTimestampResetSource
     *    @return VX2750Pha::TimestampResetSource - the digitizer's source for
     *                resettting its timestamp.
     */
    VX2750Pha::TimestampResetSource
    VX2750Pha::getTimestampResetSource()
    {
        std::string strValue = GetDeviceValue("TstampResetSource");
        return stringToEnum(stringToTimestampReset, strValue);
    }
    /**
     * setTimetsampResetSource
     *     @param source - the digitizer's source for resetting the timestamp values.
     */
    void
    VX2750Pha::setTimestampResetSource(TimestampResetSource source) {
        std::string strSource = enumToString(timeStampResetToString, source);
        SetDeviceValue("TstampResetSource", strSource.c_str());
    }
    /**
     *    getChannelTriggerMask
     *  @param ch - Digitizer channel
     *  @return std::uint64_t - trigger mask fr the channel.
     *  @note the base class does not provide for 64 bit integer values --
     */
    std::uint64_t
    VX2750Pha::getChannelTriggerMask(unsigned ch)
    {
        std::string strValue = GetChanValue(ch, "ChannelTriggerMask");
        std::stringstream s(strValue);
        std::uint64_t result;
        s >> result;
        return result;
    }
    /**
     * setChannelTriggerMask
     *  @param ch   - The channel to set.
     *  @param mask - the trigger mask to set.
     */
    void
    VX2750Pha::setChannelTriggerMask(unsigned ch, std::uint64_t mask)
    {
        std::stringstream s;
        s << mask;
        std::string value = s.str();
        SetChanValue(ch, "ChannelTriggerMask", value.c_str());
    }
    /** getTraceRecordMode
     *    @param ch - digitizer channel.
     *    @return VX2750Pha::TraceRecordMode - the trace record mode for that channel.
     */
    VX2750Pha::TraceRecordMode
    VX2750Pha::getTraceRecordMode(unsigned chan)
    {
        std::string strValue = GetChanValue(ch, "WaveSaving");
        return stringToEnum(stringToTraceRecord, strValue);
    }
    /**
     * setTraceRecordMode
     *    @param ch - digitizer channel.
     *    @param mode - desired trace recording mode for that channel.
     */
    void
    VX2750Pha::setTraceRecordMode(TraceRecordMode mode)
    {
        std::string value =  enumToString(traceRecordToString, mode);
        SetChanValue(ch, "WaveSaving", value.c_str());
    }
    /**
     * getTRGOUTMode
     *    Get what's selected to be output on the TRGOUT connector.
     * @return TRGOUTMode
     */
    VX2750Pha::TRGOUTMode
    VX2750Pha::getTRGOUTMode()
    {
        std::string strMode = GetDeviceValue("TrgOutMode");
        return stringToEnum(stringToTRGOUT, strMode);
    }
    /**
     * setTRGOUTMode
     *    Select what's on the TRGOUT output.
     * @param select - VX2750Pha::TRGOutMode selecting what's output.
     */
    void
    VX2750Pha::setTRGOutMode(TRGOUTMode select)
    {
        std::string strMode = enumToString(TRGOUTToString, select);
        SetDeviceValue("TrgOutMode", strMode.c_str());
    }
    /**
     * getGPIOMode
     *    @return VX2750Pha::GPIOMode - value indicating what is signaled on the GPIO output.
     */
    VX2750Pha::GPIOMode
    VX2750Pha::getGPIOMode()
    {
        std::string value = GetDeviceValue("GPIOMode");
        return stringToEnum(stringToGPIO, value);
    }
    /**
     * setGPIOMode
     *    @param mode  - the desired output on the GPIO output.
     */
    void
    VX2750Pha::setGPIOMode(GPIOMode mode)
    {
        std::string value = enumToString(GPIOToString, mode);
        SetDeviceValue("GPIOMode", value.c_str());
    }
    /**
     * getBusyInputSource
     *   @return VX2750Pha::BusyInSource - The current selection for the board busy.
     */
    VX2750Pha::BusyInputSource
    VX2750Pha::getBusyIn()
    {
        std::string value = GetDeviceValue("BusyInSource");
        return stringToEnum(stringToBusyIn, value);
    }
    /** setBusyInputSource
     *  @param src - desired busy source.
     */
    void
    VX2750Pha::setBusyInputSource(VX2750Pha::BusyInSource src)
    {
        std::string value = enumToSTring(BusyInToString, src);
        SetDeviceValue("BusyInSource", value.c_str());
    }
    /**
     *  getSyncOutMode
     *     @return VX2750Pha::SyncOutMode - the current selection of the CLKOUT connector
     */
    vx2750pHA::SyncOutMode
    VX2750Pha::SyncOutMode()
    {
        std::string value = GetDeviceValue("SyncOutMode");
        return stringToEnum(stringToSyncOut, value);
    }
    /**
     * setSyncOutMode
     *    @param mode - desired output on the CLKOUT
     */
    void
    VX2750Pha::setSyncOutMode(SyncOutMode mode)
    {
        std::string value =  enumToString(syncOutToString, mode);
        SetDeviceValue("SyncOutMode", value.c_str());
        
    }
    /**
     * getBoardVetoSource
     *    @return V2750::VetoSource - condition used to veto acquisitiontriggers.
     */
    VX2750Pha::VetoSource
    VX2750Pha::getBoardVeto()
    {
        std::string src = GetDeviceValue("BoardVetoSource");
        return stringToEnum(stringToVeto, src);
    }
    /**
     * setBoardVetoSource
     *   @param src  - Specifies where the board VETO comes from.
     */
    void
    VX2750Pha::setBoardVetoSource(VetoSource src)
    {
        std::string value = enumToString(vetoToString, src);
        SetDeviceValue("BoardVetoSource", value.c_str());
    }
    /**
     * getBoardVetoWidth
     *     Gets amount of strecth in the board veto in ns.  Note that 0 means
     *     the veto is only active as long as the source is asserted
     *  @return int - nanoseconds of stretch
     */
    std::uint64_t
    VX2750Pha::getBoardVetoWidth()
    {
        // Longer than an integer so:
        
        std::string strValue getDeviceValue("BoardVetoWidth");
        std::stringstream s(strValue);
        std::uint64_t result;
        s >> result;
        return result;
    }
    /**
     * setBoardVetoWidth
     *    @param ns - number of nanoseconds the veto should be stretched after
     *                the source is deasserted.
     */
    void
    VX2750Pha::setBoardVetoWidth(std::uint64_t ns)
    {
        // since SetDeviceValue overloads to in ints and this can be longer:
        
        std::stringstream strValue;
        strValue << ns;
        std::string value = strValue.str();
        SetDeviceValue("BoardVetoWidth", value.c_str());
    }
    /**
     * getBoardVetoPolarity
     *    @return VX2750Pha::VetoPolarity  - the selected veto source poliarity.
     */
    VX2750Pha::VetoPolarity
    VX2750Pha::getBoardVetoPolarity()
    {
        std::string strValue = GetDeviceValue("BoardVetoPolarity");
        return stringToEnum(stringToVetoPolarity, strValue);
    }
    /**
     * setVetoPolarity
     *    @param pol  - Which polarity makes the veto source asserted.
     */
    void
    VX2750Pha::setBoardVetoPolarity(VetoPolarity pol)
    {
        std::string strValue = enumToString(vetoPolarityToString, pol);
        SetDeviceValue("BoardVetoPolarity", strValue.c_str());
    }
    /**
     * getChannelVetoSource
     *    Return the source of a per channel veto.
     *
     *  @param ch - channel number
     *  @return VX2750Pha::ChannelVetoSource - veto source for ch.
     */
    VX2750Pha::ChannelVetoSource
    VX2750Pha::getChannelVetoSource(unsigned ch)
    {
        std::string strValue = GetChanValue(ch,  "ChannelVetoSource");
        return stringToEnum(stringToChannelVeto, strValue);
    }
    /**
     * setChannelVetoSource
     *    @param ch - channel number
     *    @param src - Source of the veto for that channel.
     */
    void
    VX2750Pha::setChannelVetoSource(unsigned ch, ChannelVetoSource src)
    {
        std::string strValue = enumToString(channelVetoToString, src);
        SetChanValue(ch,  "ChannelVetoSource", strValue.c_str());
    }
    /**
     *     getChanneVetoWidth
     *   Returns the number of nanoseconds for a channel veto (if the source
     *   is ADC under or over saturation).
     *   the deassertion of the source.
     *
     *  @param chan - the channel.
     *  @return std::uint32_t - the stretch.
     */
    std::unit32_t
    VX2750Pha::getChannelVetoWidth(unsigned chan)
    {
        return GetChanInteger(chan, "ADCVetoWidth");
    }
    /**
     * setChannelVetoWidth
     *   Sets the adc channel veto width for a channel if the source is
     *   over/under saturation.
     * @param chan - channel number.
     * @param ns   - nanoseconds.
     */

    void
    VX2750Pha::setChannelVetoWidth(unsigned chan, std::uint32_t ns)
    {
        SetChanValue(chan, "ADCVetoWidth", ns);
    }
    /**
     * getRunDelay
     *    Reads the delay between electronic (not software) run start and
     *    actual run start.  This is intended to compensate for propagation delays
     *    on a start daisy chain (earlier modules delay longer), and provides
     *    a fully synchronized start of data acquisition in multiple modules.
     *    
     *  @return std::uint32_t nanoseconds delay
     */
    std::uint32_t
    VX2750Pha::getRunDelay()
    {
        return GetDeviceInteger("RunDelay");
    }
    /**
     * setRunDelay
     *    Set the run delay (see above).
     *  @param ns - nanoseconds of run delay requested.
     */
    void
    VX2750Pha::setRunDelay(std::uint32_t ns)
    {
        SetDeviceValue("RunDelay", ns);
    }
    /**
     * isAutoDisarmEnabled
     *    @return bool - If true, the digitizer is set to disarm at the end of
     *                   a run.  If false, the digitizer remains armed when the run end.
     */
    
    bool
    VX2750Pha::isAutoDisarmEnabled()
    {
        return textToBool(GetDeviceValue("EnAutoDisarmAcq"));
        
    }
    /**
     * setAutoDisarmEnabled
     *    Set the state of the auto disarm
     *  @param state - desired state.
     */
    void
    VX2750Pha::setAutoDisarmEnabled(bool state)
    {
        std::string stringValue = state ? "True" : "False";
        SetDeviceValue("EnAutoDisarmAcq", stringValue.c_str());
    }
    /**
     * isMultiWindowRunEnabled
     * @return bool - state: if true, runs last for the duration of the RUN input
     *                 signal. If false, runs can last for the duration of several
     *                 RUN Input transitions.  See the methods that control thye
     *                 timestamp pause parameter.
     */
    bool
    VX2750Pha::isMultiWindowRunEnabled()
    {
        return textToBool(GetDeviceValue("EnMultiWindowRun"));
    }
    /**
     * setMultiWindowRunEnable
     *    @param state - desired state.  See above.
     */
    void
    VX2750Pha::setMultiWindowRunEnable(bool state) {
        std::string strValue = state ? "True" : "False";
        SetDeviceValue("EnMultiWindowRun", strValue.c_str());
    }
    /**
     * isTimestampHoldEnabled
     *    @bool - True if timestamp increments are suspended when the run is paused
     */
    bool
    VX2750Pha::isTimestampHoldEnabled()
    {
        std::string value = GetDeviceValue("PauseTimeStamp");
        // Hold -> true, Run -> False.
        
        if (value == "Hold") {
            return true;
        } elseif (value == "Run") {
            return false;
        } else {
            std::sstream strError;
            strError << "isTimestampHoldEnabled got an unrecognized value: '"
                << value << "'";
            std::string msg = strError::str();
            throw std::runtime_error(msg);
        }
    }
    /**
     * setPauseTimestampHold
     *    @param bool - true if the timestamp should not increment (Hold)
     *                  when runs are paused.
     */
    void
    VX2750Pha::setPauseTimestampHold(bool enable)
    {
        std::string value = enable ? "Hold" : "Run";
        SetDeviceValue("PauseTimestamp", value.c_str());
    }
    /**
     * getLEDStatus
     *    Return a 32 bit value bit bits set indicating the LED
     *    status.  Symbolic bitmask definitions are in VX2750Pha::LED_*
     * @return std::uint32
     */
    std::uint32_t
    VX2750Pha::getLedStatus()
    {
        return GetDeviceInteger("LedStatus");
        
    }
    /**
     * getAcquisitionStatus
     *   @return std::uint32_t - bit mask indicating acquisition status.
     *          The VX2750Pha::ACQ_* symbols provide symbolic definitions for the
     *          bit masks for each git in the response
     */
    std::uint32_t
    VX2750Pha::getAcquisitionStatus()
    {
        return GetDeviceInteger("AcquisitionStatus");
    }
    /**
     * getMaxRawDataSize
     *    @return std::uint32_t The largest number of bytes of data that can be
     *                          returned in reads from the raw endpoint.
     */
    std::uint32_t
    VX2750Pha::getMaxRawDataSize()
    {
        return GetDeviceInteger("MaxRawDataSize");
    }
    /**
     *  getVolatileClockDelay
     *       Gets the current delay in clock output relative to the input reference
     *       clock.
     * @return double
     */
    double
    VX2750Pha::getVolatileCLockDelay()
    {
        return GetDeviceReal("VolatileClockOutDelay");
    }
    /**
     *  setVolatileClockDelay
     *     @param ps - picoseconds of delay between input and output clock.
     */
    void
    VX2750Pha::setVolatileCLockDelay(double ps)
    {
        SetDeviceValue("VolatileClockOutDelay", ps);
    }
    /**
     * getPermanentClockOutDelay
     *    @return double -ps - clock delay loaded into the hardware on power
     *                         up.
     */
    double
    VX2750Pha::getPermanentClockDelay()
    {
        return GetDeviceReal("PermanentClockOutDelay");
    }
    /**
     * setPermanentCLockDelay
     *     @param double ps - picoseconds delay loaded on power up into the clock
     *                        delay.
     */
    void
    VX2750Pha::setPermanentClockDelay(double ps)
    {
        SetDeviceValue("PermanentClockOutDelay", ps);
    }
    /**
     * getWaveDataSource
     *     @param ch - channelnumber.
     *     @return VX2750Pha::WaveDataSource - enumerated value that describes
     *              the source of waveform data sent to pulse processing
     *              for that channel.
     */
    VX2750Pha::WaveDataSource
    VX2750Pha::getWaveDataSource(unsigned ch)
    {
        return stringToEnum(stringToWaveDataSource, GetChannelValue(ch, "WaveDataSource"));
    }
    /**
     * setWaveDataSource
     *    @param ch - channel to set.
     *    @param selection - select the waveform data source sent to DPP
     */
    void
    VX2750Pha::setWaveDataSource(unsigned chan, WaveDataSource, selection)
    {
        std::string value = enumToString(waveDataSourcToString, selection);
        SetChannelValue(ch, "WaveDataSource", value.c_str());
    }
    /**
     * getRecordSamples
     *    @param ch - channel number.
     *    @return int - number of samples of waveform that will be acquired.
     */
    std::uint32_t
    VX2750Pha::getRecordSamples(unsigned ch)
    {
        return GetChanInteger(ch, "ChRecordLengthS");
    }
    /**
     * setRecorcdSamples
     *    @param ch - channel number
     *    @param samples - number of samples to acquire.
     */
    void
    VX2750Pha::setRecordSamples(unsigned ch, std::uint32_t samples)
    {
        SetChanValue(ch, "ChRecordLengthS", samples);
    }
    /**
     * getRecordNs
     *   @param ch - channel number.
     *   @return std::uint32_t - number of nanoseconds of waveform that will be recorded.
     */
    std::uint32_t
    VX2750Pha::getRecordNs(unsigned ch)
    {
        GetChanInteger(ch, "ChRecordLengthT");
    }
    /**
     * setRecordNs
     *    @param ch - channel number.
     *    @param ns - Desired of captured waveform.  Note that the actual
     *                set value may differ if ns is not an exact number of samples
     */
    void
    VX2750Pha::setRecordNs(unsigned ch, std::uint32_t ns)
    {
        SetChanValue(ch, "ChRecordLengthT", ns);
    }
    /**
     * getWaveResolution
     *     Return the actual time covered by a waveform sample.
     *     This supports saveform decimation.
     * @param ch - channel.
     * @return WaveResolution - Enum indication the selected resolution.
     */
    VX2750Pha::WaveResolution
    VX2750Pha::getWaveResolution(unsigned ch)
    {
        std::string value = GetChanValue(ch, "WaveResolution");
        return stringToEnum(stringToWaveResolution, value);
    }
    /**
     * setWaveResolution
     *    @param ch - channel to set.
     *    @param sel - selector.
     */
    void
    VX2750Pha::setWaveResolution(unsigned ch, WaveResolution sel)
    {
        std::string value = enumToString(waveResolutionToString, sel);
        SetChaneValue(ch, "WaveResolution", value.c_str());
    }
    /**
     * getAnalogProbe
     *   @param ch  - channel number.
     *   @param probeNum - probe number , 0 or 1.
     *   @return VX2750Pha::AnalogProbe.
     */
    VX2750Pha::AnalogProbe
    VX2750Pha::getAnalogProbe(unsigned ch, unsigned probeNum)
    {
        checkInclusiveRange(0, 1, probeNum);         // Good probe number.
        std::string param = appendNumber("WaveAnalogProbes", probeNum);
        
        std::string value = GetChanValue(ch, param.c_str());
        return stringToEnum(stringToWaveResolution, value);
    }
    /**
     * setAnalogProbe
     *   @param ch - channel number,
     *   @param probeNum -probe number.
     *   @param selection - selected prob3e.
     */
    void
    VX2750Pha::setAnalogProbe(unsigned ch, unsigned probeNum, AnalogProbe selection)
    {
        checkInclusiveRange(0, 1, probeNum);
        std::string param = appendNumber("WaveAnalogProbes", probeNum);
        
        std::stringValue = enumToString(analogProbeToString, selection);
        SetChanValue(ch, param.c_str(), stringValue.c_str());
    }
    /**
     * getDigitalProbe
     *   @param ch - channel
     *   @param probe - number of the probe to get.
     *   @return VX2750Pha::DigitalProbe
     */
    VX2750Pha::DigitalProbe
    VX2750Pha::getDigitalProbe(unsigned ch, unsigned probe)
    {
        checkInclusiveRange(0, 3, probe);
        std::string param = appendNumber("WaveDigitalProbe", probe);
        std::string value = GetChanValue(ch, param.c_str());
        return stringToEnum(stringToDigitalProbe, value);
    }
    /**
     * setDigitalProbe
     *   @param ch - channel to set.
     *   @param probe - number of probe to set [0-3]
     *   @param selection  - The probe to select
     */
    void
    VX2750Pha::setDigitalProbe(unsigned ch, unsigned probe, DigitalProbe selection)
    {
        checkInclusiveRange(0, 3, probe);
        std::string param ("WaveDigitalProbe", probe);
        std::string value = enumToString(digitalProbeToString, probe);
        SetChanValue(ch, param.c_str(), value.c_str());
    }
    /**
     * getPreTriggerSamples
     *    @param chan  - channel numbger.
     *    @return std::uint32_t nsamples -samples in the pretrigger for that channel.
     */
    std::uint32_t
    VX2750Pha::getPreTriggerSamples(unsigned chan)
    {
        return GetChanInteger(chan, "ChPreTriggerS");
    }
    /**
     * setPreTriggerSamples
     *    @param ch - channel number.
     *    @param samples - number of samples to have in the channels' pretrigger window.
     */
    void
    VX2750Pha::setPreTriggerSamples(unsigned ch, std::uint32_t samples)
    {
        SetChanValue(ch, "ChPreTriggerS", samples);
    }
    /**
     * getPreTriggerNs
     *    Get the pretrigger time window in nanoseconds.
     *  @param ch - channel
     *  @return std::uint32_t - nanoseconds.
     */
    std::uint32_t
    VX2750Pha::getPreTriggerNs(unsigned chan)
    {
        return GetChanInteger(chan, "ChPreTriggerT");
    }
    /**
     * setPreTriggerNs
     *    Set the pretrigger time window in nanoseconds.
     *  @param ch - channel
     *  @param ns -desired pretrigger window.
     */
    void
    VX2750Pha::setPreTriggerNs(unsigned chan, std::uint32_t ns)
    {
        SetChanValue(chan, "ChPreTriggerT", ns);
    }
    
    /**
     * getTestPulsePeriod
     *    @return std::uint32_t = nanoseconds betwee repetitions of the test pulse.
     */
    std::uint32_t
    VX2750Pha::getTestPulsePeriod()
    {
        return GetDeviceInteger("TestPulsePeriod");
    }
    /**
     * setTestPulsePeriod.
     * @param ns - desired nanoseconds between repetitions of the test pulse.
     */
    void
    VX2750Pha::setTestPulsePeriod(std::uint32_t ns)
    {
        SetDeviceValue("TestPulsePeriod", ns);
    }
    /**
     * getTestPulseWidth
     *    @return std::uint32_t test pulse width in ns.
     */
    std::uint32_t
    VX2750Pha::getTestPulseWidth()
    {
        return GetDeviceInteger("TestPulseWidth");
    }
    /**
     * setTestPulseWidth
     *    @param ns - desired nanonseconds for which the test pulse is high.
     */
    void
    VX2750Pha::setTestPulseWidth(std::uint32_t ns)
    {
        SetDeviceValue("TestPulseWidth", ns);
    }
    /**
     * getTestPulseLowLevel
     * @return std::uint32_t  ADC Counts for the low value of the test pulse.
     */
    std::uint32_t
    VX2750Pha::getTestPulseLowLevel()
    {
        return GetDeviceInteger("TestPulseLowLevel");
    }
    /**
     * setTestPulseLowLevel
     *   @param counts - adc counts for the test pulse low level.
     */
    void
    VX2750Pha::setTestPulseLowLeve(std::uint32_t counts)
    {
        SetDeviceValue("TestPulseLowLevel", counts);
    }
    /**
     * getTestPulseHigHlevel
     *    @return std::uint32_t  - adc counts when the test pulser is high
     */
    std::uint32_t
    VX2750Pha::getTestPulseHighLevel()
    {
        return GetDeviceInteger("TestPulseHighLevel");
    }
    /**
     * setTestPulseHighLevel
     *    @param counts - Desired ADC counts fo the high level of the test pulser.
     */
    void
    VX2750Pha::setTestPulseHighLevel(std::uint32_t counts)
    {
         SetDeviceValue("TestPulseHighLevel", counts);
    }
    /**
     * getIOLevel
     *   @return VX2750Pha::IOLevel - I/O signalling level for the TRGIN, SIN, TRGOUT
     *                                 I/Os.
     */
    VX2750Pha::IOLevel
    VX2750Pha::getIOLevel()
    {
        return stringToEnum(stringToIOlevel, GetDeviceValue("IOLevel"));
    }
    /**
     * setIOLevel
     *   @param level - desired I/O signalling level.
     */
    void
    VX2750Pha::setIOLevel(IOLevel level)
    {
        std::string value = enumToString(ioLevelToString, level);
        SetDeviceValue("IOLevel", value.c_str());
    }
    /**
     * getAirInTemp
     *    @return double - the temperature in C at the air intakes
     */
    double
    VX2750Pha::getAirInTemp()
    {
        return GetDeviceReal("TempSensAirIn");
    }
    /**
     * getAirOutTemp
     *    Get the temperature of the board exhaust
     * @return double
     */
    double
    VX2750Pha::getAirOutTemp()
    {
        return GetDeviceReal("TempSensAirOut");
    }
    /**
     * getCoreTemp
     *    Get the temperature at the FPGA (board core).
     * @return double
     */
    double
    VX2750Pha::getCoreTemp()
    {
        return GetDeviceReal("TempSensCore");
    }
    /**
     * getFirstADCTemp
     *   @return double the temp in C at the first ADC sensor.
     */
    double
    VX2750Pha::getFirstADCTemp()
    {
        return GetDeviceReal("TempSensFirstADC");
    }
    /**
     * getLastADCTemp
     *  @return double the temp in C at the last ADC sensor.
     */
    double
    VX2750Pha::getLastADCTemp()
    {
        return GetDeviceReal("TempSensLastADC");
    }
    /**
     * getHottestADCTemp
     *    @return double - The temp in C at the hottest ADC.
     */
    double
    VX2750Pha::getHottestADCTemp()
    {
        return GetDeviceReal("TempSensHottestADC");
    }
    /**
     * getADCTemp
     *    Get the temperature at a specific adc chip temperature sensor.
     *  @param chip - ADC number [0-7].
     *  @return double - Temperature in C at that chip's sensor.
     */
    double
    VX2750Pha::getADCTemp(unsigned chip)
    {
        checkInclusiveRange(0,7, chip);
        std::string paramName = appendNumber("TempSensADC", chip);
        
        return GetDeviceReal(paramName.c_str());
    }
    /**
     * getDCDCConverterTemp
     *    @return double - Temperature in C at the DC DC voltage converter.
     */
    double
    VX2750Pha::getDCDCConverterTemp()
    {
        return GetDeviceReal("TempSensDCDC");
    }
    /**
     * getDCDCConverterInputVoltage
     *   @return double - Volts at the input sensor to the DC-DC converter.
     */
    double
    VX2750Pha::getDCDCConverterInputVoltage()
    {
        return GetDeviceReal("VInSensDCDC");
    }
    /*
     * getDCDCConverterOutputVoltage
     *  @return double - volts at the output sensor of the DC-DC converter.
     */
    double
    VX2750Pha::getDCDCConverterOutputVoltage()
    {
        return GetDeviceReal("VOutSensDCDC");
    }
    /**
     * getDCDCConverterAmps
     *     @return double - DCDC converter current in amps.
     */
    double
    VX2750Pha::getDCDCCONverterAmps()
    {
        return GetDeviceReal("IOutSensDCDC");
    }
    /**
     * getDCDCCOnverterHz
     *    @return double - frequence of the DCDC Coverter in Hz.
     */
    double
    VX2750Pha::getDCDCConverterHz()
    {
        return GetDeviceReal("FreqSensCore");
    }
    /**
     * getDCDCConverterDutyCycle
     *   @return double - percentage duty cycle of the DCDC converter.
     */
    double
    VX2750Pha::getDCDCConverterDutyCycle()
    {
        return GetDeviceReal("DutyCycleSensDCDC");
    
    }
    /**
     * getFanSpeed
     *    @param fan - Fan whose speed we want in the range [0,1].
     *    @return std::uint32_t - RPMs of the specified fan.
     *    @note this is only supported for the desktop form factor devices.
     */
    std::uint32_t
    VX2750Pha::getFanSpeed(unsigned fan)
    {
        checkInclusiveRange(0,1, fan);
        std::string paramName = appendNumber("SpeedSensFan", fan);
        return GetDeviceInteger(paramName.c_str());
    }
    /**
     * getErrorFlagMask
     *    Fetches the error bits that drive the LEDS on the device front panel.
     *    @return std::uint32_t - Bit encoded see VX2750Pha::ERR_* definitions.
     */
    std::uint32_t
    VX2750Pha::getErrorFlagMask()
    {
        return GetDeviceInteger("ErrorFlagMask");
    }
    /**
     * setErrorFlagMask
     *    Not sure why we're allowed to set this but the manual says this is
     *    a writable parameter.
     * @param mask - desired error flag mask.
     */
    void
    VX2750Pha::setErrorFlagMask(std::uint32_t mask)
    {
        SetDeviceValue("ErrorFlagMask", mask);
    }
    /**
     * getErrorFlagDataMask
     *    @return std::uint32_t the bit encoded error mask that will be included
     *               in the data.
     */
    std::uint32_t
    VX2750Pha::getErrorDataMask()
    {
        return GetDeviceInteger("ErrorFlagDataMask");
    }
    /**
     * setErrorFlagDataMask
     *    Again this, for reasons uknown to me is somehow writable.
     * @param mask - the Desired error data mask.
     */
    void
    VX2750Pha::setErrorDataMask(std::uint32_t mask)
    {
        SetDeviceValue("ErrorFlagDataMask", mask);
    }
    /**
     * getErrorFlags
     *    @return std::uint32_t - the error flags.  These are also, I think,
     *         encoded via the VX2750Pha::ERR_ bitmasks.
     */
    std::uint32_t
    VX2750Pha::getErrorFlags()
    {
        return GetDeviceInteger("ErrorFlags");
    }
    /**
     * isBoardReady
     *    @return bool - True if there are no error flags set else false
     */
    bool
    VX2750Pha::isBoardReady()
    {
        return textToBool(GetDeviceValue("BoardReady"));
    }
    /**
     * getITLAMainLogic
     *     See the figure in 2.5.1 - Returns the main logic selected for individual
     *     trigger A:
     *  @return VX2750Pha::IndividualTriggerLogic - the selection
    */
    VX2750Pha::IndividualTriggerLogic
    VX2750Pha::getITLAMainLogic()
    {
        std::string value = GetDeviceVazlue("ITLAMainLogic");
        return stringToEnum(stringToIndividualTriggerLogic, value);
    }
    /*v*
     * getITLBMainLogic.
     *   @return VX2750Pha::IndividualTriggerLogic - same as above but for trigger B.
     */
    VX2750Pha::IndividualTriggerLogic
    VX2750Pha::getITLBMainLogic()
    {
        std::string value = GetDeviceValue("ITLBMainLogic");
        return stringToEnum(stringToIndividualTriggerLogic, value);
    }
    /**
     * setITLAMainLogic
     *    Select the desired condition for the A logic trigger.
     * @param selection - The desired logic selection.
     */
    void
    VX2750Pha::setITLAMainLogic(IndividualTriggerLogic selection)
    {
        std::string value = enumToString(individualTriggerLogicToString, selection);
        SetDeviceValue("ITLAMainLogic", value.c_str());
    }
    /**
     * setITLBMainLogic
     *    Select the desired condition for the A logic trigger.
     * @param selection - The desired logic selection.
     */
    void
    VX2750Pha::setITLBMainLogic(IndividualTriggerLogic selection)
    {
        std::string value = enumToString(individualTriggerLogicToString, selection);
        SetDeviceValue("ITLBMainLogic", value.c_str());
    }
    /**
     * getITLAMajorityLevel
     *   @return unsigned - Majority level for trigger logic A. only meaningful if
     *                      ITLB's main logic has been set to majority.
     */
    unsigned
    VX2750Pha::getITLAMajorityLevel()
    {
        return GetDeviceInteger("ITLAMajorityLev");
    }
    /**
     * getITLBMajorityLevel
     *   @return unsigned - same as above but for trigger logic B.
     */
    unsigned
    VX2750Pha::getITLBMajorityLevel()
    {
        return GetDeviceInteger("ITLBMajorityLev");
    }
    /**
     * setITLAMajoritylevel
     *   @param level - the number of channels that must be triggering in coincidence
     *                  to make ITLA trigger fire if it is in majority mode.
     */
    void
    VX2750Pha::setITLAMajorityLevel(unsigned level)
    {
        SetDeviceValue("ITLAMajorityLev", level);
    }
    /**
     *  setITLBMajorityLevel
     *    @param level - same as above but for ITLB
     */
    void
    VX2750Pha::setITLBMajorityLevel(unsigned level)
    {
        SetDeviceValue("ITLBMajorityLev", level);
    }
    /**
     * getITLAPairLogic
     *    Returns how channel pairs are combined and fed into the trigger logic
     *    blocks.
     * @return PairTriggerLogic - see 2.5.3 of the PHA Parameter manual for
     *        a full description of this part of the trigger logic but note that
     *        None and OR have not so subtle differences.
     */
    VX2750Pha::PairTriggerLogic
    VX2750Pha::getITLAPairLogic()
    {
        return stringToEnum(stringToPairLogc, GetDeviceValue("ITLAPairLogic"));
    }
    /**
     * getITLBPairLogic
     *    @return PairTriggerLogic - same as above but for the B trigger block.
     */
    VX2750Pha::PairTriggerLogic
    VX2750Pha::getITLBPairLogic()
    {
        return stringToEnum(stringToPairLogc, GetDeviceValue("ITLBPairLogic"));
    }
    /**
     * setITLAPairLogic
     *    Selects the logic fed to the ITLA trigger logic by channel self triggers
     *    for adjacent pairs.
     *  @param sel - the pair logic selection
     */
    void
    VX2750Pha::setITLAPairLogic(PairTriggerLogic sel)
    {
        std::string value = enumToString(pairLogicToString, sel);
        SetDeviceValue("ITLAPairLogic", value.c_str());
    }
    /**
     * setITLBPairLogic
     *    Same as above but for the B trigger logic logic block.
     */
    void
    VX2750Pha::setITLBPairLogic(PairTriggerLogic sel)
    {
        std::string value = enumToString(pairLogicToString, sel);
        SetDeviceValue("ITLBPairLogic", value.c_str());
    }
    /**
     * isITLAInverted
     *    @bool - true if the output of ITLA is inverted.
     */
    bool
    VX2750Pha::isITLAInverted()
    {
        std::string value = GetDeviceValue("ITLAPolarity");
        if (value == "Direct") return false;
        if (value == "Inverted") return true;
        std::stringstream msg;
        msg << "VX2750Pha::isITLAInverted - unrecognized return string : ";
        msg << value;
        throw std::runtime_error(msg);
    }
    bool
    VX2750Pha::isITLBInverted()
    {
        std::string value = GetDeviceValue("ITLBPolarity");
        if (value == "Direct") return false;
        if (value == "Inverted") return true;
        std::stringstream msg;
        msg << "VX2750Pha::isITLAInverted - unrecognized return string : ";
        msg << value;
        throw std::runtime_error(msg);
    }
    /**
     * setITLAInverted
     *     @param invert - true to request inverted output.
     */
    void
    VX2750Pha::setITLAInverted(bool inverted)
    {
        std::string value = inverted? "Inverted" : "Direct";
        SetDeviceValue("ITLAPolarity", value.c_str());
    }
    /**
     * setITLBInverted
     *    @param invert true to request inverted
     */
    void
    VX2750Pha::setITLBInverted(bool inverted)
    {
        std::string value = inverted? "Inverted" : "Direct";
        SetDeviceValue("ITLBPolarity", value.c_str());
    }
    /**
     * getITLConnect
     *    @param ch  - channel number.
     *    @return ITLConnect
     */
    VX2750Pha::ITLConnect
    VX2750Pha::getITLConnect(unsigned ch)
    {
        return stringToEnum(stringToITLAConnect, GetChannelValue(ch, "ITLConnect"));
    }
    /**
     * setITLConnect
     *    @param ch - channel.
     *    @param selection - the selected connection mode.
     */
    void
    VX2750Pha::setITLConnect(ch, ITLConnect selection)
    {
        std::string value = enumToString(ITLAConnectToSTring, selection);
        SetChannelValue(ch, "ITLConnect", value.c_str());
    }
    /**
     * getITLAMask
     *    get the mask of channels that are inputs to the ITLA logic.
     *    Use either the mask or ITLConnect but not both.
     *    @return uint64_t
     */
    std::uint64_t
    VX2750Pha::getITLAMask()
    {
        return GetDeviceULong("ITLAMask");
    }
    /**
     * getITLBMask
     *    same as above but for the B trigger logic block.
     */
    std::uint64_t
    VX2750Pha::getITLBMask()
    {
        return GetDeviceULong("ITLBMask");
    }
    /**
     * setITLAMask
     *    @param mask - new ITLA mask.
     */
    void
    VX2750Pha::setITLAMask(std::uint64_t mask\)
    {
        SetDeviceValue("ITLAMask", mask);
    }
    /**
     * setITLBMask
     *    same as above but for the b mask.
     */
    void
    VX2750Pha::setITLBMask(std::uint64_t mask)
    {
        SetDeviceValue("ITLBMask", mask);
    }
    /**
     * getITLAGateWidth
     *    @return std::uint32_t - width of the A gate in 8ns steps.
     */
    std::uint32_t
    VX2750Pha::getITLAGateWidth()
    {
        return GetDeviceInteger("ITLAGateWidth");
    }
    /**
     * getITLBGateWidth
     *    same as above but for the B gat width.
     */
    std::uint32_t
    VX2750Pha::getITLBGateWidth()
    {
        return GetDeviceInteger("ITLBGateWidth");
    }
    /**
     * setITLAGateWidth
     *    @param width - desired gatewidth in 8ns steps.
     */
    void
    VX2750Pha::setITLAGateWidth(std::uint32_t width)
    {
        SetDeviceValue("ITLAGateWidth", width);
    }
    /**
     * setITLBGateWidth
     *   same as above but for the b gate.
     */
    void
    VX2750Pha::setITLBGateWidth(std::uint32_t width)
    {
        SetDeviceValue("ITLBGateWidth", width);
    }
    /**
     * getLVDSMode
     *   @param quartet - which of the quartets.
     *   @return LVDSMode - the mode that quartet is set in.
     */
    VX2750Pha::LVDSMode
    VX2750Pha::getLVDSMode(unsigned quartet)
    {
        return stringToEnum(stringToLVDSMode, GetLVDSValue(quartet, "LVDSMode"));
    }
    /**
     * setLVDSMode
     *    @param quartet -the quartet to set.
     *    @param mode    - The LVDSMode desired for that quartet.
     */
    void
    VX2750Pha::setLVDSMode(unsigned quartet, LVDSMode mode)
    {
        std::string value = enumToSring(LVDSModeToString, mode);
        SetLVDSValue(quartet, "LVDSMode", value.c_str());
    }
    /**
     * getLVDSDirection
     * @param quartet - which group of 4 to interrogate.
     * @return LVDSDirection - Is the quartet composed of inputs or outputs.
     */
    VX2750Pha::getLVDSDirection(unsigned quartet)
    {
        return stringToEnum(stringToLVDSDirection, GetLVDSValue(quartet, "LVDSDirection"));
    }
    /**
     * setLVDSDirection
     *   @param quartet to set.
     *   @param direction - desired signal direction.n
     */
    void
    VX2750Pha::setLVDSDirection(unsigned quartet, LVDSDirection direction)
    {
        std::string value = enumToString(LVDSDirectionToSTring direction);
        SetLVDSValue(quartet, "LVDSDirection", value.c_str());
    }
    /**
     * getLVDSIOReg
     *    If an LVDS register is set to IORegister mode, this will
     *    return the value of the LVDS I/O states.  This will be correct if the
     *    register has been programmed for input or output.
     * @param quartet - the quartet to get the value of
     * @return std::uint16_t - Bit mask of the I/O register.
     */
    std::uint16_t
    VX2750Pha::getLVDSIOReg(unsigned quartet)
    {
        return GetLVDSInteger(quartet, "LVDSIOReg");
    }
    /**
     * setLVDSIOReg
     *    Sets the value of an LVDS I/O register.  This is only meaningful if
     *    the mode is IORegister and the direction in output.  In that case,
     *    this sets the values of the LVDS pins per the mask value passed in.
     * @param quartet - selects the quartet of LVDS pins to set.
     * @param mask    - Bit mask of LVDS outputs.
     */
    void
    VX2750Pha::setLVDSIOReg(unsigned quartet, std::uint16_t mask)
    {
        SetLVDSValue(quartet, "LVDSIOReg", mask);
    }
    /**
     * getLVDSTriggerMask
     *    Reads the self trigger mask that would be sent to the LVDS outputs
     *    if the mode is self trigger and the mode SelfTrigger
     * @param masknum - Mask number.
     * @return std::uint64_t
     */
    std::uint64_t
    VX2750Pha::getLVDSTriggerMask(unsigned masknum)
    {
        // This mask number bit is handled with pure insantiy -- see the manual
        // rather than encoding it into the path it's the initial value of the
        // buffer.
        
        std::stringstream strnum;
        strnum << masknum;
        std::string smaskno = strnum.c_str();
        
        std::string strResult =  GetValue("/par/LVDSTrgMask", maskno.c_str());
        std::uint64_t result;
        std::stringstream sResult(strResult);
        sResult >> result;
        return result;
    }
    /**
     * setLVDSTriggerMask
     *    @param masknum - mask number.
     *    @param mask - the trigger mask.
     */
    void
    VX2750Pha::setLVDSTriggerMask(unsigned masknum, std::uint64_t mask)
    {
        std::stringstream strValue;
        strValue << masknum << "=" << mask;
        std::string value = strValue.str();
        SetValue("/par/LVDSTrgMask", value.c_str());
    }
    /**
     * getDACOutMode
     *    @return DACOutMode - the signal assignment to the DACOut connector.
     */
    VX2750Pha::DACOutMode
    VX2750Pha::getDACOutMode()
    {
        return stringToEnum(stringToDACoutMode, GetDeviceValue("DACoutMode"));
    }
    /**
     * setDACOutMode
     *    Assign the function of the DACOut connector
     *
     *    @para mode - desired mode.
     */
    void
    VX2750Pha::setDACOutMode(DACOutMode mode)
    {
        std::string value = EnumToString(DACOutModeToString, mode);
        SetDeviceValue("DACoutMode", value.c_str());
    }
    /**
     * getDACOutValue
     *    If the output has been set to Static, this value is the value sent to
     *    the DAC for output on the DACOut connector
     * @return std::uint16_t
     */
    std::uint16_t
    VX2750Pha::getDACOutValue()
    {
        return GetDeviceInteger("DACoutStaticLevel");
    }
    /**
     * setDACOutValue
     *    If the output has been set to Static, this sends a value to the DAC
     *    and the converted output is sent to the DACOut connector.
     *   @param value - DAC value to set.
     */
    void
    VX2750Pha::setDACOutValue(std::uint16_t value)
    {
        SetDeviceValue("DACoutStaticLEvel", value);
    }
    /**
     * getDACChannel
     *   @return unsigned - If a channel is gated to the DAQout connector,
     *                     this fetches the channel number.\
     */
    unsigned
    VX2750Pha::getDACChannel()
    {
        return GetDeviceInteger("DACoutChSelect");
    }
    /**
     * setDACChannel
     *    @param chan - number of the channel to send to the DACout if the
     *                  channel mode is selected.
    */
    void
    VX2750Pha::setDACCHannel(unsigned chan)
    {
        SetDeviceChannel("DACoutChSelet", chan);
    }
    /**
     * getVGAGain
     *    Return the VGA gain for a set of 16 channels.
     *  @param group - the set to retrieve for.
     *  @return double - The gain in dB
     *  @note assume that the names are:  /vga/group/par/VGAGain.
     */
    double
    VX2750Pha::getVGAGain(unsigned group)
    {
        std::stringstream strParName;
        strParName << "/vga/" << group << "/par/VGAGain";
        std::string parname = strParNam.str();
        
        return GetReal(parname.str());
    }
    /**
     * setVGAGain
     *   Set the gain common to a gropu of 16 channels.
     * @param group - the group to set.
     * @param value - the value to set.
     * @note see above about contructing the path.
     */
    void
    VX2750Pha::setVGAGain(unsigned group, double value)
    {
        std::stringstream strParName;
        strParName << "/vga" << group << "/par/VGAGain";
        std::string parname = strParName.str();
        SetValue(parname.c_str(), value);
    }
    
    /**
     * isOffsetCalibrationEanbled
     *    @param chan - the cannel to check.
     *    @return bool - true if the selected channel's offset calibration
     *                    is enabled.
     */
    bool
    VX2750Pha::isOffstCalibrationEnabled(unsigned chan)
    {
        return GetChanBool(chan, "EnOffsetCalibration");
    }
    /**
     * enableOffsetCalibration
     *    @param chan - the channel.
     *    @param enable  - True to enable, false to disable.
     */
    void
    VX2750Pha::enableOffsetCalibration(unsigned chan, bool enable)
    {
        SetChanValue(chan, "EnOffsetCalibration", enable);
    }
    /**
     * isChannelEnabled
     *    @param chan - channel to check.
     *    @return bool - True if the channel is enable.
     */
    bool
    VX2750Pha::isChannelEnabled(unsigned chan)
    {
       return GetChanBool(chan, "ChEnable");
    }
    /**
     * enableChannel
     *   @param chan  - number of channel to enable/disable.
     *   @param enable - true to enable false to disable.
     */
    void
    VX2750Pha::enableChannel(unsigned chan, bool enable)
    {
        SetChanValue(chank, "ChEnable", enable);
    }
    /**
     * getSelfTriggerRate
     *   @param chan - channel to check.
     *   @return int  - Channel self trigger rate in Hz
     */
    int
    VX2750Pha::getSelfTriggerRate(unsigned chan)
    {
        return GetChanInteger(chan, "SelfTrgRate");
    }
    /**
     * getChannelStatus
     *    @param chan - the channel to query.
     *    @return std::uint32_t - a bitmask providing the current channel
     *                           status see the CHST_ definitions for symbolic
     *                           bit names.
     */
    std::uint32_t
    VX2750Pha::getChannelStatus(unsigned chan)
    {
        return GetChannelInteger(chan, "ChStatus");
    }
    /**
     * getDCOffset
     *    @param chan - the channel to fetch.
     *    @return double percentage DC offset.,
     */
    double
    VX2750Pha::getDCOffset(unsigned chan)
    {
        return GetChanReal(chan, "DCOffset");
    }
    /**
     * setDCOffset
     *   @param chan - channel to set.
     *   @param pct  - Percentage DC Offset to set.
     */
    void
    VX2750Pha::setDCOffset(unsigned chan, double pct)
    {
        SetChanValue(chan, "DCOffset", pct);
    }
    /**
     * getGainFactor
     *    @param chan - channel number.
     *    @return double - the channel gain value stored in internal flash.
     */
    doubld
    VX2750Pha::getGainFactor(unsigned chan)
    {
        return GetChanReal(chan, "GainFactor");
    
    }
    /**
     * getTriggerThreshold
     *   @param chan - channel to fetch.
     *   @return std::uint32_t - trigger threshold of that channel.
     */
    std::uint32_t
    VX2750Pha::getTriggerThreshold(unsigned chan)
    {
        return GetChanInteger(chan "TriggerThr");
    }
    /**
     * setTriggerThreshold
     *   @param chan - channel to set
     *   @param threshold - the new threshold value.
     */
    void
    VX2750Pha::setTriggerThreshold(unsigned chan, std::uint32_t threshold)
    {
        SetChanValue(chan, "TriggerThr", threshold);
    }
    /**
     * getPulsePolarity
     *   @param chan - channel to query.
     *   @return VX2750Pha::Polarity - the channel's polarity setting.
     */
    VX2750Pha::Polarity
    VX2750Pha::getPulsePolarity(unsigned chan)
    {
        return stringToEnum(stringToPolarity, GetChannelValue(chan, "PulsePolarity"));
    }
    /**
     * setPulsePolarity
     *   @param chan - channel to set.
     *   @param pol  - New channel polarity.
     */
    void
    VX2750Pha::setPulsePolarity(unsigned chan, Polarity pol)
    {
        std:::string value = enumToString(polarityToString, pol);
        SetChanValue(chan, "PulsePolarity", value.c_str());
    }
    /**
     * getEnergySkimLowDiscriminator
     *   @param chan - channel to fetch.
     *   @return std::uint16_t
     */
    std::uint16_t
    VX2750Pha::getEnergySkimLowDiscriminator(unsigned chan)
    {
        return GetChanInteger(chan, "EnergySkimLowDiscriminator");
    }
    /**
     * setEnergySkimLowDiscriminator
     *   @parm chan    - channel to set
     *   @param value  - New low threshold for energy selection.
     */
    void
    VX2750Pha::setEnergySkimLowDiscriminator(unsigned chan, std::uint16_t value)
    {
        SetChanValue(chan, "EnergySkimLowDiscriminator", value);
    }
    /**
     * getEnerygSkimHighDiscriminator
     *    @param chan -channel to get
     *    @return std::uint16_t - Energy high cutoff value.
     */
    std::uint16_t
    VX2750PHa::getEnergySkimHighDiscriminator(unsigned chan)
    {
        return GetChanInteger(chan, "EnergySkimHighDiscriminator");
    }
    /**
     * setEnergySkimHighDiscriminator
     *    @param chan -  channel to set.
     *    @param value - new high cutoff.
     */
    void
    VX2750Pha::setEnergySkimHighDiscriminator(unsigned chan, std::uint16_t value)
    {
        SeChanInteger(chan, "EnergySkimHighDiscriminator", value);
    }
    /**
     * getEventSelector
     *    @param chan - channel number.
     *    @return EventSelection - describes which events are actually saved by the channel.
     */
    VX2750Pha::EventSelection
    VX2750Pha::getEventSelector(unsigned chan)
    {
        return stringToEnum(stringToEventSelection, GetChanValue(chan, "EventSelector"));
    }
    /**
     * setEventSelector
     *    @param chan - channel number
     *    @param sel  - Selection of events to save.
     */
    void
    VX2750Pha::getEventSelector(unsigned chan, EventSelection sel)
    {
        std::string value = enumToString(eventSelectionToString, sel);
        SetChanValue(chan, "EventSelector", value.c_str());
    }
    /**
     * getWaveSelector
     *    @paran chan
     *    @return EventSelection - filtr on which waveforms are saved.
     */
    VX2750Pha::EventSelection
    VX2750Pha::getWaveSelector(unsigned chan)
    {
        return stringToEnum(stringToEventSelection, GetChanValue(cha, "WaveSelector"));
    }
    /**
     * setWaveSelector
     *    Select which waveform are recorded.
     *  @param chan - channel number
     *  @param sel  - Event selector.
     */
    void
    VX2750Pha::setWaveSelector(unsigned chan, EventSelection sel)
    {
        std::string value = enumToString(eventSelectionToString, sel);
        SetChanValue(chan, "WaveSelector", value.c_str());
    }
    /**
     * getCoincidenceMask
     *   @param chan - channel whose selection we'll fetch.
     */
    VX2750Pha::CoincidenceMask
    VX2750Pha::getCoincidenceMask(unsigned chan)
    {
        return stringToEnum(stringToCoincidenceMask, GetChanValue(chan, "CoincidenceMask"));
    }
    /**
     * setCoincidenceMask
     *    @param chan -  channel to set.
     *    @param sel  - Desired mask.
     */
    void
    VX2750Pha::setCoincidenceMask(unsigned chan, CoincidenceMask sel)
    {
        std::string value = enumToString(coincidenceMaskToString, sel);
        SetChanValue(chan, "CoincidenceMask", value.c_str());
    }
    /**
     * getAntiCoincidenceMask
     *    @param chan - channel to fetch from.
     *    @return CoincidenceMask - the anticoincidence triggr selection.
     */
    VX2750Pha::CoincidenceMask
    VX2750Pha::getAntiCoincidenceMask(unsigned chan)
    {
        return stringToEnum(stringToCoincidenceMask, GetChanValue(chan, "AntiCoincidenceMask"));
    }
    /**
     * setAntiCoincidenceMask
     *   @param chan - channel to fetch from
     *   @param sel  - Anticoincidence trigger selection.
     */
    void
    VX2750Pha::setAntiCoincidenceMask(unsigned chan, CoincidenceMask sel)
    {
        std::string value = enumToString(coincidenceMaskToString, sel);
        SetChanValue(chan, "AntiCoincidenceMask", value.c_str());
    }
    /**
     * getCoincidenceSamples
     *    @param chan - channel to fetch from.
     *    @return std::uint32_t - number of samples in the coincidence trigger window.
     */
    std::uint32_t
    VX2750Pha::getCoincidenceSamples(unsigned chan)
    {
        return GetChanInteger(chan, "CoincidenceLengthS");
    }
    /**
     * setCoincidenceSamples
     *    @param chan - channel to set.
     *    @param samples -# samples to be used for that channel's coincidence window.
     */
    void
    VX2750Pha::setCoincidenceSamples(unsigned chan, std::uint32_t samples)
    {
        SetChanValue(chan, "CoincidenceLengthS", samples);
    }
    /**
     * setCoincidenceNs
     *    @param chan - channel to get.
     *    @return std::uint32_t - same as above but the result is in ns units./
     */
    std::uint32_t
    VX2750Pha::getCoincidenceNs(unsigned chan)
    {
        return GetChanInteger(chan, "CoincidenceLengthT");
    }
    /**
     * setCoincidenceNs
     *   @param chan - channel number.
     *   @para ns    - COincidence window in nanoseconds.
     */
    void
    VX2750Pha::setCoincidenceNs(unsigned chan, std::uint32_t ns)
    {
        SetChanValue(chan, "ConcidenceLengthT", ns);
    }
    /**
     * getTimeFilterRiseTime
     *   @param chan - channel to get
     *   @return std::unit32_t ns of rise time set in the time filter.
     */
    std::uint32_t
    VX2750Pha::getTimeFilterRiseTime(unsigned chan)
    {
        return GetChanInteger(chan, "TimeFilterRiseTimeT");
    }
    /**
     * getTimeFilterRiseSamples
     *    @param chan - channel to get.
     *    @return std::uint32_t - samples in the time filte rise time.
     */
    std::uint32_t
    VX2750Pha::getTimeFilterRiseSamples(unsigned chan)
    {
        return GetChanInteger(chan, "TimeFilterRiseTimeS");
    }
    /**
     * setTimeFilterRiseTime
     *    Set the time filter rise-time in ns
     *  @param chan - channel number to set.
     *  @param ns   - rise time requested.
     */
    void
    VX2750Pha::setTimeFilterRiseTime(unsigned chan, std::uint32_t ns)
    {
        SetChanValue(chan, "TimeFilterRiseTimeT", ns);
    }
    /**
     * setTimeFilterRiseSamples
     *   @param chan - channel number to set.
     *   @param samples - samples in the time filter rise-time to set.
     */
    void
    VX2750Pha::setTimeFilterRiseSamples(unsigned chan, std::uint32_t samples)
    {
        SetChanValue(chan, "TimeFilterRiseTimeS", samples);
    }
    /**
     * getTimeFilterRetriggerGaurdTime
     *   @param chan
     *   @return std::uint32_t - ns in the time filter retrigger guard.
     */
    std::uint32_t
    VX2750Pha::getTimeFilterRetriggerGuardTime(unsigned chan)
    {
        return GetChanInteger(chan, "TimeFilterRetriggerGuardT");
    }
    /**
     * getTimeFilterRetriggerGuardSamples
     *   @param chan
     *   @return std::uint32_t -same as above but the units are digitizer samples.
     */
    std::uint32_t
    VX2750Pha::getTimeFilterRetriggerGuardSamples(unsigned chan)
    {
        return GetChanInteger(chan, "TimeFilterRetriggerGaurdS");
    }
    /**
     * setTimeFilterRetriggerGuardTime
     *    @param chan
     *    @param ns  - ns to set the retrigger guard time to for chan.
     */
    void
    VX2750Pha::setTimeFilterRetriggerGuardTime(unsigned chan, std::uint32_t ns)
    {
        SetChanValue(chan, "TimeFilterRetriggerGuardT", ns);
    }
    /**
     * setTimeFilterRetriggerGuardSamples
     *    Same as above except the guard window is expressed in samples
     */
    void
    VX2750Pha::setTimeFilterRetriggerGuardSamples(Unsigned chan, std::uint32_t samples)
    {
        SetChanValue(chan, "TimeFilterRetriggerGuardS", samples);
    }
    /**
     * getEnergyFilterRiseTime
     *    @param chan - channel to query.
     *    @return std::uint32_t - energy filter rise time expressed in ns.
     */
    std::uint32_t
    VX2750Pha::getEnergyFilterRiseTime(unsigned chan)
    {
        return GetChanInteger(chan, "EnergyFilterRiseTimeT");
    }
    /**
     * getEnergyFilterRiseSamples
     *    Same as above but the result is expressed in digitizer samples.
     */
    std::uint32_t
    VX2750Pha::getEnergyFilterRiseSamples(unsigned chan)
    {
        return GetChanInteger(chan, "EnergyFilterRiseTimeS");
    }
    /**
     * setEnergyFilterRisetime
     *    @param chan - channel to set.
     *    @param ns   - Nanoseconds to set the energy filter rise time.
     */
    void
    VX2750Pha::setEnergyFilterRiseTime(unsigned chan, std::uint32_t ns)
    {
        SetChanInteger(chan, "EnergyFilterRiseTimeT", ns);
    }
    /**
     * setEnergyFilterRiseSamples
     *    Same as above but units are samples.
     */
    void
    VX2750Pha::setEnergyFilterRiseSamples(unsigned chan, std::uint32_t samples)
    {
        SetChanInteger(chan, "EnergyFilterRiseTimeS", samples);
    }
    
}                                // namespace
