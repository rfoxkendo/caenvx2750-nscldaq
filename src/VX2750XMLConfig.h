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
* @file     VX2750XMLConfig.h
* @brief    Build configuration from COMPASS XML configuration file.
* @author   Ron Fox
*
*/
#ifndef VX2750XMLCONFIG_H
#define VX2750XMLCONFIG_H

#include <map>
#include <vector>
#include <string>
#include <utility>

namespace pugi {
    class xml_node;
    class xml_document;
}

namespace caen_nscldaq {
class VX2750PHAModuleConfiguration;

/**
 * @class VX2750XMLConfig
 *    Process XML file to produce configuration for VX27x0 modules.
 */
class VX2750XMLConfig
{
private:
    std::map<std::string, VX2750PHAModuleConfiguration*> m_modules;
    std::string m_xmlFile;
public:
    VX2750XMLConfig(const char* pFilename);
    virtual ~VX2750XMLConfig();
    
    void configure();
    VX2750PHAModuleConfiguration* getModule(const char* name);
    std::vector<std::string> listModules();
    
private:
    void emptyMap();
    VX2750PHAModuleConfiguration* computeDefaultConfiguration(pugi::xml_document& doc);
    void createConfigurations(
        pugi::xml_document& doc,
        VX2750PHAModuleConfiguration& defaultConfiguration
    );
    void configureBoard(VX2750PHAModuleConfiguration& config, pugi::xml_node& boardTag);
    
    std::pair<std::string, std::string> getNameAndDefault(pugi::xml_node& paramDescrip);
    void applyDefaultValue(VX2750PHAModuleConfiguration& defaultCConfig, std::string name, std::string value);
    void setSpecialValue(VX2750PHAModuleConfiguration& configuration, const std::string& name, const std::string& value);
    void setCh64TriggerMaskHalf(
        VX2750PHAModuleConfiguration& configuration, const std::string& value,
        uint16_t shift, uint64_t mask, int channel = -1
    );
    void setChannelParameter(
        VX2750PHAModuleConfiguration& configuration, unsigned int channel,
        const std::string& paramName, const std::string& paramValue
    );
    void setSpecialChannelValue(
        VX2750PHAModuleConfiguration& config, unsigned int channel,
        const std::string& paramName, const std::string& paramValue
    );
    void setListItem(
        VX2750PHAModuleConfiguration& config, const std::string& name, int element,
        const std::string& newValue
    );
    std::string getChildValue(pugi::xml_node& node, const char* childTag);
    
};

}                       // caen_nscldaq 

#endif