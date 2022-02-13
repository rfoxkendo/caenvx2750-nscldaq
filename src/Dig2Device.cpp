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
     *  @todo - Can refactor in terms of a template function!.
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
    void
    Dig2Device::SetValue(const char* parameterName, double value)
    {
        std::stringstream vstream;
        vstream << value;
        std::string strValue = vstream.str();
        SetValue(parameterName, strValue.c_str());
    }
    void
    Dig2Device::SetValue(const char* parameterName, bool value)
    {
        std::stringstream vstream;
        vstream << value;
        std::string strValue = vstream.str();
        SetValue(parameterName, strValue.c_str());
    }
    /**
     * SetDeviceValue
     *     Device values have names like /par/ParameterName.
     *     This overloaded set of functions takes the terminal part of
     *     the parameter name and a value, creates the full path and
     *     sets the value.
     * @param devParName - a device parameter name.
     * @param value      - Value to set.
     * @todo - can think about this being a templated function.
     */
    void
    Dig2Device::SetDeviceValue(const char* devParName, const char* value)
    {
        std::string devPath = devPath(devParName);
        SetValue(devPath.c_str(), value);
    }
    Dig2Device::SetDeviceValue(const char* devParName, int* value)
    {
        std::string devPath = devPath(devParName);
        SetValue(devPath.c_str(), value);
    }
    Dig2Device::SetDeviceValue(const char* devParName, doubld value)
    {
        std::string devPath = devPath(devParName);
        SetValue(devPath.c_str(), value);
    }
    Dig2Device::SetDeviceValue(const char* devParName, bool value)
    {
        std::string devPath = devPath(devParName);
        SetValue(devPath.c_str(), value);
    }
    /**
     * SetChanValue
     *    Per channel values have paths of the form:
     *       /ch/n/par/ParameterName where n is a channel number.
     *    These overloaded functions construct the path and then uses SetValue
     *    to set a per channel parameter given the terminal part of its path.
     * @param chan  Channel number.
     * @param parameterName - name of the parameter.
     * @param value  Channel value.
     * @todo - there is an opportunity for factoring into a templated
     *         method
     */
    
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, const char* value)
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, int value)
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, double value)
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, bool value)
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    /**
     * getValueGet<type>Value
     *    Gets the value of a parameter.
     *   @todo using a templated function could get around the problem that
     *         overloadsd cannot vary solely in return type.
     *  @param parameterName - Full path to the parameter.
     *  @return value
     */
    std::string
    Dig2Device::GetValue(const char* parameterName)
    {
        char buffer[256];           // Max value according to lib docs.
        if (CAEN_FELib_GetValue(m_deviceHandle, parameterName, buffer) != CAEN_FELib_Success) {
            std::stringstream strMessage;
            strMessage << "GetValue failed for " << parameterName <<
                <<" : "    << lastError;
            std::string msg = strMessage.str();
            throw std::runtime_error(msg.c_str());
        }
        return std::string(buffer);
    }
    int Dig2Device::GetInteger(const char* parameterName)
    {
        int value;
        std::string strValue = GetValue(parameterName);
        std::stringstream sValue(strValue);
        sValue >> value;
        return value;
    }
    int Dig2Device::GetReal(const char* parameterName)
    {
        double value;
        std::string strValue = GetValue(parameterName);
        std::stringstream sValue(strValue);
        sValue >> value;
        return value;
    }
    int Dig2Device::GetBool(const char* parameterName)
    {
        bool value;
        std::string strValue = GetValue(parameterName);
        std::stringstream sValue(strValue);
        sValue >> value;
        return value;
    }
    /**
     * GetDeviceValue/GetDevicexxx
     *     Get a device parameter value.  These have paths like
     *     /par/parametername.
     * @param parameterName - the terminal node of the parameter path.
     * @return parameter value.
     * @todo - again there's a refactoring opportunity here in terms of a
     *         template method.
     */
    std::string
    Dig2Device::GetDeviceValue(const char* parameterName)
    {
        std::string fullPath = devPath(parameterName)
        return GetValue(fullPath.c_str());        
    }
    int
    Dig2Device::GetDeviceInteger(const char* parameterName)
    {
        std::string fullPath = devPath(parameterName);
        return GetInteger(fullPath.c_str());
    }
    double
    Dig2Device::GetDeviceReal(const char* parameterName)
    {
        std::string fullPath = devPath(parameterName);
        return GetReal(fullPath.c_str());
    }
    bool
    Dig2Device::GetDeviceBool(const char* parameterName)
    {
        std::string fullPath = devPath(parameterName);
        return GetBool(fullPath.c_str());
    }
    /**
     * GetChanValue/GetChanxxx
     *    Channel parameters have paths like /ch/n/par/name
     *    where n is the channel number.
     * @param chan          - CHannel number.
     * @param parameterName - Terminnal part of the parameter name path.
     * @return Value of that channel's parameter.
     * 
     */
    std::string
    Dig2Device::GetChanValue(unsigned chan, const char* parameterName)
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetValue(fullPath.c_str());
    }
    int
    Dig2Device::GetChanInteger(unsigned chan, const char* parameterName)
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetInteger(fullPath.c_str());
    }
    doubld
    Dig2Device::GetChanReal(unsigned chan, const char* parameterName)
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetReal(fullPath.c_str());
    }
    bool
    Dig2Device::GetChanBool(unsigned chan, const char* parameterName)
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetBool(fullPath.c_str());
    }
    /**
     * Command
     *    Send a command to the digitizer.
     * @param command - this is the terminal node of the command path string.
     */
    void
    Dig2Device::Command(const char* command)
    {
        std::string fullPath = "cmd/";
        fullPath += command;
        
        if (CAEN_FELib_SendCommand(m_deviceHandle, fullPath.c_str()) != CAEN_FELib_Success)
        {
            std::stringstream strMessage;
            strMessage << " Failed to send digitizer " << commmand
                       << " operation : " << lastError();
            std::string msg = strMessage.str();
            throw std::runtime_error(msg);
        }
    }
    /**
     * setActiveEndpoint
     *    Construct the end point path name and set it as active.
     *    
     *    @param ep = name of the endpoint
     */
    void
    Dig2Device::SetActiveEndpoint(const char* ep)
    {
        SetValue("/endpoint/par/activeendpoint", ep);
    }
}