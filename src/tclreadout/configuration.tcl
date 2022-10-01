v27xxpha create adc1

# Configure the adc to read from the pulser:
# this is based on the exampe in caen_dig2 lib pha:

# Trigger configuration:

v27xxpha config adc1 gbltriggersrc TestPulse \
    wavetriggersrc [lrepeat 64 GlobalTriggerSource] \
    eventtriggersrc [lrepeat 64 GlobalTriggerSource] \
    testpulseperiod 100000000 testpulsewidth 32

v27xxpha config adc1 testpulselowlevel 0x0 testpulsehighlevel 0x2000


# Wave/probe  configuration (note by default the analog/digital probes are not
# read those have to be turned on in the configuration)

v27xxpha config adc1 wavesource [lrepeat 64 SquareWave]

v27xxpha config adc1 recordsamples [lrepeat 64 512]  \
    waveresolutions [lrepeat 64 Res8] \
    analogprobe1 [lrepeat 64 ADCInput]  analogprobe2 [lrepeat 64 EnergyFilterMinusBaseline] \
    digitalprobe1 [lrepeat 64 Trigger] digitalprobe2 [lrepeat 64 TimeFilterArmed] \
    digitalprobe3 [lrepeat 64 EnergyFilterBaselineFreeze] digitalprobe4 [lrepeat 64 EnergyFilterPeakReady]
v27xxpha config adc1 pretriggersamples [lrepeat 64 20]

# Filter and conditioning parameters

v27xxpha config adc1 dcoffsets [lrepeat 64 0]
v27xxpha config adc1 tfrisetime [lrepeat 64 10] efrisetime [lrepeat 64 100] \
    efflattoptime [lrepeat 64 100] triggerthresholds [lrepeat 64 100] \
    efpeakingpos [lrepeat 64 80] efpolezero [lrepeat 64 1000] \
    tfretriggerguard [lrepeat 64 10] efpileupguardt [lrepeat 64 10] \
    efbaselineguardt [lrepeat 64 400] inputpolarities [lrepeat 64 Positive] \
    
    
v27xxpha config adc1 eflflimitation [lrepeat 64 false] \
    efbaselineavg [lrepeat 64 256] effinegain [lrepeat 64 1.0]


# Put analog probe 1 in the event.

v27xxpha config adc1 readanalogprobes [list off off];   
    


