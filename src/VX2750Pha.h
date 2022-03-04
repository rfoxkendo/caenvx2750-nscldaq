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
    
    typedef enum _PauseTimestampHandling {
        Hold, Run
    } PauseTimestampHandling;
    
    // Led bit masks:
    
    static const std::uint32_t LED_JESD_Y_PASS = 1;
    static const std::uint32_t LED_JESD_H_PASS = 2;
    static const std::uint32_t LED_DDR4_0_PASS = 4;
    static const std::uint32_t LED_DDR4_1_PASS = 8;
    static const std::uint32_t LED_DDR4_2_PASS = 0x10;
    static const std::uint32_t LEDFP_FAIL      = 0x20;
    static const std::uint32_t LEDFP_NIM       = 0x40;
    static const std::uint32_t LEDFP_TTL       = 0x80;
    static const std::uint32_t LEDFP_DTLOSS    = 0x100;
    static const std::uint32_t LEDFP_DTRDY     = 0x200;
    static const std::uint32_t LEDFP_TRG       = 0x400;
    static const std::uint32_t LEDFP_RUN       = 0x800;
    static const std::uint32_t LEDFP_PLL_LOCK  = 0x1000;
    static const std::uint32_t LEDFP_CLKOUT    = 0x2000;
    static const std::uint32_t LEDFP_CLKIN     = 0x4000;
    static const std::uint32_t LEDFP_USB       = 0x8000;
    static const std::uint32_t LEDFP_SFP_SD    = 0x10000;
    static const std::uint32_t LEDFP_SFP_ACT   = 0x20000;
    static const std::uint32_t LEDFP_act       = 0x40000;
    
    // Acquisition status bit mask:
    
    static const std::uint32_t ACQ_ARMED       = 1;
    static const std::uint32_t ACQ_RUN         = 2;
    static const std::uint32_t ACQ_RUN_MW      = 4;
    static const std::uint32_t ACQ_JESD_CLK_VALID = 8;
    static const std::uint32_t ACQ_BUSY        = 0x10;
    static const std::uint32_t ACQ_PRETRIGGER_READY = 0x20;
    static const std::uint32_t ACQ_LICENSE_FAIL = 0x40;
    
    // Waveform sources:
    
    typedef enum _WaveDataSource {
        ADC_DATA, ADC_TEST_TOGGLE, ADC_TEST_RAMP, ADC_SIN, IPE, Ramp, SquareWave,
        ADC_TEST_PRBS
    } WaveDataSource;
    
    typedef enum _WaveResolution {
        Res8, Res16, Res32, Res64
    } WaveResolution;
    
    typedef enum _AnalogProbe {
        ADCInput, TimeFilter, EnergyFilter, EnergyFilterBaseline,
        EnergyFilterMinusBaseline
    } AnalogProbe;
    
    typedef enum _DigitalProbe {
        Trigger, TimeFilterArmed, ReTriggerGuard, EneryFilterBaselineFreeze,
        EnergyFilterPeaking, EnergyFilterPeakReady, EnergyFilterPileupGuard,
        EventPileUp, ADCSaturation, ADCSaturationProtection, PostSaturationEvent,
        EnergyFilterSaturation, AcquisitionInhibit
    } DigitalProbe;
    
    typedef enum _IOLevel {
        TTL, NIM
    } IOLevel;
    
    // Error bits in the ErrorFlag and ErrorFlagData masks:
    
    static const std::uint32_t ERR_power_fail              = 1;
    static const std::uint32_t ERR_board_init_fault        = 2;
    static const std::uint32_t ERR_si5341_unlock           = 4;
    static const std::uint32_t ERR_si5395_unlock           = 8;
    static const std::uint32_t ERR_LMK04832_unlock         = 0x10;
    static const std::uint32_t ERR_jesd_unlock             = 0x20;
    static const std::uint32_t ERR_ddr_pl_bank0_calib_fail = 0x40;
    static const std::uint32_t ERR_ddr_pl_bank1_calib_fail = 0x80;
    static const std::uint32_t ERR_ddr_ps_calib_fail       = 0x100;
    static const std::uint32_t ERR_fpga_config_fail        = 0x200;
    static const std::uint32_t ERR_bic_error               = 0x400;
    static const std::uint32_t ERR_adc_overtemp            = 0x800;
    static const std::uint32_t ERR_air_overtemp            = 0x1000;
    static const std::uint32_t ERR_fpga_overtemp           = 0x2000;
    static const std::uint32_t ERR_dcdc_overtemp           = 0x4000;
    static const std::uint32_t ERR_clkin_miss              = 0x8000;
    static const std::uint32_t ERR_adc_shutdown            = 0x10000;
    
    // individual Trigger generation values:
    
    typedef enum _IndividualTriggerLogic {
        OR, AND, Majority
    } IndividualTriggerLogic;
    
    typedef enum _PairTriggerLogic {
        AND, OR, NONE
    } PairTriggerLogic;
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
    void          setChannelTriggerMask(std::uint64_t mask);
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
    int          getRunDelay()
    void         setRunDelay(uint32_t ns);
    bool         isAutoDisarmEnabled();
    void         setAutoDisarmEnabled(bool state);
    bool         isMultiWindowRunEnabled();
    void         setMultiWindowRunEnable(bool state);
    PauseTimestampHandling
                  getPauseTimestampHandling();
    void          setPauseTimestampHandling(PauseTimestampHandling how);
    std::uint32_t      getLEDStatus();
    std::uint32_t      getAcquisitionStatus();
    std::uint32_t      getMaxRawDataSize();
    double        getVolatileClockDelay();
    void          setVolatileCLockDelay(double value);
    double        getPermanentClockDelay();
    void          setPermanentClockDelay(double value);
    WaveDataSource getWaveDataSource(unsigned chan);
    void          setWaveDataSource(unsigned chan,, WaveDataSource source);
    std::uint32_t getRecordSamples(unsigned chan);
    void          setRecordSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t getRecordNs(unsigned chan);
    void          setRecordNs(unsigned chan, std::uint32_t ns);
    WaveResolution getWaveResolution(unsigned chan);
    void          setWaveResolution(unsigned chan, WaveResolution resolution);
    AnalogProbe   getAnalogProbe(unsigned chan);
    void          setAnalogProbe(unsigned chan, AnalogProbe probe);
    DigitalProbe  getDigitalProbe(unsigned chan, unsigned probeNum);
    void          setDigitalProbe(
        unsigned chan, unsigned probeNum, DigialProbe probe
    );
    std::uint32_t getPreTriggerSamples(unsigned chan);
    void          setPreTriggerSamples(unsigned chan, std::uint32_t nsamples);
    std::uint32_t getPreTriggerNs(unsigned chan);
    void          setPreTriggerNs(unsigned chan, std::uint32_t ns);
    std::uint32_t getTestPulsePeriod();
    void          setTestPulsePeriod(std::uint32_t ns);
    std::uint32_t getTestPulseWidth();
    void          setTestPulseWidth(std::uint32_t ns);
    std::uint32_t getTestPulseLowLimit();
    void          setTestPulseLowLevel(std::uint32_t counts);
    std::uint32_t getTestPulseHighLevel();
    void          setTestPulseHighLevel(std::uint32_t counts);
    IOLevel       getIOLevel();
    void          setIOLEvel(IOLevel level);
    double        getAirInTemp();
    double        getAirOutTemp();
    double        getCoreTemp();
    double        getFirstADCTemp();         // All in degrees C.
    double        getLastADCTemp();
    double        getHottestADCTemp();
    double        getADC0Temp();
    double        getADC1Temp();
    double        getDCDConverterTemp();
    
    double        getDCDCConverterInputVoltage();
    double        getDCDCConverterOutputVoltage();
    double        getDCDCConverterAmps();
    double        getDCDCConverterHz();
    double        getDCDCConverterDutyCycle();
    
    std::uint32_t getFan1Speed();
    std::uint32_t getFan2Speed();
    
    std::uint32_t getErrorFlagMask();
    std::uint32_t getErrorFlagDataMask();
    
    // Get ErrorFlags - 2.4.13 - need definitions of the enum and maybe
    // there's another parameter that's not properly documented here?
    
    bool          isBoardReady();
    
    IndividualTriggerLogic getITLAMainLogic();
    IndividualTriggerLogic getITLBMainLogic();
    void          setITLAMainLogic(IndividualTrigerLogic sel);
    void          setITLBMainLogic(IndividualTrigerLogic sel);
    unsigned      getITLAMajorityLevel();
    unsigned      getITLBMajorityLevel();
    void          setITLAMajorityLevel(unsigned level);
    void          setITLBMajorityLevel(unsigned level);
    
    PairTriggerLogic getITLAPairLogic();
    PairTriggerLogic getITLBPairLogic();
    void         setITLAPairLogic(PairTriggerLogic sel);
    void         setITLBPairLogic(PairTriggerLogic sel);
    
private:
    std::string  getNetworkInfo();
    std::int32_t     intFromString(const char* str);
    std::uint32_t    unsignedFromString(const char* str);
    double           floatFromString(const char* str);
};
}

#endif