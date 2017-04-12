/*
 * gpio_interface.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: Iulian Gheorghiu
 */
#include <api/gpio.h>
#include <stdlib.h>
#include <string.h>

#include "driver/ioc.h"
#include "inc/hw_ioc.h"
#include "inc/hw_types.h"
#include "driver/prcm.h"
#include "driver/sys_ctrl.h"

extern CfgGpio gpioCfg[];

/*#####################################################*/
/*#####################################################*/
GI::Dev::Gpio::Gpio(unsigned int pin, CfgGpio::gpioMode_e mode, bool multiPin)
{
	memset(this, 0, sizeof(*this));
	cfg.pin = pin;
	cfg.gpioMode = mode;
	cfg.multiPin = multiPin;
	setMode(mode);
	setOut(cfg.defValue);
}

GI::Dev::Gpio::Gpio(CfgGpio *gpioPin)
{
	memset(this, 0, sizeof(*this));
	memcpy(&this->cfg, gpioPin, sizeof(CfgGpio));
	setMode(cfg.gpioMode);
	//setOut(cfg.defValue);
}

/*#####################################################*/
GI::Dev::Gpio::~Gpio()
{
	IOCPortConfigureSet((cfg.pin % 32), IOC_PORT_GPIO, IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_DISABLE);
	HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.pin % 32));
}
/*#####################################################*/
/*#####################################################*/
SysErr GI::Dev::Gpio::setOut(unsigned int value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	if (cfg.multiPin)
	{
		HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = (HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) & ~(cfg.pin % 32))
				| (value & (cfg.pin % 32));
	}
	else
	{
		unsigned int state = value;
		if (cfg.reverse)
			state = (~state) & 0x01;
		if(state)
			HWREG(GPIO_BASE + GPIO_O_DOUTSET31_0) = 1 << (cfg.pin % 32);
		else
			HWREG(GPIO_BASE + GPIO_O_DOUTCLR31_0) = 1 << (cfg.pin % 32);
	}
	return SYS_ERR_OK;
}
/*#####################################################*/
signed int GI::Dev::Gpio::in()
{
	if (!this)
		return -1;
	if (cfg.multiPin)
	{
		return HWREG(GPIO_BASE + GPIO_O_DIN31_0) & (cfg.pin % 32);
	}
	else
	{
		if (cfg.reverse)
		{
			if(HWREG(GPIO_BASE + GPIO_O_DIN31_0) & (1 << (cfg.pin % 32)))
				return false;
			else
				return true;
		}
		else
			return (HWREG(GPIO_BASE + GPIO_O_DIN31_0) >> (cfg.pin % 32)) & 0x01;
	}
}
/*#####################################################*/
SysErr GI::Dev::Gpio::getIn(unsigned long *value)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	*value = (unsigned int)in();
	return SYS_ERR_OK;
}
/*#####################################################*/
SysErr GI::Dev::Gpio::setMode(CfgGpio::gpioMode_e mode)
{
	if (!this)
		return SYS_ERR_INVALID_HANDLER;
	unsigned long pin_func = 0;
	switch (mode)
	{
	case CfgGpio::GPIO_IN_FLOATING:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.pin % 32));
		break;
	case CfgGpio::GPIO_IN_PULL_DOWN:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_DOWN | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.pin % 32));
		break;
	case CfgGpio::GPIO_IN_PULL_UP:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) &= ~(1 << (cfg.pin % 32));
		break;
	case CfgGpio::GPIO_OUT_OPEN_DRAIN:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_IOPULL_UP | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_OPEN_DRAIN_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << (cfg.pin % 32);
		break;
	case CfgGpio::GPIO_OUT_PUSH_PULL:
		pin_func = (IOC_CURRENT_2MA | IOC_STRENGTH_AUTO | IOC_NO_IOPULL | IOC_SLEW_DISABLE | IOC_HYST_DISABLE | IOC_NO_EDGE | IOC_INT_DISABLE | IOC_IOMODE_NORMAL | IOC_NO_WAKE_UP | IOC_INPUT_ENABLE );
		HWREG(GPIO_BASE + GPIO_O_DOE31_0) |= 1 << (cfg.pin % 32);
		break;
		break;
	default:
		return SYS_ERR_INVALID_COMMAND;

	}
	return SYS_ERR_OK;
}
/*#####################################################*/
bool GI::Dev::Gpio::getState()
{
	return lastState;
}
/*#####################################################*/
void GI::Dev::Gpio::idle()
{
	if (in() == 0 && lastState == true)
	{
		lastState = false;
		events.stateDown = true;
		events.stateChanged = true;
		if (events.callback.onStateChanged)
			events.callback.onStateChanged(
					events.callback.onStateChangedHandler, false);
		if (events.callback.onDown)
			events.callback.onDown(
					events.callback.onDownHandler);
	}
	else if (in() != 0 && lastState == false)
	{
		lastState = true;
		events.stateUp = true;
		events.stateChanged = true;
		if (events.callback.onStateChanged)
			events.callback.onStateChanged(
					events.callback.onStateChangedHandler, true);
		if (events.callback.onUp)
			events.callback.onUp(
					events.callback.onUpHandler);
	}
}
/*#####################################################*/