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

// Enumerator mappings:


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
    
}