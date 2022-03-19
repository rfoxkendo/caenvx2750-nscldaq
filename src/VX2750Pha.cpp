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
    {"Internal", VX2750::Internal},
    {"FPClkIn", VX2750::FrontPanel},
    {"P0ClkIn",   VX2750::P0},
    {"Link",   VX2750::Link},
    {"DIPswitchSel", VX2750::DIPSelected}
};
static const std::map<VX2750Pha::ClockSource, std::string> clockSourceToString = {
    {VX2750::Internal, "Internal"},
    {VX2750::FrontPanel, "FPClkIn"},
    {VX2750::P0, "P0ClkIn"},
    {VX2750::Link, "Link"},
    {VX2750::DIPSelected, "DIPswitchSel"}
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
    VX2750::FWType
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
    VX2750::WaveTriggerSource
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
    VX2750::TRGOUTMode
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
     *     @return VX2750::SyncOutMode - the current selection of the CLKOUT connector
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
    VX2750::VetoSource
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
    
}                                // namespace