 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Configure file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Aly Mohamed Maamoun
 ******************************************************************************/
 
#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
* module version: 1.0.0
*/
#define PORT_CFG_SW_MAJOR_VERSION (1U)
#define PORT_CFG_SW_MINOR_VERSION (0U)
#define PORT_CFG_SW_PATCH_VERSION (0U)

/*
* AUTOSAR VERSION 4.0.3
*/

#define PORT_CFG_AR_RELEASE_MAJOR_VERSION (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                   (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                   (STD_ON)

/* Pre-compile option for presence of port_FlipChannel API */
#define PORT_SET_PIN_DIRECTION                  (STD_ON)

/* Number of the configured Port Channels */
#define PORT_CONFIGURED_CHANNLES                (43U)

/*automated*/

/* ports configuring with id */

#define PortConfig_PORTA_NUM (uint8)0
#define PortConfig_PORTB_NUM (uint8)1
#define PortConfig_PORTC_NUM (uint8)2
#define PortConfig_PORTD_NUM (uint8)3
#define PortConfig_PORTE_NUM (uint8)4
#define PortConfig_PORTF_NUM (uint8)5

/*pins configuring with id */

#define PortConfig_PIN0_NUM (uint8)0
#define PortConfig_PIN1_NUM (uint8)1
#define PortConfig_PIN2_NUM (uint8)2
#define PortConfig_PIN3_NUM (uint8)3
#define PortConfig_PIN4_NUM (uint8)4
#define PortConfig_PIN5_NUM (uint8)5
#define PortConfig_PIN6_NUM (uint8)6
#define PortConfig_PIN7_NUM (uint8)7

/* Modes configuration with id based on GPIO modes distribution*/

#define PortConfig_Mode_ADC 		(uint8)9
#define PortConfig_Mode_DIO 		(uint8)0
#define PortConfig_Mode_UART 		(uint8)1
#define PortConfig_Mode_SSI 		(uint8)2
#define PortConfig_Mode_I2C 		(uint8)3
#define PortConfig_Mode_M0PWM 		(uint8)4
#define PortConfig_Mode_M0FAULT 	(uint8)4
#define PortConfig_Mode_M1PWM 		(uint8)5
#define PortConfig_Mode_IDX 		(uint8)6
#define PortConfig_Mode_TIMER 		(uint8)7
#define PortConfig_Mode_CAN 		(uint8)8
#define PortConfig_Mode_USB 		(uint8)8
#define PortConfig_Mode_NMI 		(uint8)8

/*configuring pins with ports*/

/*PORT A*/
#define PortConfig_PORTA_PIN0_ID_INDEX (uint8)0x00
#define PortConfig_PORTA_PIN1_ID_INDEX (uint8)0x01
#define PortConfig_PORTA_PIN2_ID_INDEX (uint8)0x02
#define PortConfig_PORTA_PIN3_ID_INDEX (uint8)0x03
#define PortConfig_PORTA_PIN4_ID_INDEX (uint8)0x04
#define PortConfig_PORTA_PIN5_ID_INDEX (uint8)0x05
#define PortConfig_PORTA_PIN6_ID_INDEX (uint8)0x06
#define PortConfig_PORTA_PIN7_ID_INDEX (uint8)0x07

/*port B*/

#define PortConfig_PORTB_PIN0_ID_INDEX (uint8)0x08
#define PortConfig_PORTB_PIN1_ID_INDEX (uint8)0x09
#define PortConfig_PORTB_PIN2_ID_INDEX (uint8)0x0A
#define PortConfig_PORTB_PIN3_ID_INDEX (uint8)0x0B
#define PortConfig_PORTB_PIN4_ID_INDEX (uint8)0x0C
#define PortConfig_PORTB_PIN5_ID_INDEX (uint8)0x0D
#define PortConfig_PORTB_PIN6_ID_INDEX (uint8)0x0E
#define PortConfig_PORTB_PIN7_ID_INDEX (uint8)0x0F


/*PORT C*/
#define PortConfig_PORTC_PIN0_ID_INDEX (uint8)0x10
#define PortConfig_PORTC_PIN1_ID_INDEX (uint8)0x11
#define PortConfig_PORTC_PIN2_ID_INDEX (uint8)0x12
#define PortConfig_PORTC_PIN3_ID_INDEX (uint8)0x13
#define PortConfig_PORTC_PIN4_ID_INDEX (uint8)0x14
#define PortConfig_PORTC_PIN5_ID_INDEX (uint8)0x15
#define PortConfig_PORTC_PIN6_ID_INDEX (uint8)0x16
#define PortConfig_PORTC_PIN7_ID_INDEX (uint8)0x17

/*port D*/

#define PortConfig_PORTD_PIN0_ID_INDEX (uint8)0x18
#define PortConfig_PORTD_PIN1_ID_INDEX (uint8)0x19
#define PortConfig_PORTD_PIN2_ID_INDEX (uint8)0x1A
#define PortConfig_PORTD_PIN3_ID_INDEX (uint8)0x1B
#define PortConfig_PORTD_PIN4_ID_INDEX (uint8)0x1C
#define PortConfig_PORTD_PIN5_ID_INDEX (uint8)0x1D
#define PortConfig_PORTD_PIN6_ID_INDEX (uint8)0x1E
#define PortConfig_PORTD_PIN7_ID_INDEX (uint8)0x1F
/*PORT E*/
#define PortConfig_PORTE_PIN0_ID_INDEX (uint8)0x20
#define PortConfig_PORTE_PIN1_ID_INDEX (uint8)0x21
#define PortConfig_PORTE_PIN2_ID_INDEX (uint8)0x22
#define PortConfig_PORTE_PIN3_ID_INDEX (uint8)0x23
#define PortConfig_PORTE_PIN4_ID_INDEX (uint8)0x24
#define PortConfig_PORTE_PIN5_ID_INDEX (uint8)0x25


/*port F*/

#define PortConfig_PORTF_PIN0_ID_INDEX (uint8)0x26
#define PortConfig_PORTF_PIN1_ID_INDEX (uint8)0x27
#define PortConfig_PORTF_PIN2_ID_INDEX (uint8)0x28
#define PortConfig_PORTF_PIN3_ID_INDEX (uint8)0x29
#define PortConfig_PORTF_PIN4_ID_INDEX (uint8)0x2A

#define LOW STD_OFF
#define HIGH STD_ON


#endif 