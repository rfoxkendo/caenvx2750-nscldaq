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
* @file    VX2750Pha.h
* @brief    Class definition for PHA firmware in the CAEN VX2750
* @author   Ron Fox
*
*/
#ifndef VX2750PHA_H
#define VX2750PHA_H
#include "Dig2Device.h"
#include <string>
#include <cstdint>

caen_nscldaq {

/**
 * VX2750PHA - support for parameters in the VX2750.
 *             Since this derives from the Dig2Device class low level
 *             parameter access is also provided.
 */
class VX2750Pha : public Dig2Device
{
public
    typedef enum _FwType {
        DPP_PHA, DPP_ZLE, DPP_PSD, DPP_DAQ, DPP_OPEN, Scope,
        Unknown
    } FwType;
    typedef enum _FormFactor {
        VME, VME64X, DT, Unknown
    } FormFactor;
    
    typedef enum _ClockSource {
        Internal, FrontPanel, P0, Link, DIPSelected
    } ClockSource;
    
    typedef enum _StartSource {
        EncodedClockIn, SINLevel, SINEdge, SWCommand, LVDS, P0
    } StartSource;
    
    typedef enum _GlobalTriggerSource {
        TriggerIn, P0, Software, LFDS, InternalA, InternalB, InternalAandB,
        InternalAorB, EncodedClockIn, GPIO, TestPulse
    } GlobalTriggerSource;
    
    typedef enum _WaveTriggerSource {
        InternalA, InternalB, GlobalTriggerSource, TRGIN, ExternalInhibit,
        ADCUnderSaturation, ADCOverSaturation, Software, CHannelSelfTrigger,
        AnyChannelSelfTrigger, Disabled
    } WaveTriggerSource;
    
    typedef enum _EventTriggerSource {
        InternalB, InternalA, GlobalTriggerSource, TRGIN, Software, ChannelSelfTrigger,
        AnyChannelSelfTrigger, Disabled
    } EventTriggerSource;
    
    typedef enum _TimestampResetSource {
        Start, SIN, GPIO, EncodedClockIn
    } TimestampResetSource;
    
    typedef enum _TraceRecordMode {
        Always, OnRequest
    } TraceRecordMode;
    
    typedef enum _TRGOUTMode {
        TRGIN, P0, Software, LVDS, InternalA, InternalB, InternalAandInternalB,
        INternalAorInternalB, EncodedClockIn, Run, ReferenceClock, TestPuse,
        Busy, Zero, One, SynchIn, SIN, GPIO, AcceptedTrigger TriggerClock
    } TRGOUTMode;
    
    typedef enum _GPIOMode {
        Disabled, TriggerIn, P0, SIN LVDS, IntarnalA, InternalB, InternalAandInternalB,
        InternalAorInternalB, EncodedClockIn, Software, Run, ReferenceClock,
        TestPulse, Busy, Zero, One
    } GPIOMode;
    
    typedef enum _BusyInSource {
        SIN, GPIO, LVDS, Disabled 
    } BusyInSource
    
    typedef enum _SyncOutMode {
        Disabled, SynchIn, TestPulse, InternalClock, Run
    } SyncOutMode;
    
    typedef enum _VetoSource {
        SIN, LVDS, GPIO, P0, EncodedClock, Disabled
    } VetoSource;
    
    typedef enum _VetoPolarity {
        ActiveHigh, ActiveLow
    } VetoPolarity;
    
    typedef enum _ChannelVetoSource {
        OverSaturation, UnderSaturation, Disabled
    } ChannelVetoSource;
public:
    VX2750Pha(const char* hostOrPid, bool isUsb = false);
    virtual ~VX2750Pha();
    
    std::string  getCupVersion();
    std::string  getFpgaFWVersion();
    FWType       getFirmwareType();
    std::string  getModelCode();
    std::string  getPiggyBackCode();
    std::string  getModelName();
    FormFactor   getFormFactor();
    std::string  getFamilyCode();
    std::string  getSerialNumber();
    int          getMotherboardRev();
    int          getPiggyBackRev();
    std::string  getLicense();
    bool         isLicesnsed();
    int          remainingUnlicensedTime();
    int          channelCount();
    int          bitsOfResolution();
    int          sampleRate();
    int          inputRange();
    bool         isDifferentialInput();
    int          inputImpedance();
    double       adcOverTemperatureThreshold();
    double       fpgaOverTemperatureThreshold();
    int          ipAddress();
    int          netMask();
    int          gateway();
    ClockSource  getClockSource();
    void         setClockSource(ClockSource);
    bool         isClockOutOnP0();
    void         setClockOutOnP0(bool state);
    bool         isClockOutOnFP();
    void         setClockOutOnFP(bool state);
    StartSource  getStartSource();
    void         setStartSource(StartSource src);
    GlobalTriggerSource getGlobalTriggerSource();
    void         setGlobalTriggerSource(GlobalTriggerSource src);
    WaveTriggerSource getWaveTriggerSource();
    void         setWaveTriggerSource(WaveTriggerSource src):
    TimestampResetSource getTimestampResetSource();
    void setTimestampResetSource(TimestampResetSource);
    std::uint64_t getChannelTriggerMask();
    void          setChannelTriggerMask(uint64_t mask);
    TraceRecordMode getTraceRecordMode();
    void           setTraceRecordMode(TraceRecordMode mode);
    TRGOUTMode    getTRGOUTMode();
    void          setTRGOUTMode(TRGOUTMode mode);
    GPIOMode      getGPIOMode();
    void          setGPIOMode(GPIOMode mode);
    BuyInSource   getBusyInputSource();
    void          setBusyInputSource(BusyInSource src);
    SyncOutMode   getSyncOutMode();
    void          setSyncOutMode(SyncOutMode mode);
    VetoSource    getBoardVetoSource();
    void          setBoardVetoSource(VetoSource src);
    std::unit64_t   getBoardVetoWidth();
    void          setBoardVetoWidth(std::uint64_t ns);
    VetoPolarity  getBoardVetoPolarity();
    void          setBoardVetoPolarity(VetoPolarity pol);
    std::unit32_t getRunDelay();
    void          setRunDelay(std::uint32_t ns);
    bool          isAutoDisarmEnabled();
    void          setAutoDisarm(bool enable);
    bool          isMultiWindowRunEnabled();
    void          setMultiWindowRunEnable(bool enable);
    bool          isPauseTimestampHoldEnabled();
    void          setPauseTimestampHold(bool enable);
    
    ChannelVetoSource getChannelVetoSource(unsigned chan);
    void          setChannelVetoSource(unsigned chan, ChannelVetoSource, src);
    std::unit32_t getChannelVetoWidth(unsigned chan);
    void          setChannelVetoWidth(unsigned chan, std::uint32_t ns);
private:
    std::string  getNetworkInfo();
    
};
}

#endif