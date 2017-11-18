#include <driverlib/rom.h>
#include <driverlib/vims.h>
#include <driverlib/osc.h>
#include <driverlib/ioc.h>
#include <driverlib/gpio.h>
#include <driverlib/aon_event.h>
#include <driverlib/pwr_ctrl.h>
#include <driverlib/cpu.h>

#define BOARD_IOID_LED_1          IOID_30
#define BOARD_LED_1               (1 << BOARD_IOID_LED_1)
#define BOARD_LED_ALL             (BOARD_LED_1)

#define BOARD_IOID_DIO0           IOID_0
#define BOARD_IOID_DIO1           IOID_1
#define BOARD_IOID_CS             IOID_11
#define BOARD_IOID_TDO            IOID_16
#define BOARD_IOID_TDI            IOID_17
#define BOARD_IOID_DIO12          IOID_12
#define BOARD_IOID_DIO15          IOID_15
#define BOARD_IOID_DIO21          IOID_21
#define BOARD_IOID_DIO22          IOID_22
#define BOARD_IOID_DIO23          IOID_23
#define BOARD_IOID_DIO24          IOID_24
#define BOARD_IOID_DIO25          IOID_25
#define BOARD_IOID_DIO26          IOID_26
#define BOARD_IOID_DIO27          IOID_27
#define BOARD_IOID_DIO28          IOID_28
#define BOARD_IOID_DIO29          IOID_29
#define BOARD_IOD_IOID_6          IOID_6
#define BOARD_IOD_IOID_7          IOID_7

#define BOARD_UNUSED_PINS { \
    BOARD_IOID_DIO0, BOARD_IOID_DIO1, BOARD_IOID_CS, BOARD_IOID_TDO, \
    BOARD_IOID_TDI, BOARD_IOID_DIO12, BOARD_IOID_DIO15, BOARD_IOID_DIO21, \
    BOARD_IOID_DIO22, BOARD_IOID_DIO23, BOARD_IOID_DIO24, BOARD_IOID_DIO25, \
    BOARD_IOID_DIO26, BOARD_IOID_DIO27, BOARD_IOID_DIO28, BOARD_IOID_DIO29, \
    BOARD_IOD_IOID_6, BOARD_IOD_IOID_7, IOID_UNUSED \
  }

void
oscillators_select_if_xosc(void)
{
  /* FIXME auc_wakeup related */

  /* Switch LF clock source to the LF XOSC if required */
  if(OSCClockSourceGet(OSC_SRC_CLK_LF) != OSC_XOSC_LF) {
    OSCClockSourceSet(OSC_SRC_CLK_LF, OSC_XOSC_LF);

    /* Wait for LF clock source to become XOSC_LF */
    while(OSCClockSourceGet(OSC_SRC_CLK_LF) != OSC_XOSC_LF);

    /* Disable the LF clock qualifiers */
    DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL0,
        DDI_0_OSC_CTL0_BYPASS_XOSC_LF_CLK_QUAL_M | DDI_0_OSC_CTL0_BYPASS_RCOSC_LF_CLK_QUAL_M,
        DDI_0_OSC_CTL0_BYPASS_RCOSC_LF_CLK_QUAL_S,
        0x3);
  }

  /* Release the OSC AUX consumer */
  // FIXME 
}

void
lpm_init()
{
  // list_init(modules_list);

  /* Always wake up on any DIO edge detection */
  AONEventMcuWakeUpSet(AON_EVENT_MCU_WU3, AON_EVENT_IO);
}

void
configure_unused_pins(void)
{ 
  uint32_t pins[] = BOARD_UNUSED_PINS;

  uint32_t *pin;

  for(pin = pins; *pin != IOID_UNUSED; pin++) {
    IOCPinTypeGpioInput(*pin);
    IOCIOPortPullSet(*pin, IOC_IOPULL_DOWN);
  }
} 


void
wakeup_handler(void)
{
  /* Turn on the PERIPH PD */
  PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
  while(PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH) != PRCM_DOMAIN_POWER_ON)
    ;
}

void
board_init()
{
  /* Disable global interrupts */
  bool int_disabled = IntMasterDisable();

  /* Turn on relevant PDs */
  wakeup_handler();

  /* Enable GPIO peripheral */
  PRCMPeripheralRunEnable(PRCM_PERIPH_GPIO);

  /* Apply settings and wait for them to take effect */
  PRCMLoadSet();
  while(!PRCMLoadGet())
    ;

  //lpm_register_module(&launchpad_module);

  /* For unsupported peripherals, select a default pin configuration */
  configure_unused_pins();

  /* Initialise the RF switch if present */
  //rf_switch_init();

  /* Re-enable interrupt if initially enabled. */
  if(!int_disabled) {
    IntMasterEnable();
  }
}

void
leds_init(void)
{
  IOCPinTypeGpioOutput(BOARD_IOID_LED_1);

  GPIO_clearMultiDio(BOARD_LED_ALL);
}


int
main(void)
{
#if 0
  // enable flash cache and prefetch 
  VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);
  VIMSConfigure(VIMS_BASE, true, true);

  IntMasterDisable();

  oscillators_select_if_xosc();

  lpm_init();
#endif

  board_init();

  leds_init();

  /*
   * Disable I/O pad sleep mode and open I/O latches in the AON IOC interface
   * This is only relevant when returning from shutdown (which is what froze
   * latches in the first place. Before doing these things though, we should
   * allow software to first regain control of pins
   */
  PowerCtrlIOFreezeDisable();

  /*
   * FIXME : initialize CM3 SysTick
   */

  // FIXME : watchdog init
  // FIXME : uart init

  while(1)
  {
    // FIXME toggle LED 
    GPIO_toggleDio(BOARD_IOID_LED_1);
    CPUdelay(1000000);
  }
  return 0;
}
