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
* @file     VX2750TclConfig.cpp
* @brief    Implementation of caen_nscldaq::VX2750PHAConfiguration class.
* @author   Ron Fox
*
*/

#include "VX2750PHAConfiguration.h"
#include <TCLObject.h>
#include <TCLInterpreter.h>
#include <tcl.h>
#include <algorithm>
#include <stdexcept>
/**
 * constructor
 *    Register the command with the interpreter:
 *  @param interp - interpreter on which the command will be registered.
 *  @param pName  - Name of the command.
 */
VX2750TclConfig::VX2750TclConfig(CTCLInterpreter& interp, const char* pName) :
    CTCLObjectProcessor(interp, name, TCLPLUS::kfTRUE)
{}

/**
 * destructor
 *   - We need to get rid of the modules we've already gotten as they get
 *     new'd into existence.
 */
VX7250TclConfig::~VX2750TclConfig() {
    std::for_each(
        m_modules.begin(), m_module.end(),
        [](std::pair<std::string, VX2750PHAConfiguration*> item) {  //Lambda that
            delete item.second;                               // Deletes the module.
        }
}

/**
 * operator()
 *    Called when the command is executed.
 *  @param interp -interpreter running the command.
 *  @param objv  - command words.
 *  @return int TCL_OK - successs, TCL_ERROR failure.
 *  @note the command result may be set. See the create, config, cget, destroy and
 *        list comments.
 */
int
VX2750TclConfig::operator()(CTCLInterpreter& interp, std::vector<CTCLObject>& objv)
{
    BindAll(interp, objv);
    try {
        requireAtLeast(objv, 2, "Insufficient command parameters");
        std::string subcommand = objv[1];
        
        if (subcommand == "create") {
            create(interp, objv);
        } else if (subcommand == "config") {
            config(interp, objv);
        } else if (subcommand == "cget") {
            cget(interp, objv);
        } else if (subcommand == "delete") {
            cget(interp, objv);
        } else if (subcommand == "list") {
            list(interp, objv);
        } else {
            std::string msg = "Unrecognized subcommand: ";
            msg += subcommand;
            throw m
        }
    }
    catch (std::string msg) {
        interp.setResult(msg);
        return TCL_ERROR;
    }
    catch (std::exception& e) {
        interp.setResult(e.what());
        return TCL_ERROR;
    }
}
