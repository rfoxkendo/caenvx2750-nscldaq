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
    V2750Pha::getModelCode()
    {
        return getDeviceValue("ModelCode");
    }
    /**
     * getPiggyBackCode
     *    @return std::string - the piggy back card type code.
     */
    std::sring
    V2750Pha::getPiggyBackCode()
    {
        return getDeviceValue("PBCode");
    }
    /**
     * getModelName
     *    @return std::string - module model code.
     *    
     */
    std::string
    V2750Pha::getModelName()
    {
        return getDeviceValue("ModelName");
    }
    /**
     * getFormFactor
     * 
     * @return FormFactor - The form factor of the module.
     */
    V2750Pha::FormFactor
    V2750Pha::getFormFactor()
    {
        return stringToEnum(stringToFormFactor, getDeviceValue("FormFactor"));
    }
    /**
     * getFamilyCode
     *   @return int - family code number.
     */
    int
    V2750Pha::getFamilyCode()
    {
        return getDeviceInteger("FamilyCode");
    }
    /**
     * getSerialNumber
     *    @return std::string - serial string.
     */
    std::string
    V2750Pha::getSerialNumber()
    {
        return getDeviceValue("SerialNum");
    }
}