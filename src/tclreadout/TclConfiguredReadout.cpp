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
* @file     TclConfiguredReadout.cpp
* @brief    Implement the TclConfiguredReadout class.
* @author   Ron Fox
*
*/
#include "TclConfiguredReadout.h"
#include "DynamicMultiTrigger.h"
#include <VX2750EventSegment.h>
#include <VX2750MultiTrigger.h>
#include <VX2750MultiModuleEventSegment.h>
#include <VX2750TclConfig.h>

using namespace caen_nscldaq;

/**
 * constructor:
 *    @param configFile - name of the configuration file to process.  This can
 *                        be any path that's specified relative to the cwd in
 *                        which the Readout is run.
 *    @param pExperiment - Pointer to the experiment object that controls the
 *                       readout program.
 */
TclConfiguredReadout::TclConfiguredReadout(
    const char* configFile, CExperiment* pExperiment
) :  m_pExperiment(pExperiment), m_pTrigger(new DynamicMultiTrigger),
    m_pCurrentTrigger(nullptr),
    m_pCurrentConfiguration(nullptr),
    m_pCurrentEventSegment(nullptr),
    m_configFile(configFile)
{}

/**
 * destructor
 *    We maintain quite a bit of dynamic data but, if we get this wrong it's
 *    not that big a deal because the typical lifetime of this object is the same
 *    as the program lifetime.
 */
TclConfiguredReadout::~TclConfiguredReadout() {
    delete m_pTrigger;
    deleteTrigger();
    delete m_pCurrentConfiguration;
    delete m_pCurrentEventSegment;
    
}
//////////////////////////////////////////////////////////////////////////////
// Define modules:

/**
 *  addModule
 *      Adds a module definition to the set of modules that will be created
 *      and, if all goes well, configured when the run begins.
 *  @param name - module name - must match the name of the module created in the
 *      configuration file.
 *   @param connectionString - connection string that's passed to the Dig2Device
 *      constructor when the module is created (e.g. stringified IP address).
 *   @param isUsb - true if the module is connected via USB false if by Ethernet.
 *      This parameter defaults to false (Ethernet connected module).
 */
void
TclConfiguredReadout::addModule(
    const char* name, const char* connectionString, bool isUsb
) {
    ModuleInfo info = {
        .s_name             = name,
        .s_ConnectionString = connectionString,
        .s_isUsb            = isUsb
    };
    m_modules.push_back(info);
}
//////////////////////////////////////////////////////////////////////////////
// Private utilities:

/**
 * deleteTrigger
 *    Deleteing the trigger also implies deleting the modules it holds.
 *    @note this may be called when thee is no current trigger
 *
 */
void
TclConfiguredReadout::deleteTrigger()  {
    if (m_pCurrentTrigger) {
        auto modules = m_pCurrentTrigger->getModules();
        for (auto m: modules) {
            delete m;
        }
        delete m_pCurrentTrigger;
        m_pCurrentTrigger = nullptr;
    }
    
}