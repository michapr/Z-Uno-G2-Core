#include <SysService.h>
#include "ZUNO_LEDS.h"
#include "ZUNO_Buttons.h"
#include "ZWCCResetLocally.h"
#include "Debug.h"
#include "em_core.h"

ZunoLedMode_t SYSLED_MODES[] = {  {10, 0x55555555},  // SYSLED_LEARN_MODE_INCLUSION
                                        {10, 0x35353535},  // SYSLED_LEARN_MODE_SUBMENU_READY
                                        {20, 0x00FF0F35},  // SYSLED_LEARN_MODE_IDENTITY
                                        {20, 0x00AA00AA},  // SYSLED_LEARN_MODE_WAITING_RST
                                        {100,0x00010001},  // SYSLED_LEARN_MODE_SYSERROR_MEMORY
                                        {100,0x00050005},  // SYSLED_LEARN_MODE_SYSERROR_HW
                                        {100,0x00150015},  // SYSLED_LEARN_MODE_SYSERROR_WDOG
                                        {50, 0x55555555}, // SYSLED_LEARN_MODE_BLINK
                                        };
typedef struct ServiceData_s{
    uint8_t cntrl_mode;
    uint32_t cntrl_timeout;
    uint32_t led_learn_timeout;
    uint32_t led_active_timeout;
}ServiceData_t;
ServiceData_t g_service_data;

__WEAK void zunoSysServiceLedInit(void) {
	Led.addLed(SYSLED_LEARN, SYSLED_MODES, sizeof(SYSLED_MODES)/sizeof(ZunoLedMode_t));
	Led.addLed(SYSLED_ACTIVITY, SYSLED_MODES, sizeof(SYSLED_MODES)/sizeof(ZunoLedMode_t));
}

__WEAK void zunoSysServiceLedOff(uint8_t pin) {
	Led.off(pin);
}

__WEAK void zunoSysServiceLedOn(uint8_t pin) {
	Led.on(pin);
}

__WEAK void zunoSysServiceLedSetMode(uint8_t pin, uint8_t mode) {
	Led.setMode(pin, mode);
}

void SysReconfigLeds(){
    if(g_zuno_sys->p_config->flags & ZUNO_CFGFILE_FLAG_LED_OFF) {
        zunoSysServiceLedOff(SYSLED_ACTIVITY);
    } else {
        if(zunoGetSleepingMode()){
            #ifdef LOGGING_DBG
            //LOGGING_UART.println("*** DEVICE IS SLEEPING");
            #endif
            zunoSysServiceLedOn(SYSLED_ACTIVITY);
        } else {
            zunoSysServiceLedSetMode(SYSLED_ACTIVITY, SYSLED_LEARN_MODE_BLINK);
        }
    }
}
void SysSetLearnLedMode(uint8_t mode, uint32_t timeout){
    #ifdef LOGGING_DBG
    LOGGING_UART.print("***LEARN LED MODE:");
    LOGGING_UART.print(mode);
    LOGGING_UART.print(" TIMEOUT:"); 
    LOGGING_UART.println(timeout, HEX);
    #endif
    zunoSysServiceLedSetMode(SYSLED_LEARN, mode);
    g_service_data.led_learn_timeout = millis()+timeout;
}
void SysStopLearnLed(){
    g_service_data.led_learn_timeout = 0;
    zunoSysServiceLedOff(SYSLED_LEARN);
}
void SysServiceInit(){
	zunoSysServiceLedInit();
    Btn.addButton(SYSBUTTON);
    memset(&g_service_data, 0, sizeof(ServiceData_t));
    g_service_data.cntrl_mode = SYS_SVC_MODE_NORMAL;
    SysReconfigLeds();
}
static void _setSysCntrlState(uint8_t mode, uint32_t timeout){
    g_service_data.cntrl_mode = mode;
    g_service_data.cntrl_timeout = millis()+timeout;
}
void SysServiceTimer(){
    switch(g_service_data.cntrl_mode){
        case SYS_SVC_MODE_NORMAL:
            if(Btn.isTripleClick(SYSBUTTON)){
                zunoStartLearn(SYS_LEARN_TIMEOUT, true);
            }
            if(Btn.isLongClick(SYSBUTTON)){
                _setSysCntrlState(SYS_SVC_MODE_SUBMENU, SUBMENU_TIMEOUT);
                SysSetLearnLedMode(SYSLED_LEARN_MODE_SUBMENU_READY, SUBMENU_TIMEOUT);
            }
            break;
        case SYS_SVC_MODE_SUBMENU:
            if(Btn.isTripleClick(SYSBUTTON)){
                zunoResetLocally();
            }
            if(Btn.isSingleClick(SYSBUTTON)){
                zunoStartLearn(SYS_LEARN_TIMEOUT, false);
            }
            break;
        case SYS_SVC_MODE_LEARN:
            if(Btn.isSingleClick(SYSBUTTON)){
                zunoStartLearn(0xFF, true); // Cancel learn mode
            }
            break;
    }
    if((g_service_data.cntrl_timeout != 0) && 
        (millis() > g_service_data.cntrl_timeout)){
        g_service_data.cntrl_mode = SYS_SVC_MODE_NORMAL;
        g_service_data.cntrl_timeout = 0;
    }
    if((g_service_data.led_learn_timeout != 0) && 
        (millis() > g_service_data.led_learn_timeout)){
        #ifdef LOGGING_DBG
        LOGGING_UART.println("***LEARN LED STOPPED");
        #endif
        SysStopLearnLed();
    }
}
void SysServiceEvent(ZUNOSysEvent_t * ev){
    switch(ev->event){
        case ZUNO_SYS_EVENT_LEARNSTARTED:
            SysSetLearnLedMode(SYSLED_LEARN_MODE_INCLUSION, SYS_LEARN_TIMEOUT * 1000UL);
            _setSysCntrlState(SYS_SVC_MODE_LEARN, SYS_LEARN_TIMEOUT * 1000UL);
            break;
        case ZUNO_SYS_EVENT_LEARNSTATUS:
            g_service_data.cntrl_mode = SYS_SVC_MODE_NORMAL;
            SysStopLearnLed();
            break;
    }
}
void SysServiceSleep(){
    zunoSysServiceLedOff(SYSLED_LEARN);
    zunoSysServiceLedOff(SYSLED_ACTIVITY);
}
void SysServiceWUP(){
    SysReconfigLeds();
}

CORE_irqState_t CORE_EnterAtomic(void) __attribute__ ((alias("CORE_EnterCritical")));
CORE_irqState_t CORE_EnterCritical(void) {
	zunoSysCall(ZUNO_SYSFUNC_ENTER_CRITICAL, 0);
	return (0x0);
}

void CORE_ExitAtomic(CORE_irqState_t irqState) __attribute__ ((alias("CORE_ExitCritical")));
void CORE_ExitCritical(CORE_irqState_t irqState) {
	zunoSysCall(ZUNO_SYSFUNC_EXIT_CRITICAL, 0);
	(void)irqState;
}
