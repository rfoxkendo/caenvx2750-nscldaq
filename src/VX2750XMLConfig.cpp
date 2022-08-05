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
* @file     VX2750XMLConfig.cpp
* @brief    Implement XML configuration.
* @author   Ron Fox
*
*/
#include "VX2750XMLConfig.h"
#include "VX2750PHAConfiguration.h"
#include <pugixml.hpp>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

namespace caen_nscldaq {
    
    //Local mapping tables and data.. it's all internal so no need for it to be
    // class data e.g.:
    
    //
    // ParameterType provides the top level description for how
    // XML paramters are mapped to internal parameter settings:
    //  -   Text - the text of the value is passed unmodified.
    //  -   Mapped - The text of the value is a lookup key into a mapping
    //               between XML values and internal values.
    //  -  Special - Special programmatic handling is done by
    //                setSpecialValue
    
    enum ParameterType {
        Text, Mapped, Special
    };
    typedef struct std::map<std::string, std::string> EnumeratorMap;
    
    const EnumeratorMap ClockSource = {
        {"CLK_SRC_INTERNAL", "Internal"},
        {"CLK_SRC_FRONTPANEL", "FPClkIn"}
    };
    
    const EnumeratorMap PolarityMap = {
        {"POLARITY_POSITIVE", "Positive"},
        {"POLARITY_NEGATIVE", "Negative"}
    };
    
    const EnumeratorMap SyncOutMap = {
      {"SYNCOUT_MODE_DISALBED", "Disabled"},
      {"SYNCOUT_MODE_SYNCIN", "SyncIn"},
      {"SYNCOUT_MODE_TESTPULSE", "TestPulse"},
      {"SYNCOUT_MODE_INTCLK", "IntClk"},
      {"SYNCOUTMODE_RUN", "Run"}
    };
    
    const EnumeratorMap StartModeMap = {
        {"START_MODE_ENC_CLKIN", "EncodedClkIn"},
        {"START_MODE_S_IN", "SINlevel"},
        {"START_MODE_S_IN_EDGE", "SINedge"},
        {"START_MODE_SW", "SWcmd"},
        {"START_MODE_LVDS", "LVDS"},
        {"START_MODE_P0", "P0"}
    };
    const EnumeratorMap TriggerOutModeMap = {
        {"TRGOUT_MODE_EXT_TRG", "TRGIN"},
        {"TRGOUT_MODE_SW_TRG", "SwTrg"},
        {"TRGOUT_MODE_PLL_CLK", "EncodedClkIn"},
        {"TRGOUT_MODE_RUN", "Run"},
        {"TRGOUT_MODE_SAMPLE_CLK", "RefClk"},
        {"TRGOUT_MODE_BUSY", "Busy"},
        {"TRGOUT_MODE_LEVEL0", "Fixed0"},
        {"TRGOUT_MODE_LEVEL1", "Fixed1"},
        {"TRGOUT_MODE_SYNCIN", "SyncIn"}
    };
    const EnumeratorMap ChVetoSrcMap = {
          {"VETO_SRC_COMMON", "BoardVeto"},
          {"VETO_SRC_OVERSATURATION", "ADCOverSaturation"},
          {"VETO_SRC_UNDERSATURATION", "ADCUnderSaturation"},
          {"VETO_SRC_DISABLED", "Disabled"},
    };
    const EnumeratorMap VetoPolarityMap = {
        {"BOARDVETO_POLARITY_HIGH", "ActiveHigh"},
        {"BOARDVETO_POLARITY_LOW", "ActiveLow"}
    };
    const EnumeratorMap BoardVetoSrcMap = {
        {"BOARDVETO_SOURCE_SIN", "SIN"},
        {"BOARDVETO_SOURCE_GPIO", "GPIO"}, 
        {"BOARDVETO_SOURCE_DISABLED", "Disabled"}
    };
    // Describes a parameter in the  configuration, it has a name and a type
    // that specifies if we only have one value (board) or several (channel).
    
    typedef struct {
        std::string   s_configurationName;    // Internal configuration param name.
        enum ParameterType s_type;            // How to map XML -> internal
        const EnumeratorMap* s_valueMap;            // For mapped mode the mapping.
        size_t        s_nElements;            // Nuber of list elmeents.
    } ParameterInfo;
    
    // This map maps names of parameters in the XML file to names and types
    // of parameters in the configuration.
    
    const std::map<std::string, ParameterInfo> ParameterMapping = {
        {"SRV_PARAM_CH_ENABLED", {"channelenables", Text, nullptr, 64}},
        {"SRV_PARAM_CLOCK_SOURCE", {"clocksource", Mapped, &ClockSource, 1}},
        {"SRV_PARAM_CH_TRAP_PEAKING", {"efpeakingpos", Text, nullptr, 64}},
        {"SRV_PARAM_CH_PRETRG", {"pretriggersamples", Text, nullptr, 64}},
        {"SW_PARAMETER_CH_ENERGYHIGHCUT", {"energyskimhigh", Text, nullptr, 64}},
        {"SRV_PARAM_TIMETAG", {"readrawtimes", Text, nullptr, 1}},
        {"SRV_PARAM_WAVEFORMS", {"readanalogprobes", Text, nullptr, 1}},
        {"SRV_PARAM_RECLEN", {"recordsamples", Text, nullptr, 64}},
        {"SRV_PARAM_CH_POLARITY", {"inputpolarities",  Mapped, &PolarityMap, 64}},
        {"SRV_PARAM_CH_FAST_TRIANG_RISE", {"tfrisetime", Text, nullptr, 64}},
        {"SRV_PARAM_CH_THRESHOLD", {"triggerthresholds", Text, nullptr, 64}},
        {"SRV_PARAM_CH_TRG_HOLDOFF", {"tfretriggerguard", Text, nullptr, 64}},
        {"SRV_PARAM_CH_TTF_SMOOTING", {"efbaselineavg", Text, nullptr, 64}},
        {"SRV_PARAM_CH_TTF_DELAY", {"efpileupguardt", Text, nullptr, 64}},
        {"SRV_PARAM_CH_FAST_TRIANG_RISE", {"trfisetime", Text, nullptr, 64}},
        {"SRV_PARAM_CH_PEAK_NSMEAN", {"efpeakingavg", Text, nullptr, 64}},
        {"SRV_PARAM_CH_TRAP_TRISE", {"efrisetime", Text, nullptr, 64}},
        {"SRV_PARM_CH_TRAP_TFLAT", {"efflattoptime", Text, nullptr, 64}},
        {"SRV_PARAM_CH_TRAP_POLEZERO", {"efpolezero", Text, nullptr, 64}},
        {"SRV_PARAM_CH_TRAP_POLEZERO", {"efpeakingpos", Text, nullptr, 64}},
        {"SRV_PARAM_CH_ENERGY_FINE_GAIN", {"effinegain", Text, nullptr, 64}},
        {"SRV_PRARAM_OUT_CLOCK_ENABLE", {"", Special, nullptr, 1}},
        {"SRV_PARAM_SYNCOUT_MODE", {"syncoutmode", Mapped, &SyncOutMap, 1}},
        {"SRV_PARAM_AUTO_DISARM_ENABLE", {"autodisarm", Text, nullptr, 1}},
        {"SRV_PARAM_START_MODE", {"startsource", Mapped, &StartModeMap, 1}},
        {"SRV_PARAM_TRGOUT_MODE", {"triggeroutmode", Mapped, &TriggerOutModeMap, 1}},
        {"SRV_PARAM_START_DELAY", {"rundelay", Text, nullptr, 1}},
        {"SRV_PARAM_VETO_SRC", {"boardvetosrc", Mapped, &BoardVetoSrcMap, 1}},
        // {"SVR_PARAM_WAVEFORM_DOWNSAMPLING", {...}}                 // No param definitions??
        {"SRV_PARAM_VETO_POLARITY", {"boardvetopolarity", Mapped, &VetoPolarityMap}},
        {"SRV_PARAM_VETO_WIDTH", {"chanvetowidth", Text, nullptr, 64}},
        {"SRV_PARAM_CH_SELF_TRG_ENABLE", {"", Special, nullptr, 64}},
        {"SRV_PARAM_CH_CH64_TRG_ENABLE", {"", Special, nullptr, 64}},
        {"SRV_PARAM_CH_64_TRG_MASK_LOW", {"", Special, nullptr, 64}},
        {"SRV_PARAM_CH_64_TRG_MASK_HIGH", {"", Special, nullptr, 64}},
        {"SRV_PARAM_CH_VETO_SRC", {"chanvetosrc", Mapped, &ChVetoSrcMap, 64}}, 
        {"SRV_PARAM_VGA_GAIN", {"vgagain", Text, nullptr, 4}}
    };
    
    /**
     * construction
     *   Really does nothing.. just saves the xml filename.
     *
     * @param pFilename - Pointer to null terminated XML filename.
     */
    VX2750XMLConfig::VX2750XMLConfig(const char* pFilename) :
        m_xmlFile(pFilename)
    {}
    /**
     * destructor
     *   Just needs to empty the map, deleting all configurations.
     */
    VX2750XMLConfig::~VX2750XMLConfig()
    {
        emptyMap();
    }
    
    /**
     * configure
     *    Process the XML configuration file, which produces the m_modules conten
     *    @throw - various std::exception descended object types depending on what the
     *         failure is.  We directly throw std::invalid_argument if the
     *         parse of the XML failed to produce a good DOM.
     */
    void VX2750XMLConfig::configure()
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(m_xmlFile.c_str());
        
        if (result) {
            std::unique_ptr<VX2750PHAModuleConfiguration> defaultConfiguration (
                computeDefaultConfiguration(doc));
            emptyMap();
            createConfigurations(doc, *defaultConfiguration);
            
        } else {
            std::stringstream error;
            error << "XML configuration file" << m_xmlFile << " failed to parse: ";
            error << result.description() << " : " << result.offset;
            
            std::string msg = error.str();
            throw std::invalid_argument(msg);
        }
        
    }
    /**
     * getModule
     *    @param name - name of a configuration produced by config.
     *    @return VX2750PHAmoduleConfiguration*  - pointer into the result.
     *    @throw std::invalid_argument - if the module does not exit.
     */
    VX2750PHAModuleConfiguration*
    VX2750XMLConfig::getModule(const char* name)
    {
        auto p = m_modules.find(name);
        if (p != m_modules.end()) {
            return p->second;
        } else {
            std::stringstream error;
            error << "No such module: " << name;
            
            std::string msg = error.str();
            
            throw std::invalid_argument(msg);
        }
        
    }
    /**
     * listModules
     *    Lists the configuration names.
     * @return std::vector<std::string>
     */
    std::vector<std::string>
    VX2750XMLConfig::listModules()
    {
        std::vector<std::string> result;
        for (auto& p : m_modules) {
            result.push_back(p.first);
        }
        return result;
    }
    /////////////////////////////////////////////////////////////////////////
    // private utility methods.
    //
    /**
     * emptMap
     *    destroy all of the exiting configurations and clear
     *    the module map
     */
    void
    VX2750XMLConfig::emptyMap()
    {
        
        // First delete the configurations:
        
        for (auto& p : m_modules) {
            delete p.second;
        }
        // Clear the map itself
        
        m_modules.clear();
    }
    /**
     * computeDefaultConfiguration
     *    Iterate over all of the <parameterDescription> tags
     *    computing a configuration that represents the defaults for a module
     *    configuration according to COMPASS.
     * @param doc - the XML document from pugi.
     * @return VX2750PHAModuleConfiguration* pointer to a dynamically created
     *         configuration that is the result of computing the default config.
     */
    VX2750PHAModuleConfiguration*
    VX2750XMLConfig::computeDefaultConfiguration(pugi::xml_document& doc)
    {
        auto pResult = new VX2750PHAModuleConfiguration("Default");
        
        try {
            // Next line may be incorrect.
            auto config = doc.child("configuration");     // I think this is the <config> tag?
            std::cout << "First child of document is " << config.name() << std::endl;
            
            // Now traverse over the <parameterDescriptorTags:
            
            for (auto descriptor = config.child("parameterDescriptor");
                descriptor;
                descriptor = descriptor.next_sibling("parameterDescriptor")) {
                std::pair<std::string, std::string> nameAndDefault =
                    getNameAndDefault(descriptor);
                std::string paramName = nameAndDefault.first;
                std::string value     = nameAndDefault.second;
                
                // Only apply the default if its not empty... else use the built in:
                
                if (value != "")  {
                    applyDefaultValue(*pResult, paramName, value);
                }
            }
        }
        catch(...) {
            delete pResult;   // can't be a smart ptr and be returned.
            throw;
        }
        
        return pResult;
    }
    /**
     * createConfigurations
     *     Given a default configuration that can be  cloned, iterates through
     *     all <board> tags. For each board tag"
     *      - a new configuration is created whose name is given by the <label>tag.
     *      - the <parameters> tag <entry> subtags are iterated over to establish
     *        a board level default configuration.
     *      - The <channel> tags are iterated over, the <index> is a channel number
     *        and <values> tag contains <entry> tags which modify the
     *        per channel configuration from base.
     *      - The new configuration is entered into the module's map indexed by
     *        it's label.
     *  @param doc - the dom parsed from the XML <configuration> is the root tag.
     *  @param defaultConfiguration - default configuration that's cloned to create
     */
    void
    VX2750XMLConfig::createConfigurations(
        pugi::xml_document& doc,
        VX2750PHAModuleConfiguration& defaultConfiguration    
    )
    {
        auto config = doc.child("configuration");
        // Iterator over <board> children of the <config> tag
        
        for (auto board = config.child("board"); board; board.next_sibling("board")) {
            std::string label = getChildValue(board, "label");
            auto pConfig = new VX2750PHAModuleConfiguration(defaultConfiguration);
            pConfig->setName(label.c_str());
            try {
                configureBoard(*pConfig, board);
            }
            catch (...) {
                delete pConfig;
            }
            m_modules[label] = pConfig;
        }
        
    }
    /**
     * configureBoard
     *   Given the <board> tag:
     *   - Ensure the <dpptype> subtag contains "DPP_PHA"
     *   - Processes the <entry> tags in the <parameters> child (it's not an
     *     error either for there to be no <parameters> tag or for there to be
     *     no <entry> children to override the defaults).
     *   - For each <channel> tag, extract the index (contents of <index> subtag)
     *     and process the <entry> subtags in the <values> tag
     *     (again no harm if <values> does not exist or contains no <entries>
     * @note - at present we don't know/understand how to select waveforms or
     *         their readout. Nor do we understand the <lvdsGroup> tag
     *         contents yet....we can leave that for another day.
     * @param config - board configuration reference.
     * @param boardTag - pugi::xml_node for the <board> tag.
     * @throw std::invalid_argument - board type is not DPP_PHA
     * @throw std::invalid_arugment - board <entry> tag has no <value> child.
     */
    void
    VX2750XMLConfig::configureBoard(
        VX2750PHAModuleConfiguration& config, pugi::xml_node& boardTag
    )
    {
        std::string dppType = getChildValue(boardTag, "dpptype");
        std::string boardName = getChildValue(boardTag, "label");
        if (dppType != "DPP_PHA") {
            std::stringstream msg;
            msg << "Board " << boardName
                << " Is not a DPP_PHA board but a " << dppType;
            std::string emsg = msg.str();
            throw std::invalid_argument(emsg);
        }
        auto paramTag = boardTag.child("parameters");
        if (paramTag) {
            for (auto entry = paramTag.child("entry"); entry; entry.next_sibling("entry")) {
                // Pull the entry and value from the entry:
                
                std::string paramName = getChildValue(entry, "key");
                std::string value;
                auto outerValue = entry.child("value");
                if (outerValue) {
                    value = getChildValue(outerValue,"value");
                } else {
                    std::stringstream msg;
                    msg << "Tag  " << paramName << " is missing a <value> tag";
                    std::string emsg = msg.str();
                    throw std::invalid_argument(emsg);
                }
                applyDefaultValue(config, paramName, value);
            }
        }
        // Now iterate over the channel tags:
        // These have the form:
        //   <channel>
        //      <index>channel-number</index>
        //      <values>
        //         <entry>
        //           <key>parameter_name</key>
        //           <value>parametervalue</value
        //         <entry>
        //            ...
        //      </values>
        //   </channel>
        //   Note that <channel> tags are children of <board>
        
        for(auto channelTag = boardTag.child("channel"); channelTag; channelTag.next_sibling("channel")) {
            std::string chanNumberString = getChildValue(channelTag, "index");
            int channelNumber = std::stoi(chanNumberString, nullptr,  0);
            auto valuesTag = channelTag.child("values");
            if (!valuesTag) {
                std::stringstream errorMessage;
                errorMessage << "Missing <values> tag for channel " << channelNumber
                    << " of board " << boardName;
                std::string msg = errorMessage.str();
                throw std::logic_error(msg);
            }
            for (auto entryTag = valuesTag.child("entry"); entryTag; entryTag.next_sibling("entry")) {
                std::string name = getChildValue(entryTag, "key");
                std::string value = getChildValue(entryTag, "value");
                setChannelParameter(config, channelNumber, name, value);
            }
        }
        
        
    }
    
    /**
     * getNameAndDefault
     *   Given an XML node that's a <parameterDesriptor> fetch the ID and its
     *   contents (the parameter name) and the defaultValue contents as the
     * @param paramDescrip - node to query.
     * @return std::pair<std::string, std::string> - first is name second is value.
     */
    std::pair<std::string, std::string>
    VX2750XMLConfig::getNameAndDefault(pugi::xml_node& paramDescrip)
    {
        std::pair<std::string, std::string> result;
        result.first = getChildValue(paramDescrip, "id");
        result.second = getChildValue(paramDescrip, "defaultValue");
        return result;
    }
    /**
     * applyDefaultValue
     *    Given the default configuration, the name and value of a parameter,
     *    applies that as the default.
     *    -  If there's no mapping from XML parameter name to configuration parameter
     *       name silently bail.
     *    -  create a list which contains the correct repetitions of value.
     *    -  Configure.
     *  @param defaultConfig - the  default configuration.
     *  @param name          - name of the parameter.
     *  @param value         -  value to set.
     */
    void
    VX2750XMLConfig::applyDefaultValue(VX2750PHAModuleConfiguration& defaultConfig, std::string name, std::string value)
    {
        auto p = ParameterMapping.find(name);
        if (p != ParameterMapping.end()) {
            std::string mappedName = p->second.s_configurationName;
            ParameterType type     = p->second.s_type;
            const EnumeratorMap* pMap    = p->second.s_valueMap;
            size_t      elements   = p->second.s_nElements;
            
            // The value might need to be mapped:
            
            std::string actualValue = value;
            if (type == Special) {
                setSpecialValue(defaultConfig, name, value);
                return;
            }
            if (type == Mapped) {
                auto pValue = pMap->find(actualValue);
                if (pValue != pMap->end()) {
                    actualValue = pValue->second;
                } else {
                    std::stringstream error;
                    error << "Error looking up mapped value for" << value << " parameter: " << name;
                    std::string msg = error.str();
                    throw std::logic_error(msg);
                }
            } 
            std::string configValue;
            for (int i =0; i < elements; i++) {
                configValue += actualValue;
                configValue += ' ';
            }
            
            
            defaultConfig.configure(mappedName, configValue);
        } 
    }
    /**
     * setSpecialValue
     *     Set a special scalar value.  THese are values that don't have
     *     a good 1:1 mapping to configuration parameters:
     * @param name - XML Name of the parameter.
     * @param value - proposed new value.
     * @param config - The configuration being modified.
     */
    void
    VX2750XMLConfig::setSpecialValue(
        VX2750PHAModuleConfiguration& configuration, const std::string& name, const std::string& value
    )
    {
        if (name == "SRV_PARAM_OUT_CLOCK_ENABLE") {
            // send clock to both FP and backplane:
            
            configuration.configure("outputp0clock", value);
            configuration.configure("outputfpclock", value);
            
        } else if (name == "SRV_PARAM_CH_SELF_TRG_ENABLE") {
            std::string src;
            if (value == "true") {
                std::string actualValue;
                for (int i =0; i < 64; i++) {
                    actualValue += "ChSelfTrigger ";
                }
                configuration.configure("wavetriggersrc", actualValue);
                configuration.configure("eventtriggersrc", actualValue);
            }

        } else if (name == "SRV_PARAM_CH_CH64_TRG_ENABLE") {
            std::string src;
            
            if (value == "true") {
                std::string actualValue;    
                for (int i=0; i < 64; i++) {
                    actualValue += "Ch64Trigger ";
                }
                configuration.configure("wavetriggersrc", actualValue);
                configuration.configure("eventtriggersrc", actualValue);
            }

        } else if (name == "SRV_PARAM_CH64_TRG_MASK_LOW") {
            setCh64TriggerMaskHalf(configuration, value, 0, 0xffffffff);
        } else if (name == "SRV_PARAM_CH64_TRG_MASK_HIGH") {
            setCh64TriggerMaskHalf(configuration, value, 32, 0xffffffff00000000);
        }
    }
    /**
     * setCh64TriggerMaskHalf
     *    In the XML version the 64 channel trigger masks are set in lower
     *    and upper halves. This method integrates a trigger mask half into the
     *    existing trigger mask lists:
     *
     * @param configuration - Configuration being manipulated.
     * @param value - texstified value of the mask half.  This can translate
     *                 to a uint32_t.
     * @param shift - Positioning shift count to put the value in the right half quad.
     * @param mask  - Mask that covers the region to insert.
     * @param channel - if this  value is not -1, it represents the single channel
     *                to modify.
     */
    void
    VX2750XMLConfig::setCh64TriggerMaskHalf(
        VX2750PHAModuleConfiguration& configuration, const std::string& value,
        uint16_t shift, uint64_t mask, int channel
    )
    {
        mask = ~mask;                     // turn it into a removal mask.
        std::uint64_t ivalue = std::strtoul(value.c_str(), nullptr, 0);
        ivalue = ivalue << shift;
        std::vector<std::uint64_t> masks = configuration.getUnsignedList("channeltriggermasks");
        
        // Fold the new value into the mask:
        
        for (int i =0; i < masks.size(); i++) {
            if ((channel == -1) || (channel == i)) {
                int idx = i;
                masks[idx] = (masks[idx] & mask) | ivalue;
            }
        }
        std::stringstream strValue;
        for (int i = 0; i < masks.size(); i++) {
            strValue << masks[i] << " ";
        }
        std::string sValue = strValue.str();
        configuration.configure("channeltriggermasks" ,sValue);
        
    }
    /**
     * setChannelParameter
     *    Sets a per channel parameter.  If we see a parameter that's not
     *    a per channel parameter, we'll noisily ignore it.  By noisily I mean
     *    we'll output an error message to stderr.
     *  @param config - references the configuration we're 'editing'
     *  @param channel - The number of the channel that's been given this parameter.
     *  @param paramName - name (XML) of the parameter to modify.
     *  @param paramValue - New value.
     *
     *  Assumptions:
     *     - All parameters are described by the parameter descriptions and therfore
     *       the ParameterMapping map is valid.
     *     - All values are legal (once mapped) and therefore don't need to go through the
     *       legality checking of the configuration subsystem.
     *     - All channel numbers are legal.
     *  @note -specials are handled by setSpecialChannelValue rather than setSpecialValue
     *       since again we need to handle only modifying the specific parameter.
     *  @note - a useful utility we use is setListItem which can modify one element
     *         of a list item (per channel parameters are lists).
     *       
     */
    void
    VX2750XMLConfig::setChannelParameter(
        VX2750PHAModuleConfiguration& configuration, unsigned int channel,
        const std::string& paramName, const std::string& paramValue
    )
    {
        auto pMapEntry = ParameterMapping.find(paramName);
        if (pMapEntry != ParameterMapping.end()) {
            std::string mappedName = pMapEntry->second.s_configurationName;
            ParameterType type     = pMapEntry->second.s_type;
            const EnumeratorMap* pMap    = pMapEntry->second.s_valueMap;
            size_t      elements   = pMapEntry->second.s_nElements;
            if (elements == 1) {
                std::cerr << "Warning channel setting attemped for scalar parameter: " << mappedName << std::endl;
                return;
            }
            switch (type) {
                case Text:
                    setListItem(configuration , mappedName, channel, paramValue);
                    break;
                case Mapped: {
                        auto pValue = pMap->find(paramValue);
                        if (pValue != pMap->end()) {
                            setListItem(configuration, mappedName, channel, pValue->second);    
                        } else {
                            std::stringstream error;
                            error << "Error looking up mapped value for" << paramValue << " parameter: " << paramName;
                            std::string msg = error.str();
                            throw std::logic_error(msg);
                        }
                    }
                    break;
                case Special:
                    setSpecialChannelValue(configuration, channel, paramName, paramValue);
                    break;
            }
        }
    }
    /**
     * setListItem
     *     Given a list parameter, an index, and a new value for that index,
     *     reconfigures the parameter per the requested index.
     *  @param config - references the configuration being edited.
     *  @param name   - Name of the parameter given _in the configuration_
     *  @param element - list element number to change.
     *  @param newValue - New value to give to that element.
     *  @throw std::logic_error - if the element is out of range of the list indices.
     */
    void
    VX2750XMLConfig::setListItem(
        VX2750PHAModuleConfiguration& config, const std::string& name, int element,
        const std::string& newValue
    )
    {
        std::vector<std::string> configList = config.getList(name);
        if (element >= configList.size()) {
            std::stringstream errorMessage;
            errorMessage << "Attempting to configure " << config.getName()
            << " parameter: " << name << " with a list index of "
            << element << ". But the existing configuration list has only "
            << configList.size() << " elements.";
            std::string msg = errorMessage.str();
            throw std::logic_error(msg);
        }
        configList[element] = newValue;
        
        // Build the list value:
        
        std::string configValue = "";
        for (auto v : configList) {
            configValue += v;
            configValue += " ";
        }
        config.configure(name, configValue);
    }
    /**
     * setSpecialChannelValue
     *    As with the defaults, some values are special, they
     *    require  handling that can't be easily encapsulated in our simple
     *    table driven scheme.  For per channel specials, this method
     *    modifies a single value of such an item.
     *  @param config - references the config.
     *  @param channel - channel number being configured (index intolist param).
     *  @param paramName - XML parameter name
     *  @param paramValue - New value for that channel's parameter.
     */
    void
    VX2750XMLConfig::setSpecialChannelValue(
        VX2750PHAModuleConfiguration& configuration, unsigned int channel,
        const std::string& paramName, const std::string& paramValue
    )
    {
         if (paramName == "SRV_PARAM_CH_SELF_TRG_ENABLE") {
            std::string src;
            if (paramValue == "true") {
                setListItem(
                    configuration, "wavetriggersrc", channel, "ChSelfTrigger"
                );
                setListItem(
                    configuration, "eventtriggersrc", channel, "ChSelfTrigger"
                );
                
            }

        } else if (paramName == "SRV_PARAM_CH_CH64_TRG_ENABLE") {
            std::string src;
            if (paramValue == "true") {
                setListItem(configuration, "wavetriggersrc", channel, "Ch64Trigger");
                setListItem(configuration, "eventtriggersrc", channel, "Ch64Trigger");
                
            }
        } else if (paramName == "SRV_PARAM_CH64_TRG_MASK_LOW") {
            setCh64TriggerMaskHalf(configuration, paramValue, 0, 0xffffffff,  channel);
        } else if (paramName == "SRV_PARAM_CH64_TRG_MASK_HIGH") {
            setCh64TriggerMaskHalf(configuration, paramValue, 32, 0xffffffff00000000, channel);
        }
    }
    
    /**
     *   getChildValue
     *      Given an XML node, locate the specified child (one level deep), and
     *      return the string contents of the node. Suppose, for example we have
     *      the simple document fragment:
     *  \verbatim
     *      <root>
     *         <child>Some contents</child>
     *      </root>
     *  \verbaitm
     *
     *  if the node is the node for <root>, getChildValue(node, "child") will return
     *  "Some contents"
     *
     * @param node - XML node we're searching inside.
     * @param childTag - tag we want the guts of.
     * @return std::string - contents of childTag.
     * @throw std::invalid_argument - childTag Does not exist or its contents are not
     *        a string.
     */
    std::string
    VX2750XMLConfig::getChildValue(pugi::xml_node& node, const char* childTag)
    {
        auto child = node.child(childTag);
        if (child) {
            auto pcnode = child.first_child();
            if (pcnode.type() == pugi::node_pcdata) {
                return std::string(pcnode.value());
            } else {
                std::stringstream msg;
                msg << "getChildValue: specified child: " << childTag <<
                " of " << node.name() <<
                " contents are not #PCDATA";
                std::string etext = msg.str();
                throw std::invalid_argument(etext);
            }
        } else {
            std::stringstream msg;
            msg << "getChildValue specified child " << childTag << " for "  << node.name()
                << " not found";
            std::string etext = msg.str();
            throw std::invalid_argument(etext);
        }
    }
    
    
}                                      // end caen_nscldaq namespace.`