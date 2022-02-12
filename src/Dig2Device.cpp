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
* @file     Dig2Device.cpp
* @brief    Implementation of the Dig2Device classs.
* @author   Ron Fox
*
*/
#include "Dig2Device.h"
#include <CAEN_FELib.h>
#include <stdexcept>
#include <sstream>

static const char* scheme="dig2";
static const char* usbhost="usb:";

namespace caen_nscldaq {
    /**
     * constructor
     *    Fills in the m_deviceHandle member from the successful result of
     *    CAEN_FELib_Open with us doing the dirty work of encoding the URI.
     *
     *    The URI uses the host for ethernet connections and usb:PID for
     *    usb connections.
     *
     * @param hostOrPid - host for ethernet of PID for USB connections.
     * @param isusb     - True if connection is via usb.
     * @throw std::runtime_error if the open failed. THe string will
     *        include textual error information from FELib.
     */
    Dig2Device::Dig2Device(const char* hostOrPid, bool isusb) {
        std::stringstream uristream;
        uristream << scheme << "://";
        if (isusb) {
            uristream << "usbhost";
        }
        uristream << hostOrPid;
        
        std::string uri = uristream.str();
        auto status = CAEN_FELib_Open(uri.c_str(), &m_deviceHandle);
        if (status != CAEN_FELib_Success) {
            std::string msg("Failed to open device: ");
            msg += uri;
            msg += " ";
            msg += lastError();
            throw std::runtime_error(msg);
        }
    }
    /**
     * destructor
     *   C++ destructors cannot toss exceptions so we must assume the close
     *   worked:
     */
    Dig2Device::~Dig2Device()
    {
        CAEN_FELib_Close(m_deviceHandle);
    }
    ////////////////////////////////////////////////////////////////////////////
    // Make device settings:
    
    /** SetValue
     *     These overloads work on the full parameter path e.g.
     *     /par/digitizer-paramname.
     *  @param parameterName - full path to parameter.  See as well
     *                         SetDeviceValue as well as SetChan Value
     *  @param value         - Value to set.
     */
    void
    Dig2Device::SetValue(const char* parameterName, const char* value)
    {
        if (CAEN_FELib_SetValue(m_deviceHandke, parameterName, value) != CAEN_FELib_Success) {
            std::stringstream failmsg;
            failmsg << " Failed to set value: " << parameterName
                << " to : " <<  value << " : " << lastError();
            std::string msg = failmsg.str();
            throw std::runtime_error(msg);
        }
    }
    void
    Dig2Device::SetValue(const char* parameterName, int value)
    {
        std::stringstream vstream;
        vstream << value;
        std::string strValue = vstream.str();
        SetValue(parameterName, strValue.c_str());
    }
}