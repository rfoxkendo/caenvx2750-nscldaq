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
* @file     VX2750MultiTrigger.h
* @brief    Manage the trigger for several modules.
* @author   Ron Fox
*
*/
#ifndef CAENVX2750MULTITRIGGER_H
#define CAENVX2750MULTITRIGGER_H
#include <CEventTrigger.h>
#include <vector>

class VX2750PhaTrigger;
class VX2750Pha;

namespace caen_nscldaq {
    /**
     * @class VX2750MultiTrigger
     *     Manages several CAENVX2750 modules as a single trigger.
     *     Each pass of the trigger collects all of the modules
     *     that have triggered into a vector which can be fetched by the
     *     readout.  The trigger condition is any module triggered.
     */
    class VX2750MultiTrigger : public CEventTrigger
    {
    private:
        std::vector<VX2750PhaTrigger*> m_triggers;
        std::vector<VX2750Pha*> m_triggeredModules;
        
    public:
        VX2750MultiTrigger();
        virtual ~VX2750MultiTrigger();
        
        void addTrigger(VX2750PhaTrigger* pTrigger);
        void removeTrigger(VX2750PhaTrigger* pTrigger);
        std::vector<VX2750PhaTrigger*> getTriggers() const;
        std::vector<VX2750Pha*> getModules() const;
        const std::vector<VX2750PhaTrigger*>& getTriggeredModules() const;
        
        virtual bool operator()();
    };
}                                             // namespace

#endif