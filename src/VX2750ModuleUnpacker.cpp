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
* @file     VX2750ModuleUnpacker.cpp
* @brief    Implement unpacking of a single module.
* @author   Ron Fox
*
*/

#include "VX2750ModuleUnpacker.cpp"
#include <TreeParameter.h>
#include <sstream>
#include <std::string>
#include <stdexcept>
#include <iostream>

namespace caen_spectcl {
/**
 * constructor
 *    Construct the tree parameters and reset the channel mask to prep for
 *    the first set of decodings
 * @param moduleName - must match the module name in the data to unpack.
 * @param paramBaseName - used for the parameter base names for the treeparameters.
 *                 We create 64 element tree parameter arrays::
 *                 -  basname.ns   - Timestamp in nanoseconds.
 *                 -  basename.rawTime - Raw coarse timestamps.
 *                 -  basename.cfdTime - CFD timestamp.
 *                 -  basename.energy  - DPP eneregies fished out of the waveforms.
 */
VX2750ModuleUnpacker::VX2750ModuleUnpacker(
    const char* moduleName, const char* paramBaseName
) :
    m_moduleName(moduleName),
    m_channelMask(0),
    m_ns(nullptr), m_rawTimestamp(nullptr), m_fineTimestamp(nullptr),
    m_energy(nullptr)
{
    {
        std::stringstream ns;
        ns << paramBaseName << ".ns";
        m_ns = new CTreeParameterArray(ns.str(), "ns", 64, 0);
    }
    {
        std::stringstream ts;
        ts << paramBaseName << ".rawTime";
        m_rawTimestamp = new CTreeParameterArray(ts.str(), "arb", 64, 0);
    }
    {
        std::stringstream finet;
        finet << paramBaseName << ".cfdTime";
        m_fineTimestamp = new CTreeParameterArray(finet.str(), "arb", 64, 0);
    }
    {
        std::stringstream e;
        e << paramBaseName << ".energy";
        m_energy = new CTreeParameterArray(e.str(), "arb", 64, 0);
    }
    
}
/**
 * destructor
 *   Must delete the tree parameter arrays we created.  Everything else self-destructs
 */
VX2750ModuleUnpacker::~VX2750ModuleUnpacker()
{
    delete m_ns;
    delete m_rawTimestamp;
    delete m_fineTimestamp;
    delete m_energy;
}

/**
 * reset
 *   Should be called before decoding the hits that make up an event built event.
 *   Just clears the m_channelMask field so that we can properly keep track of the
 *   channels in an event.  Clear's the probe arrays as well.
 */
void
VX2750ModuleUnpacker::reset()
{
    m_channelMask = 0;
    m_analogProbe1Samples.clear();
    m_analogProbe2Samples.clear();
    m_digitalProbe1Samples.clear();
    m_digitalProbe2Samples.clear();
    m_digitalProbe3Samples.clear();
    m_digitalProbe4Samples.clear();
}
/**
 * unpackModule
 *   Unpack a hit into the appropriate chunks of the tree parameter array and
 *   the internal data which can be fetched by event processors e.g.
 * @param pData - pointer to the module data.
 * @return const void* - Pointer to the byte just after the unpacked data.
 */
const void*
VX2750ModuleUnpacker(const void* pData)
{
    // This union allows us to access the data in the most natural way
    // for each data type:
    
    union pointer {
        char*         c
        std::uint8_t* b;
        std::uint16_t* w;
        std::uint32_t* l;
        std::uint64_t* q;
    } p = reinterpret_cast<union pointer>(pData);
    
    // Check the mdoule name:
    
    std::string name(p.c);
    if (name != m_moduleName) {
        throw std::logic_error("Mismatch between data module name and unpacker module name!");
    }
    p.b += name.size() + 1;                      // +1 for null term.
    
    std::uint16_t ch = *p.w;
    std::uint64_t m  = 1;
    m = m << ch;                                // Bit in channel mask
    
    if (m & m_channelMask != 0) {
        std::cerr << "** Warning: duplicate channel " << ch <<
            " in module: " << name << " Second hit overwrites first" <<  std::endl;
    }
    m_channelMask |= m;
    
    p.w++;
    
    // Timestamp, coarse, fine, and energy.
    
    
    
    return p.b;              // Any field will do.
}

}                                             // caen_spectcl namespace.