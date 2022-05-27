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
#include <map>
#include <cstdint>
#include <json/json.h>

namespace caen_nscldaq {
/**
 * VX2750PHA - support for parameters in the VX2750.
 *             Since this derives from the Dig2Device class low level
 *             parameter access is also provided.
 */
class VX2750Pha : public Dig2Device
{
public:
    typedef enum _FwType {
        DPP_PHA, DPP_ZLE, DPP_PSD, DPP_DAQ, DPP_OPEN, Scope
    } FwType;
    typedef enum _FormFactor {
        VME, VME64X, DT
    } FormFactor;
    
    typedef enum _ClockSource {
        Internal, FrontPanel, Clock_P0, Link, DIPSelected
    } ClockSource;
    
    typedef enum _StartSource {
        Start_EncodedClockIn, SINLevel, SINEdge, SWCommand, Start_LVDS, Start_P0
    } StartSource;
    
    typedef enum _GlobalTriggerSource {
        GlobalTrigger_TriggerIn, GlobalTrigger_P0,
        GlobalTrigger_Software, GlobalTrigger_LVDS, GlobalTrigger_InternalA,
        GlobalTrigger_InternalB, GlobalTrigger_InternalAandB,
        GlobalTrigger_InternalAorB, GlobalTrigger_EncodedClockIn, GlobalTrigger_GPIO,
        GlobalTrigger_TestPulse
    } GlobalTriggerSource;
    
    typedef enum _WaveTriggerSource {
        WaveTrigger_InternalA, WaveTrigger_InternalB,
        WaveTrigger_GlobalTriggerSource, WaveTrigger_TRGIN, ExternalInhibit,
        ADCUnderSaturation, ADCOverSaturation,
        WaveTrigger_Software, WaveTrigger_ChannelSelfTrigger,
        WaveTrigger_AnyChannelSelfTrigger, WaveTrigger_Disabled
    } WaveTriggerSource;
    
    typedef enum _EventTriggerSource {
        EventTrigger_InternalB, EventTrigger_InternalA,
        EventTrigger_GlobalTriggerSource, EventTrigger_TRGIN,
        EventTrigger_Software, EventTrigger_ChannelSelfTrigger,
        EventTrigger_AnyChannelSelfTrigger, EventTrigger_Disabled
    } EventTriggerSource;
    
    typedef enum _TimestampResetSource {
        TimestampReset_Start, Timestamp_SIN, TimestampReset_GPIO,
        TimestampReset_EncodedClockIn
    } TimestampResetSource;
    
    typedef enum _TraceRecordMode {
        Always, OnRequest
    } TraceRecordMode;
    
    typedef enum _TRGOUTMode {
        TriggerOut_TRGIN, TriggerOut_P0,
        TriggerOut_Software, TriggerOut_LVDS, TriggerOut_InternalA,
        TriggerOut_InternalB, TriggerOut_InternalAandInternalB,
        TriggerOut_InternalAorInternalB, TriggerOut_EncodedClockIn,
        TriggerOut_Run, TriggerOut_ReferenceClock, TriggerOut_TestPulse,
        TriggerOut_Busy, TriggerOut_Zero,
        TriggerOut_One, TriggerOut_SynchIn, TriggerOut_SIN, TriggerOut_GPIO,
        AcceptedTrigger, TriggerClock
    } TRGOUTMode;
    
    typedef enum _GPIOMode {
        GPIOMode_Disabled, GPIOMode_TriggerIn, GPIOMode_P0, GPIOMode_SIN, GPIOMode_LVDS, GPIOMode_InternalA,
        GPIOMode_InternalB, GPIOMode_InternalAandInternalB,
        GPIOMode_InternalAorInternalB, GPIOMode_EncodedClockIn,
        GPIOMode_SoftwareTrigger, GPIOMode_Run, GPIOMode_ReferenceClock,
        GPIOMode_TestPulse, GPIOMode_Busy, GPIOMode_Zero, GPIOMode_One
    } GPIOMode;
    
    typedef enum _BusyInSource {
        BusyIn_SIN, BusyIn_GPIO, BusyIn_LVDS, BusyIn_Disabled 
    } BusyInSource;
    
    typedef enum _SyncOutMode {
        SyncOut_Disabled, SyncOut_SynchIn, SyncOut_TestPulse, InternalClock, SyncOut_Run
    } SyncOutMode;
    
    typedef enum _VetoSource {
        Veto_SIN, Veto_LVDS, Veto_GPIO, Veto_P0, Veto_EncodedClock, Veto_Disabled
    } VetoSource;
    
    
    typedef enum _VetoPolarity {
        ActiveHigh, ActiveLow
    } VetoPolarity;
    
    typedef enum _ChannelVetoSource {
        BoardVeto, OverSaturation, UnderSaturation, ChanVeto_Disabled
    } ChannelVetoSource;
    
    typedef enum _PauseTimestampHandling {
        Hold, PauseTstamp_Run
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
        ADC_DATA, ADC_TEST_TOGGLE, ADC_TEST_RAMP, ADC_SIN, WaveSource_IPE,
        WaveSource_Ramp, SquareWave,
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
        DProbe_Trigger, TimeFilterArmed, ReTriggerGuard, EneryFilterBaselineFreeze,
        EnergyFilterPeaking, EnergyFilterPeakReady, EnergyFilterPileupGuard,
        EventPileup, ADCSaturation, ADCSaturationProtection, PostSaturationEvent,
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
        ITL_OR, ITL_AND, Majority
    } IndividualTriggerLogic;
    
    typedef enum _PairTriggerLogic {
        PTL_AND, PTL_OR, NONE
    } PairTriggerLogic;
    
    typedef enum _ITLConnect {
        ITL_Disabled, ITL_ITLA, ITL_ITLB
    } ITLConnect;
    
    typedef enum _LVDSMode {
        SelfTriggers, Sync, IORegister
    } LVDSMode;
    typedef enum _LVDSDirection {
        Input, Output
    } LVDSDirection;
    
    typedef enum _DACOutputMode {
        Static, DACOut_IPE, DACOut_ChInput, MemOccupancy, ChSum, OverThrSum,
        DACOut_Ramp, Sine5MHz, Square
    } DACOutputMode;
    
    static const std::uint32_t CHST_DelayInitdone = 1;
    static const std::uint32_t CHST_TFInitDone    = 2;
    static const std::uint32_t CHST_EFInitDone    = 4;
    static const std::uint32_t CHST_FullInitDone  = 8;
    static const std::uint32_t CHST_Unused1       = 0x10;
    static const std::uint32_t CHST_AcqEnabled    = 0x20;
    static const std::uint32_t CHST_InnerActive   = 0x40;
    static const std::uint32_t CHST_CanWriteTE    = 0x80;
    static const std::uint32_t CHST_CanWriteWF    = 0x100;
    
    typedef enum _Polarity {
        Positive, Negative
    } Polarity;
    
    typedef enum _EventSelection {
        All, Pileup, EnergySkim
    } EventSelection;
    
    typedef enum _CoincidenceMask {
        Coincidence_Disabled, Ch64Trigger, Coincidence_TRGIN, Coincidence_GlobalTriggerSource,
        Coincidence_ITLA, Coincidence_ITLB
    } CoincidenceMask;
    
    typedef enum _EnergyPeakingAverage {
        Average1, Average4, EPeakAvg_Average16, EPeakAvg_Average64
    } EnergyPeakingAverage;
    
    typedef enum _EnergyFilterBaselineAverage {
        Fixed, EFilterBlineAvg_Average16, EFilterBlineAvg_Average64,
        Average256, Average1024, Average4K, Average16K
    } EnergyFilterBaselineAverage;
    
    typedef enum _Endpoint {
        Raw, PHA
    } Endpoint;

    
    // Struct to hold decoded data. The user can select which fields are filled in:
    
    typedef struct _DecodedEvent {
        std::uint8_t   s_channel;               // Always present.
        std::uint64_t  s_nsTimestamp;           // Always present.
        std::uint64_t  s_rawTimestamp;          // s_enableRawTimestamps 
        std::uint16_t  s_fineTimestamp;         // s_enableFineTimestamps
        std::uint16_t  s_energy;                // Always present.
        std::uint16_t  s_lowPriorityFlags;      // s_enableFlags
        std::uint16_t  s_highPriorityFlags;     // s_enableFlags
        std::uint8_t   s_timeDownSampling;      // s_enableDownSampledTime.
        std::int32_t*  s_pAnalogProbe1;         // s_enableAnalogProbe1
        std::uint8_t   s_analogProbe1Type;      // s_enableAnalogProbe1
        std::int32_t*  s_pAnalogProbe2;         // s_enableAnalogProbe2
        std::uint8_t   s_analogProbe2Type;      // s_enableAnalogProbe2
        std::uint8_t*  s_pDigitalProbe1;        // s_enableDigitalProbe1
        std::uint8_t   s_digitalProbe1Type;     // s_enableDigitalProbe1
        std::uint8_t*  s_pDigitalProbe2;        // s_enableDigitalProbe2
        std::uint8_t   s_digitalProbe2Type;     // s_enableDigitalProbe2
        std::uint8_t*  s_pDigitalProbe3;        // s_enableDigitalProbe3
        std::uint8_t   s_digitalProbe3Type;     // s_enableDigitalProbe3
        std::uint8_t*  s_pDigitalProbe4;        // s_enableDigitalProbe4
        std::uint8_t   s_digitalProbe4Type;      // s_enableDigitalProbe4
        size_t         s_samples;               // s_enableSampleCount
        bool           s_fail;                  // Always present.
        size_t         s_eventSize;             // s_enableEventSize
        
    } DecodedEvent, *pDecodedEvent;
    
    // this struct is used to store the initial enables for optional
    // fields of the data from the DPP-PHA endpoint.
    // It's used to generate the JSON required to configure the
    // dpp-pha endpoint.
    struct EnabledItems {
        //                      channel is always on.
        bool s_enableRawTimestamps;
        //                      ns timestamp always enabled.
        bool s_enableFineTimestamps;
        //                      energy always enabled.
        bool s_enableFlags;     // Enables all flags.
        bool s_enableDownsampledTime;
        bool s_enableAnalogProbe1;
        bool s_enableAnalogProbe2;
        bool s_enableDigitalProbe1;
        bool s_enableDigitalProbe2;
        bool s_enableDigitalProbe3;
        bool s_enableDigitalProbe4;
        bool s_enableSampleCount;
        //                       fail always enabled.
        bool s_enableEventSize;
        // constructor:
        EnabledItems() {
            resetOptions();
        }
        void resetOptions() {
            s_enableRawTimestamps = false;
            s_enableFineTimestamps = false;
            s_enableFlags    = false;
            s_enableDownsampledTime = false;;
            s_enableAnalogProbe1 = false;;
            s_enableAnalogProbe2 = false;
            s_enableDigitalProbe1 = false;
            s_enableDigitalProbe2 = false;
            s_enableDigitalProbe3 = false;
            s_enableDigitalProbe4 = false;
            s_enableSampleCount   = false;
            s_enableEventSize      = false;
        }
    };
    // These are useful for string based configuration modules.
    
    static std::map<std::string, VX2750Pha::ClockSource> stringToClockSource;
    static std::map<std::string, VX2750Pha::StartSource> stringToStartSource;
    static std::map<std::string, VX2750Pha::GlobalTriggerSource> stringToGlobalTriggerSource;
    static std::map<std::string, VX2750Pha::WaveTriggerSource> stringToWaveTrigger;
    static std::map<std::string, VX2750Pha::EventTriggerSource> stringToEventTrigger;
    static std::map<std::string, VX2750Pha::TimestampResetSource> stringToTimestampReset;
    static std::map<std::string, VX2750Pha::TRGOUTMode> stringToTRGOUT;
    static std::map<std::string, VX2750Pha::GPIOMode>  stringToGPIO;
    static std::map<std::string, VX2750Pha::BusyInSource> stringToBusyIn;
    static std::map<std::string, VX2750Pha::SyncOutMode> stringToSyncOut;
    static std::map<std::string , VX2750Pha::VetoSource>  stringToVeto;
    static std::map<std::string, VX2750Pha::VetoPolarity> stringToVetoPolarity;
    static std::map<std::string, VX2750Pha::ChannelVetoSource> stringToChannelVeto;
    // InternalData.
private:
    EnabledItems  m_dppPhaOptions;
public:
    VX2750Pha(const char* hostOrPid, bool isUsb = false);
    virtual ~VX2750Pha();
    
private:
    VX2750Pha(const VX2750Pha&);
    VX2750Pha& operator=(const VX2750Pha& );
    int operator==(const VX2750Pha&);
    int operator!=(const VX2750Pha&);
public:
    
    std::string  getCupVersion();     // Get the CUP version
    std::string  getFpgaFWVersion();  // Get the firmware Version
    FwType       getFirmwareType();   // Get the type of the firmware.
    std::string  getModelCode();
    std::string  getPiggyBackCode();
    std::string  getModelName();
    FormFactor   getFormFactor();
    int          getFamilyCode();
    std::string  getSerialNumber();
    int          getMotherboardRev();
    int          getPiggybackRev();
    std::string  getLicense();
    bool         isLicensed();
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
    WaveTriggerSource getWaveTriggerSource(unsigned ch);
    void         setWaveTriggerSource(unsigned ch, WaveTriggerSource src);
    EventTriggerSource getEventTriggerSource(unsigned ch);
    void         setEventTriggerSource(unsigned ch, EventTriggerSource src);
    TimestampResetSource getTimestampResetSource();
    void setTimestampResetSource(TimestampResetSource);
    std::uint64_t getChannelTriggerMask(unsigned ch);
    void          setChannelTriggerMask(unsigned ch, std::uint64_t mask);
    TraceRecordMode getTraceRecordMode(unsigned ch);
    void           setTraceRecordMode(unsigned ch, TraceRecordMode mode);
    TRGOUTMode    getTRGOUTMode();
    void          setTRGOUTMode(TRGOUTMode mode);
    GPIOMode      getGPIOMode();
    void          setGPIOMode(GPIOMode mode);
    BusyInSource   getBusyInputSource();
    void          setBusyInputSource(BusyInSource src);
    SyncOutMode   getSyncOutMode();
    void          setSyncOutMode(SyncOutMode mode);
    VetoSource    getBoardVetoSource();
    void          setBoardVetoSource(VetoSource src);
    std::uint64_t   getBoardVetoWidth();
    void          setBoardVetoWidth(std::uint64_t ns);
    VetoPolarity  getBoardVetoPolarity();
    void          setBoardVetoPolarity(VetoPolarity pol);
    ChannelVetoSource getChannelVetoSource(unsigned chan);
    void          setChannelVetoSource(unsigned chan, ChannelVetoSource src);
    std::uint32_t getChannelVetoWidth(unsigned chan);
    void          setChannelVetoWidth(unsigned chan, std::uint32_t ns);
    std::uint32_t getRunDelay();
    void         setRunDelay(std::uint32_t ns);
    bool         isAutoDisarmEnabled();
    void         setAutoDisarmEnabled(bool state);
    bool         isMultiWindowRunEnabled();
    void         setMultiWindowRunEnable(bool state);
    bool          isPauseTimestampHoldEnabled();
    void          setPauseTimestampHold(bool enable);
    
    std::uint32_t      getLEDStatus();
    std::uint32_t      getAcquisitionStatus();
    std::uint32_t      getMaxRawDataSize();
    double        getVolatileClockDelay();
    void          setVolatileClockDelay(double value);
    double        getPermanentClockDelay();
    void          setPermanentClockDelay(double value);
    WaveDataSource getWaveDataSource(unsigned chan);
    void          setWaveDataSource(unsigned chan, WaveDataSource source);
    std::uint32_t getRecordSamples(unsigned chan);
    void          setRecordSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t getRecordNs(unsigned chan);
    void          setRecordNs(unsigned chan, std::uint32_t ns);
    WaveResolution getWaveResolution(unsigned chan);
    void          setWaveResolution(unsigned chan, WaveResolution resolution);
    AnalogProbe   getAnalogProbe(unsigned chan, unsigned probeNum);
    void          setAnalogProbe(unsigned chan, unsigned probeNum, AnalogProbe probe);
    DigitalProbe  getDigitalProbe(unsigned chan, unsigned probeNum);
    void          setDigitalProbe(
        unsigned chan, unsigned probeNum, DigitalProbe probe
    );
    std::uint32_t getPreTriggerSamples(unsigned chan);
    void          setPreTriggerSamples(unsigned chan, std::uint32_t nsamples);
    std::uint32_t getPreTriggerNs(unsigned chan);
    void          setPreTriggerNs(unsigned chan, std::uint32_t ns);
    std::uint32_t getTestPulsePeriod();
    void          setTestPulsePeriod(std::uint32_t ns);
    std::uint32_t getTestPulseWidth();
    void          setTestPulseWidth(std::uint32_t ns);
    std::uint32_t getTestPulseLowLevel();
    void          setTestPulseLowLevel(std::uint32_t counts);
    std::uint32_t getTestPulseHighLevel();
    void          setTestPulseHighLevel(std::uint32_t counts);
    IOLevel       getIOLevel();
    void          setIOLevel(IOLevel level);
    double        getAirInTemp();
    double        getAirOutTemp();
    double        getCoreTemp();
    double        getFirstADCTemp();         // All in degrees C.
    double        getLastADCTemp();
    double        getHottestADCTemp();
    double        getADCTemp(unsigned chip);
    double        getDCDCConverterTemp();
    
    double        getDCDCConverterInputVoltage();
    double        getDCDCConverterOutputVoltage();
    double        getDCDCConverterAmps();
    double        getDCDCConverterHz();
    double        getDCDCConverterDutyCycle();
    
    std::uint32_t getFanSpeed(unsigned fan);
    
    // Bit encoded errors per ERR_* bit definitions.
    
    std::uint32_t getErrorFlagMask();
    void          setErrorFlagMask(std::uint32_t mask);
    std::uint32_t getErrorFlagDataMask();
    void          setErrorFlagDataMask(std::uint32_t mask);
    std::uint32_t getErrorFlags();
    
    bool          isBoardReady();
    
    IndividualTriggerLogic getITLAMainLogic();
    IndividualTriggerLogic getITLBMainLogic();
    void          setITLAMainLogic(IndividualTriggerLogic sel);
    void          setITLBMainLogic(IndividualTriggerLogic sel);
    unsigned      getITLAMajorityLevel();
    unsigned      getITLBMajorityLevel();
    void          setITLAMajorityLevel(unsigned level);
    void          setITLBMajorityLevel(unsigned level);
    
    PairTriggerLogic getITLAPairLogic();
    PairTriggerLogic getITLBPairLogic();
    void         setITLAPairLogic(PairTriggerLogic sel);
    void         setITLBPairLogic(PairTriggerLogic sel);
    bool          isITLAInverted();
    bool          isITLBInverted();
    void          setITLAInverted(bool invert);
    void          setITLBInverted(bool invert);
    ITLConnect    getITLConnect(unsigned ch);
    void          setITLConnect(unsigned ch, ITLConnect selection);
    
    std::uint64_t getITLAMask();
    std::uint64_t getITLBMask();
    
    void          setITLAMask(std::uint64_t mask);
    void          setITLBMask(std::uint64_t mask);
    std::uint32_t getITLAGateWidth();
    std::uint32_t getITLBGateWidth();
    void          setITLAGateWidth(std::uint32_t ns);
    void          setITLBGateWidth(std::uint32_t ns);
    
    LVDSMode      getLVDSMode(unsigned quartet);
    void          setLVDSMode(unsigned quartet, LVDSMode mode);
    LVDSDirection getLVDSDirection(unsigned quartet);
    void          setLVDSDirection(unsigned quartet, LVDSDirection direction);
    std::uint16_t getLVDSIOReg(unsigned quartet);
    void          setLVDSIOReg(unsigned quartet, std::uint16_t mask);
    std::uint64_t getLVDSTriggerMask(unsigned inputNum);
    void          setLVDSTriggerMask(unsigned inputNum, std::uint64_t mask);
    
    void setDACOutMode(DACOutputMode mode);
    DACOutputMode getDACOutMode();
    std::uint16_t getDACOutValue();
    void          setDACOutValue(std::uint16_t value);
    unsigned      getDACChannel();
    void          setDACChannel(unsigned chan);
    
    
    
    double        getVGAGain(unsigned group);
    void          setVGAGain(unsigned group, double value);
    
    bool          isOffsetCalibrationEnabled(unsigned chan);
    void          enableOffsetCalibration(unsigned chan, bool enable);
    bool          isChannelEnabled(unsigned chan);
    void          enableChannel(unsigned chan, bool enable);
    int           getSelfTriggerRate(unsigned chan);
    std::uint32_t getChannelStatus(unsigned chan);
    double        getDCOffset(unsigned chan);
    void          setDCOffset(unsigned chan, double pct);
    double        getGainFactor(unsigned chan);
    std::uint32_t getTriggerThreshold(unsigned chan);
    void          setTriggerThreshold(unsigned chan, std::uint32_t threshold);
    Polarity      getPulsePolarity(unsigned chan);
    void          setPulsePolarity(unsigned chan, Polarity pol);
    
    std::uint16_t getEnergySkimLowDiscriminator(unsigned chan);
    void          setEnergySkimLowDiscriminator(unsigned chan, std::uint16_t value);
    std::uint16_t getEnergySkimHighDiscriminator(unsigned chan);
    void          setEnergySkimHighDiscriminator(unsigned chan, std::uint16_t value);
    
    EventSelection getEventSelector(unsigned chan);
    void           setEventSelector(unsigned chan, EventSelection sel);
    EventSelection getWaveformSelector(unsigned chan);
    void           setWaveformSelector(unsigned chan, EventSelection sel);
    CoincidenceMask getCoincidenceMask(unsigned chan);
    void           setCoincidenceMask(unsigned chan, CoincidenceMask sel);
    CoincidenceMask getAntiCoincidenceMask(unsigned chan);
    void           setAntiCoincidenceMask(unsigned chan, CoincidenceMask sel);
    
    std::uint32_t  getCoincidenceSamples(unsigned chan);
    void           setCoincidenceSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t  getCoincidenceNs(unsigned chan);
    void           setCoincidenceNs(unsigned chan, std::uint32_t ns);
    
    std::uint32_t  getTimeFilterRiseTime(unsigned chan);
    std::uint32_t  getTimeFilterRiseSamples(unsigned chan);
    void           setTimeFilterRiseTime(unsigned chan, std::uint32_t ns);
    void           setTimeFilterRiseSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t  getTimeFilterRetriggerGuardTime(unsigned chan);
    std::uint32_t  getTimeFilterRetriggerGuardSamples(unsigned chan);
    void           setTimeFilterRetriggerGuardTime(unsigned chan, std::uint32_t ns);
    void           setTimeFilterRetriggerGuardSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t  getEnergyFilterRiseTime(unsigned chan);
    std::uint32_t  getEnergyFilterRiseSamples(unsigned chan);
    void           setEnergyFilterRiseTime(unsigned chan, std::uint32_t ns);
    void           setEnergyFilterRiseSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t  getEnergyFilterFlatTopTime(unsigned chan);
    std::uint32_t  getEnergyFilterFlatTopSamples(unsigned chan);
    void           setEnergyFilterFlatTopTime(unsigned chan, std::uint32_t ns);
    void           setEnergyFilterFlatTopSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t  getEnergyFilterPeakingPosition(unsigned chan);
    void           setEnergyFilterPeakingPosition(unsigned chan, std::uint32_t pct);
    EnergyPeakingAverage getEnergyFilterPeakingAverage(unsigned chan);
    void          setEnergyFilterPeakingAverage(unsigned chan, EnergyPeakingAverage sel);
    std::uint32_t getEnergyFilterPoleZeroTime(unsigned chan);
    std::uint32_t getEnergyFilterPoleZeroSamples(unsigned chan);
    void          setEnergyFilterPoleZeroTime(unsigned chan, std::uint32_t ns);
    void          setEnergyFilterPoleZeroSamples(unsigned chan, std::uint32_t samples);
    double        getEnergyFilterFineGain(unsigned chan);
    void          setEnergyFilterFineGain(unsigned chan, double gain);
    bool          isEnergyFilterFLimitationEnabled(unsigned chan);
    void          enableEnergyFilterFLimitation(unsigned chan, bool enable);
    EnergyFilterBaselineAverage getEnergyFilterBaselineAverage(unsigned chan);
    void          setEnergyFilterBaselineAverage(unsigned chan, EnergyFilterBaselineAverage sel);
    std::uint32_t getEnergyFilterBaselineGuardTime(unsigned chan);
    std::uint32_t getEnergyFilterBaselineGuardSamples(unsigned chan);
    void          setEnergyFilterBaselineGuardTime(unsigned chan, std::uint32_t ns);
    void          setEnergyFilterBaselineGuardSamples(unsigned chan, std::uint32_t samples);
    std::uint32_t getEnergyFilterPileupGuardTime(unsigned chan);
    std::uint32_t getEnergyFilterPileupGuardSamples(unsigned chan);
    void          setEnergyFilterPileupGuardTime(unsigned chan, std::uint32_t ns);
    void          setEnergyFilterPileupGuardSamples(unsigned chan, std::uint32_t samples);
    
    std::uint8_t  getEnergyBits(unsigned chan);
    std::uint64_t getRealtime(unsigned chan);
    std::uint64_t getDeadtime(unsigned chan);
    
    // Commands:
    
    void          Reset();
    void          Clear();
    void          Arm();
    void          Disarm();
    void          Start();
    void          Stop();
    void          Trigger();
    void          ReloadCalibration();
    
    // Endpoint management:
    
    Endpoint      getEndpoint();
    void          selectEndpoint(Endpoint selection);
    
    // Raw endpoint - note that when reading raw waveforms we always read
    // both the size and data fields.
    
    void   initializeRawEndpoint();          // In case someone changes the json.
    size_t readRawEndpoint(void* pBuffer);
    
    // We're going to try to hide that awful JSON crap inside our class
    // Some items will be mandatory, others not and are off by default
    // but can be turned on/off programatically
    // Mandatory:  channel, nsTimestamp, energy, fail
    //
    void setDefaultFormat();
    void enableRawTimestamp(bool enable);
    void enableFineTimestamp(bool enable);
    void enableFlags(bool enable);
    void enableDownsampling(bool enable);
    void enableAnalogProbes(bool probe1, bool probe2);
    void enableDigitalProbes(bool probe1, bool probe2, bool probe3, bool probe4);
    void enableSampleSize(bool enable);
    void  enableRawEventSize(bool enable);
    
    // This sends the JSON:
    
    void initializeDPPPHAReadout();
    void readDPPPHAEndpoint(DecodedEvent& event);
    
    // Buffer management for the formatted (DPPPHAEndpoint):
    
    void initDecodedBuffer(DecodedEvent& event);
    void setupDecodedBuffer(DecodedEvent& event);
    void freeDecodedBuffer(DecodedEvent& event);
    
    
private:
    uint32_t    dottedToInt(const std::string& dotted);
    template<class T> std::string enumToString(const std::map<T, std::string>& map, T value);
    template<class T> T stringToEnum(const std::map<std::string, T>& map, const std::string& value);
    bool textToBool(const std::string& str);
    void checkInclusiveRange(int low, int high, int value);
    std::string appendNumber(const char* base, unsigned number);
    Json::Value createScalar(const char* name, const char* type);
    Json::Value createArray(const char* name, const char* type, unsigned dimension);
  
};

// Implementation of template methods:
//  We use enums where possible so we can get compile time checking of
//  values, therefore the main thing we need to do that's templated is
//  conversions from enumerated values -> strings and back:

/**
 * enumToString
 *    Given an enumerated value return the associated string.
 *    This is templated on the key of the conversion mapping.
 * @param map  - Map of enumerated value -> strings.
 * @param value - Value in the key type of the map
 * @return std::string  - mapped string value.
 * @throw std::invalid_argument - lookup failure
 * @note it is possible for a lookup failure to occur given C/C++'s ability to
 *       treat enumerated values like integers including being able to perform
 *       arithmetic on them DON'T DO THIS, but we protect anyway.
 */
template<class T>
std::string
VX2750Pha::enumToString(const std::map<T, std::string>&map, T value)
{
    auto p = map.find(value);
    if (p == map.end()) {
        throw std::invalid_argument("Invalid lookup key passed to enumToString");
    }
    return p->second;
}
/**
 * stringToEnum
 *    Given a  string looks up an enumeraged value.
 *    Yes I could really implement both with two templated arguments
 *    but I'm too stupid to do that, and this provides
 *    a meaningful pair of method names which that would not
 *
 * @param map - mapping from string->enum.
 * @param value - String value to look up.
 * @return T.
 * @throw std::invalid_argument if there's no mapping for the string
 * @note If the map has been constructed properly, there's less chance of throw
 *       than in the previous method because the 'hardware's returning the
 *       strings that are getting tossed into this.
 */
template<class T>
T
VX2750Pha::stringToEnum(const std::map<std::string, T>& map, const std::string& value)
{
    auto p = map.find(value);
    if (p == map.end()) {
        // We can also be a bit more detailed in our message:
        
        std::string msg("Invalid lookup key passed to enumToString : ");
        msg += value;
        throw std::invalid_argument(msg);
    }
    return p->second;
}

}                                 // namespace.

#endif
