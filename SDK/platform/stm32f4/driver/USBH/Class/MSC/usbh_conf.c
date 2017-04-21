/**
  ******************************************************************************
  * @file    FatFs/FatFs_USBDisk/Src/usbh_conf.c
  * @author  MCD Application Team
  * @version V1.3.1
  * @date    09-October-2015
  * @brief   USB Host configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/USBH/core/inc/usbh_core.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_gpio_ex.h"

#if defined(STM32F405xx)
  #include "include/stm32f405xx.h"
#elif defined(STM32F415xx)
  #include "include/stm32f415xx.h"
#elif defined(STM32F407xx)
  #include "include/stm32f407xx.h"
#elif defined(STM32F417xx)
  #include "include/stm32f417xx.h"
#elif defined(STM32F427xx)
  #include "include/stm32f427xx.h"
#elif defined(STM32F437xx)
  #include "include/stm32f437xx.h"
#elif defined(STM32F429xx)
  #include "include/stm32f429xx.h"
#elif defined(STM32F439xx)
  #include "include/stm32f439xx.h"
#elif defined(STM32F401xC)
  #include "include/stm32f401xc.h"
#elif defined(STM32F401xE)
  #include "include/stm32f401xe.h"
#elif defined(STM32F410Tx)
  #include "include/stm32f410tx.h"
#elif defined(STM32F410Cx)
  #include "include/stm32f410cx.h"
#elif defined(STM32F410Rx)
  #include "include/stm32f410rx.h"
#elif defined(STM32F411xE)
  #include "include/stm32f411xe.h"
#elif defined(STM32F446xx)
  #include "include/stm32f446xx.h"
#elif defined(STM32F469xx)
  #include "include/stm32f469xx.h"
#elif defined(STM32F479xx)
  #include "include/stm32f479xx.h"
#else
 #error "Please select first the target STM32F4xx device used in your application (in stm32f4xx.h file)"
#endif

HCD_HandleTypeDef hhcd;

#define HOST_POWERSW_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()
#define HOST_POWERSW_PORT                  GPIOB
#define HOST_POWERSW_VBUS                  GPIO_PIN_2
#define OTG_FS1_POWER_SWITCH_PORT      		GPIOB
#define OTG_FS1_POWER_SWITCH_PIN       		GPIO_PIN_4


/*******************************************************************************
                       HCD BSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
  /* Note: On STM32F4-Discovery board only USB OTG FS core is supported. */
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  if(hhcd->Instance == USB_OTG_FS)
  {
    /* Configure USB FS GPIOs */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HOST_POWERSW_CLK_ENABLE();
    
    /* Configure DM DP Pins */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;    
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
    
    /* This for ID line debug */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
    
    /* Configure Power Switch Vbus Pin */
    GPIO_InitStruct.Pin = HOST_POWERSW_VBUS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(HOST_POWERSW_PORT, &GPIO_InitStruct);
    
    /* Enable USB FS Clocks */ 
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    
    /* Set USBFS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
    
    /* Enable USBFS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  } 
}
/**
  * @brief  DeInitializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspDeInit(HCD_HandleTypeDef *hhcd)
{
  if(hhcd->Instance == USB_OTG_FS)
  {  
    /* Disable USB FS Clocks */ 
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
  }
}

/*******************************************************************************
 LL Driver Callbacks (HCD -> USB Host Library)
 *******************************************************************************/

/**
 * @brief  SOF callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
	USBH_LL_IncTimer((USBH_HandleTypeDef*) hhcd->pData);
}

/**
 * @brief  Connect callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
	USBH_LL_Connect((USBH_HandleTypeDef*) hhcd->pData);
}

/**
 * @brief  Disconnect callback.
 * @param  hhcd: HCD handle
 * @retval None
 */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
	USBH_LL_Disconnect((USBH_HandleTypeDef*) hhcd->pData);
}

/**
 * @brief  Notify URB state change callback.
 * @param  hhcd: HCD handle
 * @param  chnum: Channel number
 * @param  urb_state: URB State
 * @retval None
 */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum,
		HCD_URBStateTypeDef urb_state)
{
	/* To be used with OS to sync URB state with the global state machine */
}

/*******************************************************************************
 LL Driver Interface (USB Host Library --> HCD)
 *******************************************************************************/
/**
 * @brief  USBH_LL_Init
 *         Initialize the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
#ifdef USE_USB_FS
	/* Set the LL Driver parameters */
	hhcd.Instance = USB_OTG_FS;
	hhcd.Init.Host_channels = 11;
	hhcd.Init.dma_enable = 0;
	hhcd.Init.low_power_enable = 0;
	hhcd.Init.phy_itface = HCD_PHY_EMBEDDED;
	hhcd.Init.Sof_enable = 0;
	hhcd.Init.speed = HCD_SPEED_FULL;
	hhcd.Init.vbus_sensing_enable = 0;
	hhcd.Init.lpm_enable = 0;

	/* Link the driver to the stack */
	hhcd.pData = phost;
	phost->pData = &hhcd;

	/* Initialize the LL Driver */
	HAL_HCD_Init(&hhcd);
#endif

#ifdef USE_USB_HS
	/* Set the LL driver parameters */
	hhcd.Instance = USB_OTG_HS;
	hhcd.Init.Host_channels = 11;
	hhcd.Init.dma_enable = 0;
	hhcd.Init.low_power_enable = 0;

#ifdef USE_USB_HS_IN_FS
	hhcd.Init.phy_itface = HCD_PHY_EMBEDDED;
#else
	hhcd.Init.phy_itface = HCD_PHY_ULPI;
#endif
	hhcd.Init.Sof_enable = 0;
	hhcd.Init.speed = HCD_SPEED_HIGH;
	hhcd.Init.vbus_sensing_enable = 0;
	hhcd.Init.use_external_vbus = 1;
	hhcd.Init.lpm_enable = 0;

	/* Link the driver to the stack */
	hhcd.pData = phost;
	phost->pData = &hhcd;

	/* Initialize the LL driver */
	HAL_HCD_Init(&hhcd);

#endif /*USE_USB_HS*/
	USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&hhcd));

	return USBH_OK;
}

/**
 * @brief  De-Initializes the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
	HAL_HCD_DeInit((HCD_HandleTypeDef*) phost->pData);
	return USBH_OK;
}

/**
 * @brief  Starts the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
	HAL_HCD_Start((HCD_HandleTypeDef*) phost->pData);
	return USBH_OK;
}

/**
 * @brief  Stops the Low Level portion of the Host driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
	HAL_HCD_Stop((HCD_HandleTypeDef*) phost->pData);
	return USBH_OK;
}

/**
 * @brief  Returns the USB Host Speed from the Low Level Driver.
 * @param  phost: Host handle
 * @retval USBH Speeds
 */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
	USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

	switch (HAL_HCD_GetCurrentSpeed((HCD_HandleTypeDef*) phost->pData))
	{
	case 0:
		speed = USBH_SPEED_HIGH;
		break;

	case 1:
		speed = USBH_SPEED_FULL;
		break;

	case 2:
		speed = USBH_SPEED_LOW;
		break;

	default:
		speed = USBH_SPEED_FULL;
		break;
	}
	return speed;
}

/**
 * @brief  Resets the Host Port of the Low Level Driver.
 * @param  phost: Host handle
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
	HAL_HCD_ResetPort((HCD_HandleTypeDef*) phost->pData);
	return USBH_OK;
}

/**
 * @brief  Returns the last transferred packet size.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @retval Packet Size
 */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
	return HAL_HCD_HC_GetXferCount((HCD_HandleTypeDef*) phost->pData, pipe);
}

/**
 * @brief  Opens a pipe of the Low Level Driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @param  epnum: Endpoint Number
 * @param  dev_address: Device USB address
 * @param  speed: Device Speed
 * @param  ep_type: Endpoint Type
 * @param  mps: Endpoint Max Packet Size
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe,
		uint8_t epnum, uint8_t dev_address, uint8_t speed, uint8_t ep_type,
		uint16_t mps)
{
	HAL_HCD_HC_Init((HCD_HandleTypeDef*) phost->pData, pipe, epnum, dev_address,
			speed, ep_type, mps);
	return USBH_OK;
}

/**
 * @brief  Closes a pipe of the Low Level Driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
	HAL_HCD_HC_Halt((HCD_HandleTypeDef*) phost->pData, pipe);
	return USBH_OK;
}

/**
 * @brief  Submits a new URB to the low level driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 *          This parameter can be a value from 1 to 15
 * @param  direction: Channel number
 *          This parameter can be one of these values:
 *           0: Output
 *           1: Input
 * @param  ep_type: Endpoint Type
 *          This parameter can be one of these values:
 *            @arg EP_TYPE_CTRL: Control type
 *            @arg EP_TYPE_ISOC: Isochronous type
 *            @arg EP_TYPE_BULK: Bulk type
 *            @arg EP_TYPE_INTR: Interrupt type
 * @param  token: Endpoint Type
 *          This parameter can be one of these values:
 *            @arg 0: PID_SETUP
 *            @arg 1: PID_DATA
 * @param  pbuff: pointer to URB data
 * @param  length: length of URB data
 * @param  do_ping: activate do ping protocol (for high speed only)
 *          This parameter can be one of these values:
 *           0: do ping inactive
 *           1: do ping active
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe,
		uint8_t direction, uint8_t ep_type, uint8_t token, uint8_t* pbuff,
		uint16_t length, uint8_t do_ping)
{
	HAL_HCD_HC_SubmitRequest((HCD_HandleTypeDef*) phost->pData, pipe, direction,
			ep_type, token, pbuff, length, do_ping);
	return USBH_OK;
}

/**
 * @brief  Gets a URB state from the low level driver.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 *          This parameter can be a value from 1 to 15
 * @retval URB state
 *          This parameter can be one of these values:
 *            @arg URB_IDLE
 *            @arg URB_DONE
 *            @arg URB_NOTREADY
 *            @arg URB_NYET
 *            @arg URB_ERROR
 *            @arg URB_STALL
 */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost,
		uint8_t pipe)
{
	return (USBH_URBStateTypeDef) HAL_HCD_HC_GetURBState(
			(HCD_HandleTypeDef*) phost->pData, pipe);
}

/**
 * @brief  Drives VBUS.
 * @param  phost: Host handle
 * @param  state: VBUS state
 *          This parameter can be one of these values:
 *           0: VBUS Active
 *           1: VBUS Inactive
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
#ifdef USE_USB_FS

	if(state == 0)
	{
		/* Configure Low Charge pump */
		HAL_GPIO_WritePin(OTG_FS1_POWER_SWITCH_PORT, OTG_FS1_POWER_SWITCH_PIN, GPIO_PIN_RESET);
	}
	else
	{
		/* Drive High Charge pump */
		HAL_GPIO_WritePin(OTG_FS1_POWER_SWITCH_PORT, OTG_FS1_POWER_SWITCH_PIN, GPIO_PIN_SET);
	}

#endif

#ifdef USE_USB_HS_IN_FS
	if(state == 0)
	{
		/* Configure Low Charge pump */
		HAL_GPIO_WritePin(OTG_FS2_POWER_SWITCH_PIN, GPIO_PIN_RESET);
	}
	else
	{
		/* Drive High Charge pump */
		HAL_GPIO_WritePin(OTG_FS2_POWER_SWITCH_PIN, GPIO_PIN_SET);
	}
#endif
	HAL_Delay(200);
	return USBH_OK;
}

/**
 * @brief  Sets toggle for a pipe.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @param  toggle: toggle (0/1)
 * @retval USBH Status
 */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe,
		uint8_t toggle)
{
	if (hhcd.hc[pipe].ep_is_in)
	{
		hhcd.hc[pipe].toggle_in = toggle;
	}
	else
	{
		hhcd.hc[pipe].toggle_out = toggle;
	}
	return USBH_OK;
}

/**
 * @brief  Returns the current toggle of a pipe.
 * @param  phost: Host handle
 * @param  pipe: Pipe index
 * @retval toggle (0/1)
 */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
	uint8_t toggle = 0;

	if (hhcd.hc[pipe].ep_is_in)
	{
		toggle = hhcd.hc[pipe].toggle_in;
	}
	else
	{
		toggle = hhcd.hc[pipe].toggle_out;
	}
	return toggle;
}

/**
 * @brief  Delay routine for the USB Host Library
 * @param  Delay: Delay in ms
 * @retval None
 */
void USBH_Delay(uint32_t Delay)
{
#if (USBH_USE_OS == 1)
	osDelay(Delay);
#else
	HAL_Delay(Delay);
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
