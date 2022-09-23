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
#include <Exception.h>
#include <VX2750TclConfig.h>
#include <TCLInterpreter.h>

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

/**
* getTrigger
*    Returns a pointer to the trigger object we created.
*    This is actually the DynamicMultiTrigger.
*    This allows the Skeleton code to instantiate
*    and register this event segment and then get its
*    trigger and register that.
* @return CEventTrigger*
*/
CEventTrigger*
TclConfiguredReadout::getTrigger() {
    return m_pTrigger;
}
////////////////////////////////////////////////////////////////////////////////
// Event segment interface.
//

/**
 * onBegin
 *    Here's where all the heavy lifting is done.  Since we are beginning a run,
 *    we also know that we are not yet taking data.
 *    -   Delete the prior configuration, trigger, event segment -- if there is one
 *        (note that deleting a nullptr is a harmless no-op by the C++ standard).
 *    -  In order to fail early, we process the configuration file to create a new
 *       configuration -- failure -- we abort the begin by throwing an exception.
 *    -  We create modules for each entry in m_modules.
 *    -  We create a trigger for each module and add it to a new VX2750Event segment
 *       specific event trigger...we make that trigger what they dynamic trigger uses.
 *    -  We create a multimodule event segment.
 *    -  We invoke the onBegin for the multimodule event segment.
 *   All of this sets up the infrastructure to allow us to simply delegate
 *   the remainder of our event acquisitino methods to the multmodule event segment.
 *   
 */
void
TclConfiguredReadout::onBegin()
{
    // Kill off the previous readout stuff.
    
    delete m_pCurrentConfiguration;
    m_pCurrentConfiguration = nullptr;
    deleteTrigger();
    delete m_pCurrentEventSegment;
    m_pCurrentEventSegment = nullptr;
    
    
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
    
    readConfiguration();         // Won't return on error
    checkModuleConfiguration();  // won't return on error.
    createTrigger();             // also create the modules in the trigger.
    
    m_pCurrentEventSegment = new VX2750MultiModuleEventSegment(m_pExperiment, m_pCurrentTrigger);
    
    m_pTrigger->clear();
    m_pTrigger->addTrigger(m_pCurrentTrigger);
    
    m_pCurrentEventSegment->onBegin();
}
/**
 * initialize
 *    Called just prior to data taking to do hardware initialization.
 */
void
TclConfiguredReadout::initialize() {
    if (m_pCurrentEventSegment) {
        m_pCurrentEventSegment->initialize();
    }
}
/**
 * disable
 *    turns off hardware just prior to ending the run.
 */
void
TclConfiguredReadout::disable() {
    if (m_pCurrentEventSegment) {
        m_pCurrentEventSegment->disable();
    }
}
/**
 * onPause
 *    Called in respose to the pause command.
 */
void
TclConfiguredReadout::onPause() {
    if (m_pCurrentEventSegment) {
        m_pCurrentEventSegment->onPause();
    }
}
/**
 * onResume
 *    Called in response to a resume command:
 */
void
TclConfiguredReadout::onResume()
{
    if (m_pCurrentEventSegment) {
        m_pCurrentEventSegment->onResume();
    }
}
/**
 * read
 *    Called to actually read data.
 *  @param pBuffer -  pointer to the buffer.
 *  @param maxwords- Maximum number of 16 bit words the buffer can hold.
 */
size_t
TclConfiguredReadout::read(void* pBuffer, size_t maxwords) {
    return m_pCurrentEventSegment->read(pBuffer, maxwords);
}