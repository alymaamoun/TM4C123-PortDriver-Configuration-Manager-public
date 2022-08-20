/******************************************************************************
*
* Module: Port
*
* File Name: Port_PBCfg.C
*
* Description: Prebuild configure source file for TM4C123GH6PM Microcontroller - Port Driver.
*
* Author: Aly Mohamed Maamoun
******************************************************************************/

#include "Port.h"


/*
*Module version 1.0.0
*/
#define PORT_PBCFG_SW_MAJOR_VERSION (1U)
#define PORT_PBCFG_SW_MINOR_VERSION (0U)
#define PORT_PBCFG_SW_PACTH_VERSION (0U)

/*
*AUTOSAR version 4.0.3
*/
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION (3U)

/* AUTOSAR Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
  ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between PORT_PBcfg.c and PORT.h files */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
  ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of PBcfg.c does not match the expected version"
#endif

const Port_ConfigType Port_Configuration={
/*automated*/
};

/* Description: Structure to configure each individual PIN:
*	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
*	2. the number of the pin in the PORT.
*      3. the direction of pin --> INPUT or OUTPUT
*      4. the internal resistor --> Disable, Pull up or Pull down
*      5. initial value of the pin
*      6. the mode of the pin --> ADC,DIO,UART,SSI,I2C,PWM,PHB,IDX,T2CCP,WTCCP,CAN,USB,NMI,C,TRD
*      7. pin mode changeability --> MODE_CHANGE_OFF or  MODE_CHANGE_ON
*      8. pin direction changeability --> DIRECTION_CHANGE_OFF or DIRECTION_CHANGE_ON
*/