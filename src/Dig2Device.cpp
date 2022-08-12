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
#include <assert.h>
#include <string.h>

static const char* scheme="dig2";

static const char* usbhost="caendgtz-usb-";

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
            uristream << usbhost;
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
    Dig2Device::SetValue(const char* parameterName, const char* value) const
    {
        if (CAEN_FELib_SetValue(m_deviceHandle, parameterName, value) != CAEN_FELib_Success) {
            std::stringstream failmsg;
            failmsg << " Failed to set value: " << parameterName
                << " to : " <<  value << " : " << lastError();
            std::string msg = failmsg.str();
            throw std::runtime_error(msg);
        }
    }
    void
    Dig2Device::SetValue(const char* parameterName, int value) const
    {
        std::stringstream vstream;
        vstream << value;
        std::string strValue = vstream.str();
        SetValue(parameterName, strValue.c_str());
    }
    void
    Dig2Device::SetValue(const char* parameterName, std::uint64_t value) const
    {
        std::stringstream vstream;
        vstream << value;
        std::string strValue = vstream.str();
        SetValue(parameterName, strValue.c_str());
    }
    void
    Dig2Device::SetValue(const char* parameterName, double value) const
    {
        std::stringstream vstream;
        vstream << value;
        std::string strValue = vstream.str();
        SetValue(parameterName, strValue.c_str());
    }
    void
    Dig2Device::SetValue(const char* parameterName, bool value) const
    {
        
        std::string strValue = value ? "True" : "False";
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
    Dig2Device::SetDeviceValue(const char* devParName, const char* value) const
    {
        std::string fullDevPath = devPath(devParName);
        SetValue(fullDevPath.c_str(), value);
    }
    void
    Dig2Device::SetDeviceValue(const char* devParName, int value) const
    {
        std::string fullDevPath = devPath(devParName);
        SetValue(fullDevPath.c_str(), value);
    }
    void
    Dig2Device::SetDeviceValue(const char* devParName, std::uint64_t value) const
    {
        std::string fullDevPath = devPath(devParName);
        SetValue(fullDevPath.c_str(), value);
    }
    void
    Dig2Device::SetDeviceValue(const char* devParName, double value) const
    {
        std::string fullDevPath = devPath(devParName);
        SetValue(fullDevPath.c_str(), value);
    }
    void
    Dig2Device::SetDeviceValue(const char* devParName, bool value) const
    {
        std::string fullDevPath = devPath(devParName);
        SetValue(fullDevPath.c_str(), value);
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
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, const char* value) const
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, int value) const
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned chan, const char* chanParName, std::uint64_t value) const
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, double value) const
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    void
    Dig2Device::SetChanValue(unsigned  chan, const char* chanParName, bool value) const
    {
        std::string parPath = chanPath(chan, chanParName);
        SetValue(parPath.c_str(), value);
    }
    /**
     * SetLVDSValue
     *   @param quartet -which of the LVDS groups
     *   @param LVDSName - name of the parameter,
     *   @param value  (overloaded) value to set.
     */
    void
    Dig2Device::SetLVDSValue(unsigned quartet, const char* LVDSName, const char* value) const
    {
        std::string path = LVDSPath(quartet, LVDSName);
        SetValue(path.c_str(), value);
    }
    void
    Dig2Device::SetLVDSValue(unsigned quartet, const char* LVDSName, int value) const
    {
        std::string path = LVDSPath(quartet, LVDSName);
        SetValue(path.c_str(), value);
    }
    void
    Dig2Device::setLVDSValue(unsigned quartet, const char* LVDSName, std::uint64_t value) const
    {
        std::string path = LVDSPath(quartet, LVDSName);
        SetValue(path.c_str(), value);
    }
    void
    Dig2Device::SetLVDSValue(unsigned quartet, const char* LVDSName, double value) const
    {
        std::string path = LVDSPath(quartet, LVDSName);
        SetValue(path.c_str(), value);
    }
    void
    Dig2Device::SetLVDSValue(unsigned quartet, const char* LVDSName, bool value) const
    {
        std::string path = LVDSPath(quartet, LVDSName);
        SetValue(path.c_str(), value);
    }
    /**
     * getValueGet<type>Value
     *    Gets the value of a parameter.
     *   @todo using a templated function could get around the problem that
     *         overloadsd cannot vary solely in return type.
     *  @param parameterName - Full path to the parameter.
     *  @param initial       - for LVDS trigger mask insanity - if specified, the
     *                         initial value put int he buffer.
     *  @return value
     */
    std::string
    Dig2Device::GetValue(const char* parameterName, const char* initial) const
    {
        char buffer[256];           // Max value according to lib docs.
        if (initial) {
            strcpy(buffer, initial);
        }
        if (CAEN_FELib_GetValue(m_deviceHandle, parameterName, buffer) != CAEN_FELib_Success) {
            std::stringstream strMessage;
            strMessage << "GetValue failed for " << parameterName 
                << " : "    << lastError();
            std::string msg = strMessage.str();
            throw std::runtime_error(msg.c_str());
        }
        return std::string(buffer);
    }
    int Dig2Device::GetInteger(const char* parameterName) const
    {
        int value;
        std::string strValue = GetValue(parameterName);
        std::stringstream sValue(strValue);
        sValue >> value;
        return value;
    }
    std::uint64_t Dig2Device::GetULong(const char* parameterName) const
    {
        std::uint64_t value;
        std::string strValue = GetValue(parameterName);
        std::stringstream sValue(strValue);
        sValue >> value;
        return value;
    }
    double Dig2Device::GetReal(const char* parameterName) const
    {
        double value;
        std::string strValue = GetValue(parameterName);
        std::stringstream sValue(strValue);
        sValue >> value;
        return value;
    }
    bool Dig2Device::GetBool(const char* parameterName) const
    {
        bool value;
        std::string strValue = GetValue(parameterName);
        value = strValue == "True" ? true : false;
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
    Dig2Device::GetDeviceValue(const char* parameterName) const
    {
        std::string fullPath = devPath(parameterName);
        return GetValue(fullPath.c_str());        
    }
    int
    Dig2Device::GetDeviceInteger(const char* parameterName) const
    {
        std::string fullPath = devPath(parameterName);
        return GetInteger(fullPath.c_str());
    }
    std::uint64_t
    Dig2Device::GetDeviceULong(const char* parameterName) const
    {
        std::string fullPath = devPath(parameterName);
        return GetULong(fullPath.c_str());
    }
    double
    Dig2Device::GetDeviceReal(const char* parameterName) const
    {
        std::string fullPath = devPath(parameterName);
        return GetReal(fullPath.c_str());
    }
    bool
    Dig2Device::GetDeviceBool(const char* parameterName) const
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
    Dig2Device::GetChanValue(unsigned chan, const char* parameterName) const
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetValue(fullPath.c_str());
    }
    int
    Dig2Device::GetChanInteger(unsigned chan, const char* parameterName) const
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetInteger(fullPath.c_str());
    }
    std::uint64_t
    Dig2Device::GetChanULong(unsigned chan, const char* parameterName) const
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetULong(fullPath.c_str());    
    }
    double
    Dig2Device::GetChanReal(unsigned chan, const char* parameterName) const
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetReal(fullPath.c_str());
    }
    bool
    Dig2Device::GetChanBool(unsigned chan, const char* parameterName) const
    {
        std::string fullPath = chanPath(chan, parameterName);
        return GetBool(fullPath.c_str());
    }
    /**
     * getLVDSxxx
     *   Get the value of an LVDS parameter.
     * @param quartet - which of the LVDS quartets to query.
     * @param parameterName - the name of the parameter in LVDS space.
     * @return std::string the value read.
     */
    std::string
    Dig2Device::GetLVDSValue(unsigned quartet, const char* parameterName) const
    {
        std::string fullPath = LVDSPath(quartet, parameterName);
        return GetValue(fullPath.c_str());
    }
    int
    Dig2Device::GetLVDSInteger(unsigned quartet, const char* parameterName) const
    {
        std::string fullPath = LVDSPath(quartet, parameterName);
        return GetInteger(fullPath.c_str());
    }
    std::uint64_t
    Dig2Device::GetLVDSULong(unsigned quartet, const char* parameterName) const
    {
        std::string fullPath = LVDSPath(quartet, parameterName);
        return GetULong(fullPath.c_str());
    }
    double
    Dig2Device::GetLVDSReal(unsigned quartet, const char* parameterName) const
    {
        std::string fullPath = LVDSPath(quartet, parameterName);
        return GetReal(fullPath.c_str());
    }
    bool
    Dig2Device::GetLVDSBool(unsigned quartet, const char* parameterName) const
    {
        std::string fullPath = LVDSPath(quartet, parameterName);
        return GetBool(fullPath.c_str());
    }
    /**
     * Command
     *    Send a command to the digitizer.
     * @param command - this is the terminal node of the command path string.
     */
    void
    Dig2Device::Command(const char* command) const
    {
        std::string fullPath = "cmd/";
        fullPath += command;
        
        if (CAEN_FELib_SendCommand(m_deviceHandle, fullPath.c_str()) != CAEN_FELib_Success)
        {
            std::stringstream strMessage;
            strMessage << " Failed to send digitizer " << command
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
    Dig2Device::SetActiveEndpoint(const char* ep) const
    {
        SetValue("/endpoint/par/activeendpoint", ep);
    }
    /**
     * gGtActiveEndpoint
     *   Return the string name of the selected endpoint :
     */
    std::string
    Dig2Device::GetActiveEndpoint() const
    {
        return GetValue("/endpoint/par/activeendpoint");
    }
    /**
     * setReadDataFormat
     *   The actual stuff returned from a read is determined by this
     *   function.  We pass some JSON stuff to it to describe what we want
     *   to get out of the digitizer and then ReadData must pass an appropriately
     *   crafted parameter list. See ReadData.
     * @param json   - The JSON that descsribes what we want from the device.
     */
    void
    Dig2Device::SetReadDataFormat(const char* json) const
    {
        if (CAEN_FELib_SetReadDataFormat(m_deviceHandle, json) != CAEN_FELib_Success) {
            std::stringstream strMessage;
            strMessage << "Failed to set the data fromat "
            << lastError();
            std::string msg = strMessage.str();
            throw std::runtime_error(msg);
        }
    }
    /**
     * ReadData
     *    This method is a bit of nonsense.  The CAEN_FELib read methods
     *    use variable argument lists.  The C/C++ standards don't provide
     *    a recognized working method to jacket a variadic function with another
     *    variadic function.  Yes there are bits and pieces on forums but
     *    none of them are standard compliant in the sense that they are
     *    guaranteed to work as compilers change.
     *
     *    In CAEN Support ticket #TT154304 I asked for a version of ReadData
     *    that allowed an argc, argv parameter list and that was turned down
     *    with the suggestion that I do the kludge in this method.
     *
     *    If somoeone ever actually decides to validate the argument count
     *    this will blow up spectacularly.  The claim in that ticket is that:
     *    "Additional arguments not defined in the
     *    CAEN_FELib_SetReadDataFormat() will be ignored by the
     *    CAEN_FELib_ReadData()."
     *
     *    Note this ticket also requests a check for data function which
     *    the software folks said they will implement.
     *
     *    Note that this kludge also assumes that scaler types (not pointed to)
     *    can fit in a void*  This also not assured.  We can't do a compile
     *    time check for this, alas.  We can do a run-time check and, if this
     *    all fails, we can document that uint64_t must be received via pointers.
     *
     *    @param timeout - # ms timeout.
     *    @param argc    - Number of arguments.
     *    @param argv    - Arguments themselves.
     *    @return bool   - true if data were read, false if timeout.
     */
    bool
    Dig2Device::ReadData(int timeout, int argc, void** argv) const
    {
        // No point in making a constant size since we'll have to list the
        // elements explicitly in the call to ReadData.
        void* args[30];                // hopefully enough.
        assert(sizeof(std::uint64_t) <= sizeof(void*));
        if (argc > (sizeof(args)/sizeof(void*))) {
            throw std::out_of_range("argc in Dig2Device:: too big maximum 20");
        }
        memcpy(args, argv, argc*sizeof(void*));
        auto status = CAEN_FELib_ReadData(m_deviceHandle, timeout,
            args[0], args[1], args[2], args[3], args[4],
            args[5], args[6], args[7], args[8], args[9],
            args[10], args[11], args[12], args[13], args[14],
            args[15], args[16], args[17], args[18], args[19],
            args[20], args[21], args[22], args[23], args[24],
            args[25], args[26], args[27], args[28], args[29]
        );
        if (status == CAEN_FELib_Timeout) return false;
        if(status != CAEN_FELib_Success) {
            std::stringstream strMessage;
            strMessage << "ReadData failed: " << lastError();
            std::string msg = strMessage.str();
            throw std::runtime_error(msg);
        }
        return true;
    }
    /**
     * hasData
     * @return bool - true if the device has data that can be read:
     */
    bool
    Dig2Device::hasData() const
    {
        return CAEN_FELib_HasData(m_deviceHandle, 0);
    }
    ///////////////////////////////////////////////////////////////
    // Utlity methods.
    
    /**
     * devPath
     *    Given a device parameter name returns the full path to the
     *    parameter.
     * @param devParName - device parameter name.
     * @return std::string - the full path.
     */
    std::string
    Dig2Device::devPath(const char* devParName) const
    {
        std::string result = "/par/";
        result += devParName;
        return result;
    }
    /**
     * chanPath
     *    Given a channel number and a channel parameter name
     *    returns the full path to that parameter.
     * @param chan - Channel number.
     * @param chanParName - name of a parameter for that channel.
     * @return std::string Full path.
     */
    std::string
    Dig2Device::chanPath(unsigned chan, const char* chanParName) const
    {
        std::stringstream strPath;
        strPath << "/ch/" << chan << "/par/" << chanParName;
        
        std::string result = strPath.str();
        return result;
    }
    /**
     * LVDSPath
     *    Return the path to an LVDS Parameter
     *
     * @param quartet - the lVDS quartet selected.
     *  @param LVDS parameter name.
     *  @return std::string
     */
    std::string
    Dig2Device::LVDSPath(unsigned quartet, const char* lvdsParName) const
    {
        std::stringstream strPath;
        strPath << "/lvds" << quartet << "/par/" << lvdsParName;
        std::string result = strPath.str();
        return result;
    }
    /**
     * lastError
     *    Wrapper for CAEN_FELib_GetLastError.
     * @return std::string
     */
    std::string
    Dig2Device::lastError() const
    {
        char msg[1024];                    // According to the API.
        if(CAEN_FELib_GetLastError(msg) != CAEN_FELib_Success) {
            // not sure what could go wrong but:
            
            throw std::runtime_error("Failed to get last error message!");
        }
        std::string result(msg);
        return result;
    }
    
}
