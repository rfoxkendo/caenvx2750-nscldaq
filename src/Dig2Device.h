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
* @file     Dig2Device.h
* @brief    Class to encapsulate the Dig2 device interface.
* @author   Ron Fox
*
*/
#ifndef DIG2DEVICE_H
#define DIG2DEVICE_H
#include <cstdint>
#include <string>
#include <cstdarg>

namespace caen_nscldaq {
    /**
     * @class Dig2Device
     *       Encapsulates the dig2 library in a bit simpler-to-use
     *       mechanism.
     * @note at present I don't see the advantage to monitor connections
     *       so they are not supported -- though I imagine support could be added.
     */
    class Dig2Device {
    private:
        std::uint64_t m_deviceHandle;
    public:
        Dig2Device(const char* hostOrPid, bool isUsb = false);
        virtual ~Dig2Device();
        
        // Use overloads to make the user's life simpler:
        void SetValue(const char* parameterName, const char* value);
        void SetValue(const char* parameterName, int  value);
        void SetValue(const char* parameterName, std::uint64_t value);
        void SetValue(const char* parameterName, double value);
        void SetValue(const char* parameterName, bool value);
        
        void SetDeviceValue(const char* devParName, const char* value);
        void SetDeviceValue(const char* devParName, int value);
        void SetDeviceValue(const char* devParName, std::uint64_t value);
        void SetDeviceValue(const char* devParName, double value);
        void SetDeviceValue(const char* devParName, bool value);
        
        void SetChanValue(unsigned chan, const char* chanParName, const char* value);
        void SetChanValue(unsigned chan, const char* chanParName, int value);
        void setChanValue(unsigned chan, const char* chanParName, std::uint64_t value);
        void SetChanValue(unsigned chan, const char* chanParName, double value);
        void SetChanValue(unsigned chan, const char* chanParName, bool value);
        ch
        void SetLVDSValue(unsigned quartet, const char* LVDSName, const char* value);
        void SetLVDSValue(unsigned quartet, const char* LVDSName, int value);
        void setLVDSValue(unsigned quartet, const char* LVDSName, std::uint64_t value);
        void SetLVDSValue(unsigned quartet, const char* LVDSName, double value);
        void SetLVDSValue(unsigned quartet, const char* LVDSName, bool value);
        
        
        
        // Sadly we can't do the overload trick for getValue since
        // C++ does not overload solely on return value type so:
        
        std::string GetValue(const char* parameterName, const char* initial = nullptr);
        int         GetInteger(const char* parameterName);
        std::uint64_t GetULong(const char* parameterName);
        double      GetReal(const char* parameterName);
        bool        GetBool(const char* parameterName);
        
        std::string GetDeviceValue(const char* parameterName);
        int         GetDeviceInteger(const char* parameterName);
        std::uint64_t GetDeviceULong(const char* parameterName);
        double      GetDeviceReal(const char* parameterName);
        bool        GetDeviceBool(const char* parameterName);
        
        std::string GetChanValue(unsigned chan, const char* parameterName);
        int         GetChanInteger(unsigned chan, const char* parameterName);
        std::uint64_t GetChanULong(unsigned chan, const char* parameterName);
        double      GetChanReal(unsigned chan, const char* parameterName);
        bool        GetChanBool(unsigned chan, const char* parameterName);
        
        std::string GetLVDSValue(unsigned quartet, const char* parameterName);
        int         GetLVDSInteger(unsigned quartet, const char* parameterName);
        std::uint64_t GetLVDSULong(unsigned quartet, const char* parameterName);
        double      GetLVDSReal(unsigned quartet, const char* parameterName);
        bool        GetLVDSBool(unsigned quartet, const char* parameterName);
        
        // Digitizer commands:
        
        void Command(const char* command);
        
        // Acquistion -- these need a higher level of abstraction
        // than offered by felib IMHO.
        
        void SetActiveEndpoint(const char* ep);
        std::string GetActiveEndpoint();
        void SetReadDataFormat(const char* json);   // Can we hide JSON generation?
        
        bool ReadData(int timeout, int argc, void** argv);
        bool hasData();                             // True if a device has data.
    private:
        std::string devPath(const char* devParName);
        std::string chanPath(unsigned chan, const char* chanParName);
        std::string LDVDSPath(unsigned quartet, const char* lvdsParName);
        std::string lastError();
    };
}

#endif