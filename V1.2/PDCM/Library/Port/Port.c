/******************************************************************************
*
* Module: Port
*
* File Name: Port.c
*
* Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
*
* Author: Mohamed Tarek
******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
|| (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
  || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif


#endif
STATIC const Port_ConfigChannel * Port_PortChannels = NULL_PTR;
STATIC uint8 Port_Status =PORT_NOT_INITIALIZED;

/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* check if the input configuration pointer is not a NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_Init_SID,PORT_E_PARAM_CONFIG);
  }
  else
#endif
    /*variable to save the statues of port initializing*/
    Port_Status=PORT_INITIALIZED;
  /*pointer to the constructor of configured channels*/
  Port_PortChannels=ConfigPtr->Channels;
  
  /*pointer to save the port which will be operation will occur on*/
  volatile uint32* PortGPIO_Ptr=NULL_PTR;
  /*delay for clock enabling for a port*/
  volatile uint32 delay=0;
  /*the channels looper*/
  volatile uint8 i=0;
  /*looping on the configured channels*/
  for(i=0;i<PORT_CONFIGURED_CHANNLES ;i++)
  {
    /*saving the port address in the pointer depending on the configured channel*/
    switch(Port_PortChannels[i].port_num)
    {
    case PortConfig_PORTA_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
      break;
    case PortConfig_PORTB_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
      break;
    case PortConfig_PORTC_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
      break;
    case PortConfig_PORTD_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
      break;
    case PortConfig_PORTE_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
      break;
    case PortConfig_PORTF_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
      break;
    }
    /*entering clock to the port*/
    SYSCTL_REGCGC2_REG |= (1<<Port_PortChannels[i].port_num);
    /*polling till the clock is established for the port*/
    delay = SYSCTL_REGCGC2_REG;
    if( ((Port_PortChannels[i].port_num == 3) && (Port_PortChannels[i].pin_num == 7)) || ((Port_PortChannels[i].port_num == 5) && (Port_PortChannels[i].pin_num == 0)) ) /* PD7 or PF0 */
    {
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[i].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (Port_PortChannels[i].port_num == 2) && (Port_PortChannels[i].pin_num <= 3) ) /* PC0 to PC3 */
    {
      continue;
    }
    else
    {
      /* Do Nothing ... No need to unlock the commit register for this pin */
    }
    
    /*
    *ALGORITHM
    *check analog
    *if not the rest of modes is digital
    *check if digital
    *if yes clear the PCTL and AFSEL
    *else
    *enter swicth case
    *couple Output direction with inital data in data register
    *couple Input direction with the internal resistance
    */
    
    /*checking if the mode is analog*/
    if(Port_PortChannels[i].pin_mode==PortConfig_Mode_ADC)
    {
      /*setting the analog mode for the configured channel if it's mode is analog*/
      SET_BIT((*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ), Port_PortChannels[i].pin_num);
      /*clearing the digital mode*/
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,Port_PortChannels[i].pin_num );
    }
    else
    {
      /*else the rest of modes is digital */
      CLEAR_BIT((*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET)) ,Port_PortChannels[i].pin_num );
      SET_BIT((*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET)) ,Port_PortChannels[i].pin_num );
    }
    if(Port_PortChannels[i].pin_mode==PortConfig_Mode_DIO)
    {
      /*if the mode is digital the AFSEL for that configured pin will be disabled, no need to clear the PCTL here*/
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,Port_PortChannels[i].pin_num );
    }
    else
    {
      /*else the AFSEL will be set and the PCTL will be updated in the next swicth case*/
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,Port_PortChannels[i].pin_num );
    }
    
    switch(Port_PortChannels[i].pin_mode)
    {
    case PortConfig_Mode_ADC:
      /*do nothing*/
      break;
      
    case PortConfig_Mode_DIO:
      /*clear the PCTL*/
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[i].pin_num * 4));
      break;
      
    case PortConfig_Mode_UART:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_UART << (Port_PortChannels[i].pin_num * 4));
      break;
      
    case PortConfig_Mode_SSI:
      if((Port_PortChannels[i].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[i].pin_num<=PortConfig_PIN3_NUM))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_UART << (Port_PortChannels[i].pin_num * 4));
      }
      else
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_SSI<< (Port_PortChannels[i].pin_num * 4));
      }
      break;
      
    case PortConfig_Mode_I2C:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_I2C<< (Port_PortChannels[i].pin_num * 4));
      break;
      
    case PortConfig_Mode_M0PWM:
      if(((Port_PortChannels[i].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[i].pin_num<=PortConfig_PIN2_NUM))||((Port_PortChannels[i].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[i].pin_num<=PortConfig_PIN6_NUM))||((Port_PortChannels[i].port_num==PortConfig_PORTF_NUM)&&(Port_PortChannels[i].pin_num<=PortConfig_PIN2_NUM)))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_M0FAULT << (Port_PortChannels[i].pin_num * 4));
      }
      else
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_M0PWM << (Port_PortChannels[i].pin_num * 4));
      }
      break;
    case PortConfig_Mode_M1PWM:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_M1PWM << (Port_PortChannels[i].pin_num * 4));
      break;
    case PortConfig_Mode_IDX:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_IDX << (Port_PortChannels[i].pin_num * 4));
      break;      
    case PortConfig_Mode_TIMER:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_TIMER<< (Port_PortChannels[i].pin_num * 4));
      break;
    case PortConfig_Mode_CAN:
      
      if(((Port_PortChannels[i].port_num==PortConfig_PORTA_NUM)&&((Port_PortChannels[i].port_num==PortConfig_PIN0_NUM)||(Port_PortChannels[i].port_num==PortConfig_PIN1_NUM)))||
         ((Port_PortChannels[i].port_num==PortConfig_PORTB_NUM)&&((Port_PortChannels[i].port_num==PortConfig_PIN4_NUM)||(Port_PortChannels[i].port_num==PortConfig_PIN5_NUM)))||
           ((Port_PortChannels[i].port_num==PortConfig_PORTE_NUM)&&((Port_PortChannels[i].port_num==PortConfig_PIN4_NUM)||(Port_PortChannels[i].port_num==PortConfig_PIN5_NUM))))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_CAN<< (Port_PortChannels[i].pin_num * 4));
      }
      else if(((Port_PortChannels[i].port_num==PortConfig_PORTC_NUM)&&((Port_PortChannels[i].port_num==PortConfig_PIN6_NUM)||(Port_PortChannels[i].port_num==PortConfig_PIN7_NUM)))||
              ((Port_PortChannels[i].port_num==PortConfig_PORTD_NUM)&&((Port_PortChannels[i].port_num==PortConfig_PIN2_NUM)||(Port_PortChannels[i].port_num==PortConfig_PIN3_NUM)))||
                ((Port_PortChannels[i].port_num==PortConfig_PORTF_NUM)&&((Port_PortChannels[i].port_num==PortConfig_PIN4_NUM))))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_USB<< (Port_PortChannels[i].pin_num * 4));
      }
      else if (((Port_PortChannels[i].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[i].port_num==PortConfig_PIN7_NUM))||\
        ((Port_PortChannels[i].port_num==PortConfig_PORTF_NUM)&&(Port_PortChannels[i].port_num==PortConfig_PIN0_NUM)))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_NMI<< (Port_PortChannels[i].pin_num * 4));
      }
      else
      {
        /*do nothing*/
      }
      break;
    }
    
    
    
    if(Port_PortChannels[i].direction==OUTPUT)
    {
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[i].pin_num);
      if(Port_PortChannels[i].initial_value==STD_HIGH)
      {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DATA_REG_OFFSET),Port_PortChannels[i].pin_num);
      }
      else
      {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DATA_REG_OFFSET),Port_PortChannels[i].pin_num);
      }
      
    }
    else if (Port_PortChannels[i].direction==INPUT)
    {
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[i].pin_num);
      if(Port_PortChannels[i].resistor==PULL_UP)
      {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_PULL_UP_REG_OFFSET ),Port_PortChannels[i].pin_num);
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_PULL_DOWN_REG_OFFSET ),Port_PortChannels[i].pin_num);
      }
      else if(Port_PortChannels[i].resistor==PULL_DOWN)
      {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_PULL_DOWN_REG_OFFSET ),Port_PortChannels[i].pin_num);
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_PULL_UP_REG_OFFSET ),Port_PortChannels[i].pin_num);
      }
      else
      {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_PULL_UP_REG_OFFSET ),Port_PortChannels[i].pin_num);
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_PULL_DOWN_REG_OFFSET ),Port_PortChannels[i].pin_num);
      }
    }
    else
    {
      /*do nothing*/
    }
    
  }
  
}




/************************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin          - Port Pin ID number
Direction    - Port Pin Direction 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction
************************************************************************************/
#if(PORT_SET_PIN_DIRECTION==STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
  boolean error=FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* Check If Pin Is Correct */
  if (Pin >= PORT_CONFIGURED_CHANNLES)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_PARAM_PIN);
    error = TRUE ;
  }
  else
  {
    /* DO Nothing */
  }
  if (Port_Status == PORT_NOT_INITIALIZED)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_UNINIT);
    error = TRUE ;
  }
  else
  {
    /* Do Nothing */
  }
  if (Port_PortChannels[Pin].direction_changeable == DIRECTION_CHANGE_OFF)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_DIRECTION_UNCHANGEABLE);
    error = TRUE ;
  }
  else
  {
    /* Do Nothing */
  }
#endif
  
  if(error==FALSE)
  {
    uint8 delay=0;
    volatile uint32* PortGPIO_Ptr=NULL_PTR;
    switch(Port_PortChannels[Pin].port_num)
    {
    case PortConfig_PORTA_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
      break;
    case PortConfig_PORTB_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
      break;
    case PortConfig_PORTC_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
      break;
    case PortConfig_PORTD_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
      break;
    case PortConfig_PORTE_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
      break;
    case PortConfig_PORTF_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
      break;
    }
    /*entering clock to the port*/
    SYSCTL_REGCGC2_REG |= (1<<Port_PortChannels[Pin].port_num);
    /*polling till the clock is established for the port*/
    delay = SYSCTL_REGCGC2_REG;
    if( ((Port_PortChannels[Pin].port_num == 3) && (Port_PortChannels[Pin].pin_num == 7)) || ((Port_PortChannels[Pin].port_num == 5) && (Port_PortChannels[Pin].pin_num == 0)) ) /* PD7 or PF0 */
    {
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[Pin].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (Port_PortChannels[Pin].port_num == 2) && (Port_PortChannels[Pin].pin_num <= 3) ) /* PC0 to PC3 */
    {
      return;
    }
    else
    {
      /* Do Nothing ... No need to unlock the commit register for this pin */
    }
    if(Direction==OUTPUT)
    {
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].pin_num);
    }
    else
    {
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].pin_num);
    }
    
  }
  
  
  else
  {
    /*do nothing*/
  }
  
  
  
  
}
#endif







/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None     
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction
************************************************************************************/
void Port_RefreshPortDirection(void)
{
  boolean error=FALSE;
#if(PORT_DEV_ERROR_DETECT == STD_ON)
  {
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
      Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                      PORT_E_UNINIT);
      error = TRUE ;
    }
    else
    {
      /* Do Nothing */
    }
    
  }
#endif
  if(error==FALSE)
  {
    uint8 delay=0;
    volatile uint32* PortGPIO_Ptr=NULL_PTR;
    volatile uint8 i=0;
    /*looping on the configured channels*/
    for(i=0;i<PORT_CONFIGURED_CHANNLES ;i++)
    {
      /*saving the port address in the pointer depending on the configured channel*/
      switch(Port_PortChannels[i].port_num)
      {
      case PortConfig_PORTA_NUM:
        PortGPIO_Ptr=(volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
        break;
      case PortConfig_PORTB_NUM:
        PortGPIO_Ptr=(volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
        break;
      case PortConfig_PORTC_NUM:
        PortGPIO_Ptr=(volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
        break;
      case PortConfig_PORTD_NUM:
        PortGPIO_Ptr=(volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
        break;
      case PortConfig_PORTE_NUM:
        PortGPIO_Ptr=(volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
        break;
      case PortConfig_PORTF_NUM:
        PortGPIO_Ptr=(volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
        break;
      }
      /*entering clock to the port*/
      SYSCTL_REGCGC2_REG |= (1<<Port_PortChannels[i].port_num);
      /*polling till the clock is established for the port*/
      delay = SYSCTL_REGCGC2_REG;
      if( ((Port_PortChannels[i].port_num == 3) && (Port_PortChannels[i].pin_num == 7)) || ((Port_PortChannels[i].port_num == 5) && (Port_PortChannels[i].pin_num == 0)) ) /* PD7 or PF0 */
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[i].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
      }
      else if( (Port_PortChannels[i].port_num == 2) && (Port_PortChannels[i].pin_num <= 3) ) /* PC0 to PC3 */
      {
        continue;
      }
      else
      {
        /* Do Nothing ... No need to unlock the commit register for this pin */
      }
      if(Port_PortChannels[i].direction==OUTPUT)
      {
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[i].pin_num);
      }
      if(Port_PortChannels[i].direction==INPUT)
      {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[i].pin_num);
        
      }
      
      
    }
  }
}










/************************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None     
* Parameters (inout): None
* Parameters (out): versioninfo     -Pointer to where to store the version information of this module. 
* Return value: None
* Description: Returns the version information of this module. 
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* Check if input pointer is not Null pointer */
  if(NULL_PTR == versioninfo)
  {
    /* Report to DET  */
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                    Port_SetPinDirection_SID, PORT_E_PARAM_POINTER);
  }
  else
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Copy the vendor Id */
    versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
    /* Copy the module Id */
    versioninfo->moduleID = (uint16)PORT_MODULE_ID;
    /* Copy Software Major Version */
    versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
    /* Copy Software Minor Version */
    versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
    /* Copy Software Patch Version */
    versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
  }
}

#endif

/************************************************************************************
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin          - Port Pin ID number
Mode         - New Port Pin mode to be set on port pin. 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
  boolean error=FALSE;
  
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* Check If Pin Is Correct */
  if (Pin >= PORT_CONFIGURED_CHANNLES)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_PARAM_PIN);
    error = TRUE ;
  }
  else
  {
    /* DO Nothing */
  }
  if (Port_Status == PORT_NOT_INITIALIZED)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_UNINIT);
    error = TRUE ;
  }
  else
  {
    /* Do Nothing */
  }
  if (Port_PortChannels[Pin].mode_changeable == MODE_CHANGE_OFF)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_DIRECTION_UNCHANGEABLE);
    error = TRUE ;
  }
  else
  {
    /* Do Nothing */
  }
  if(Mode>=PORT_CONFIGURED_MODES)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, Port_SetPinDirection_SID,
                    PORT_E_DIRECTION_UNCHANGEABLE);
    error = TRUE ;
  }
  else
  {
    /* Do Nothing */
  }
#endif
  if(error==FALSE)
  {
    uint8 delay=0;
    volatile uint32* PortGPIO_Ptr=NULL_PTR;
    switch(Port_PortChannels[Pin].port_num)
    {
    case PortConfig_PORTA_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
      break;
    case PortConfig_PORTB_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
      break;
    case PortConfig_PORTC_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
      break;
    case PortConfig_PORTD_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTD_BASE_ADDRESS;
      break;
    case PortConfig_PORTE_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTE_BASE_ADDRESS;
      break;
    case PortConfig_PORTF_NUM:
      PortGPIO_Ptr=(volatile uint32*)GPIO_PORTF_BASE_ADDRESS;
      break;
    }
    /*entering clock to the port*/
    SYSCTL_REGCGC2_REG |= (1<<Port_PortChannels[Pin].port_num);
    /*polling till the clock is established for the port*/
    delay = SYSCTL_REGCGC2_REG;
    if( ((Port_PortChannels[Pin].port_num == 3) && (Port_PortChannels[Pin].pin_num == 7)) || ((Port_PortChannels[Pin].port_num == 5) && (Port_PortChannels[Pin].pin_num == 0)) ) /* PD7 or PF0 */
    {
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[Pin].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (Port_PortChannels[Pin].port_num == 2) && (Port_PortChannels[Pin].pin_num <= 3) ) /* PC0 to PC3 */
    {
      return;
    }
    else
    {
      /* Do Nothing ... No need to unlock the commit register for this pin */
    }
    if(Port_PortChannels[Pin].pin_mode==PortConfig_Mode_ADC)
    {
      /*setting the analog mode for the configured channel if it's mode is analog*/
      SET_BIT((*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ), Port_PortChannels[Pin].pin_num);
      /*clearing the digital mode*/
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,Port_PortChannels[Pin].pin_num );
    }
    else
    {
      /*else the rest of modes is digital */
      CLEAR_BIT((*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET)) ,Port_PortChannels[Pin].pin_num );
      SET_BIT((*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET)) ,Port_PortChannels[Pin].pin_num );
    }
    if(Port_PortChannels[Pin].pin_mode==PortConfig_Mode_DIO)
    {
      /*if the mode is digital the AFSEL for that configured pin will be disabled, no need to clear the PCTL here*/
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,Port_PortChannels[Pin].pin_num );
    }
    else
    {
      /*else the AFSEL will be set and the PCTL will be updated in the next swicth case*/
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,Port_PortChannels[Pin].pin_num );
    }
    
    switch(Port_PortChannels[Pin].pin_mode)
    {
    case PortConfig_Mode_ADC:
      /*do nothing*/
      break;
      
    case PortConfig_Mode_DIO:
      /*clear the PCTL*/
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PortChannels[Pin].pin_num * 4));
      break;
      
    case PortConfig_Mode_UART:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_UART << (Port_PortChannels[Pin].pin_num * 4));
      break;
      
    case PortConfig_Mode_SSI:
      if((Port_PortChannels[Pin].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[Pin].pin_num<=PortConfig_PIN3_NUM))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_UART << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_SSI<< (Port_PortChannels[Pin].pin_num * 4));
      }
      break;
      
    case PortConfig_Mode_I2C:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_I2C<< (Port_PortChannels[Pin].pin_num * 4));
      break;
      
    case PortConfig_Mode_M0PWM:
      if(((Port_PortChannels[Pin].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[Pin].pin_num<=PortConfig_PIN2_NUM))||((Port_PortChannels[Pin].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[Pin].pin_num<=PortConfig_PIN6_NUM))||((Port_PortChannels[Pin].port_num==PortConfig_PORTF_NUM)&&(Port_PortChannels[Pin].pin_num<=PortConfig_PIN2_NUM)))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_M0FAULT << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_M0PWM << (Port_PortChannels[Pin].pin_num * 4));
      }
      break;
    case PortConfig_Mode_M1PWM:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_M1PWM << (Port_PortChannels[Pin].pin_num * 4));
      break;
    case PortConfig_Mode_IDX:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_IDX << (Port_PortChannels[Pin].pin_num * 4));
      break;      
    case PortConfig_Mode_TIMER:
      *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_TIMER<< (Port_PortChannels[Pin].pin_num * 4));
      break;
    case PortConfig_Mode_CAN:
      
      if(((Port_PortChannels[Pin].port_num==PortConfig_PORTA_NUM)&&((Port_PortChannels[Pin].port_num==PortConfig_PIN0_NUM)||(Port_PortChannels[Pin].port_num==PortConfig_PIN1_NUM)))||
         ((Port_PortChannels[Pin].port_num==PortConfig_PORTB_NUM)&&((Port_PortChannels[Pin].port_num==PortConfig_PIN4_NUM)||(Port_PortChannels[Pin].port_num==PortConfig_PIN5_NUM)))||
           ((Port_PortChannels[Pin].port_num==PortConfig_PORTE_NUM)&&((Port_PortChannels[Pin].port_num==PortConfig_PIN4_NUM)||(Port_PortChannels[Pin].port_num==PortConfig_PIN5_NUM))))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_CAN<< (Port_PortChannels[Pin].pin_num * 4));
      }
      else if(((Port_PortChannels[Pin].port_num==PortConfig_PORTC_NUM)&&((Port_PortChannels[Pin].port_num==PortConfig_PIN6_NUM)||(Port_PortChannels[Pin].port_num==PortConfig_PIN7_NUM)))||
              ((Port_PortChannels[Pin].port_num==PortConfig_PORTD_NUM)&&((Port_PortChannels[Pin].port_num==PortConfig_PIN2_NUM)||(Port_PortChannels[Pin].port_num==PortConfig_PIN3_NUM)))||
                ((Port_PortChannels[Pin].port_num==PortConfig_PORTF_NUM)&&((Port_PortChannels[Pin].port_num==PortConfig_PIN4_NUM))))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_USB<< (Port_PortChannels[Pin].pin_num * 4));
      }
      else if (((Port_PortChannels[Pin].port_num==PortConfig_PORTD_NUM)&&(Port_PortChannels[Pin].port_num==PortConfig_PIN7_NUM))||\
        ((Port_PortChannels[Pin].port_num==PortConfig_PORTF_NUM)&&(Port_PortChannels[Pin].port_num==PortConfig_PIN0_NUM)))
      {
        *(volatile uint32 *)((volatile uint8 *)PortGPIO_Ptr + PORT_CTL_REG_OFFSET) |= ((uint32)PortConfig_Mode_NMI<< (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        /*do nothing*/
      }
      break;
    }
    
    
    
  }
}

