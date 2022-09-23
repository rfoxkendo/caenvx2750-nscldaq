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
#include <VX2750EventSegment.h>
#include <VX2750MultiTrigger.h>
#include <VX2750MultiModuleEventSegment.h>
#include <VX2750TclConfig.h>

using namespace caen_nscldaq;