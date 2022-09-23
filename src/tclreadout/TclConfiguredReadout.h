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
* @file     TclConfiguredReadout.h
* @brief    Manage several modules using a Tcl configuration file.
* @author   Ron Fox
*
*/
#ifndef TCLCONFIGUREDREADOUT_H
#define TCLCONFIGUREDREADOUT_H
#include <CEventSegment.h>

#include <string>
#include <vector>

// Forward definitions:

class DynamicMultiTrigger;
class CExperiment;
class CEventTrigger;

namespace caen_nscldaq {
    class VX2750EventSegment;
    class VX2750MultiTrigger;
    class VX2750MultiModuleEventSegment;
    class VX2750TclConfig;
};

/**
 * @class TclConfiguredReadout
 *    This class maintains a multi module event segment and a multi trigger
 *    It also maintains a configuration.
 *
 *   On construction it's handed a configuration file name.  This is a Tcl script
 *   that defines and configures modules according to the DSL defined in
 *   VX2750TclConfig.   On init(), the file is processed to regenerate the
 *   configuration and then the trigger and modules are regenerated using
 *   that configuration file and module name/connection triplets givento us...
 *   normally just after construction.
 *   
 *   Key data/attributes
 *       m_pTrigger - the DynamicMultiTrigger that encapsulates our
 *                    current VX2750MultiTrigger
 *       m_pCurrentTrigger - the current multi trigger that's been put into  the
 *                     dynamic trigger.
 *       m_modules  - vector of module information.
 *       m_pCurrentConfiguration - the current configuration.
 *       m_pCurrentEventSegment  - multmodule event segment.
 *       m_configFile - Name of the configuration file.
 *
 * @note The current VX2750MultiTrigger contains the individual modules.
 * 
 */
class TclConfiguredReadout : public CEventSegment {
private:
   // Describes a module configuration/connection parameters
   
   typedef struct _ModuleInfo {
      std::string s_name;
      std::string s_ConnectionString;
      bool        s_isUsb;
   } ModuleInfo, *pModuleInfo;
private:
   CExperiment*                                 m_pExperiment;
   DynamicMultiTrigger*                         m_pTrigger;
   caen_nscldaq::VX2750MultiTrigger*            m_pCurrentTrigger;
   std::vector<ModuleInfo>                      m_modules;
   caen_nscldaq::VX2750TclConfig*               m_pCurrentConfiguration;
   caen_nscldaq::VX2750MultiModuleEventSegment* m_pCurrentEventSegment;
   std::string                                  m_configFile;
public:
    TclConfiguredReadout(const char* configFile, CExperiment* m_pExperiment);
    virtual ~TclConfiguredReadout();
    
    void addModule(const char* name, const char* connectionString, bool isUsb);
    CEventTrigger* getTrigger();
    
    // Event segment interface:
    
    virtual void initialize();
    virtual void disable();
    virtual void onBegin();
    virtual void onPause();
    virtual void onResume();
    virtual size_t read(void*pBuffer, size_t maxwords);
private:
};


#endif