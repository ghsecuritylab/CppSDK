/*
 * CLK.c
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */
#include <avr/io.h>
#include "core_init.h"
#include <sys/systime.h>
/**********************************************************************************************
* External objects
**********************************************************************************************/


/**********************************************************************************************
* Global variables
**********************************************************************************************/
unsigned long FCPU = CORE_CLOCK_DEFAULT;

/**********************************************************************************************
* Constants and macros
**********************************************************************************************/


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/


/**********************************************************************************************
* Local variables
**********************************************************************************************/


/**********************************************************************************************
* Local functions
**********************************************************************************************/


static void SystemClock_Config(unsigned long int_osc_freq, unsigned long ext_osc_freq, unsigned long core_freq)
{
	if(ext_osc_freq)
	{
		PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
		CCP = CCP_IOREG_gc;
		OSC.CTRL &= ~(OSC_RC2MEN_bm | OSC_RC32MEN_bm);
#if defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega32E5__)
		OSC.CTRL &= ~OSC_RC8MEN_bm;
#endif
		FCPU = ext_osc_freq;
	}
	else if(core_freq)
	{
		if(core_freq == 32000000)
		{
			OSC.CTRL |= OSC_RC32MEN_bm;
			while(!(OSC.STATUS & OSC_RC32MRDY_bm));
			PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
			CCP = CCP_IOREG_gc;
			OSC.CTRL &= ~OSC_RC2MEN_bm;
			FCPU = 32000000;
		}
#if defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16E5__) || defined(__AVR_ATxmega32E5__)
		else if(core_freq == 8000000)
		{
			OSC.CTRL |= OSC_RC8MEN_bm;
			while(!(OSC.STATUS & OSC_RC8MRDY_bm));
			PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC8M_gc);
			CCP = CCP_IOREG_gc;
			OSC.CTRL &= ~OSC_RC2MEN_bm;
			OSC.CTRL &= ~OSC_RC32MEN_bm;
			FCPU = 8000000;
		}
#endif
		else if(core_freq == 32768)
		{
			OSC.CTRL |= OSC_RC32KEN_bm;
			while(!(OSC.STATUS & OSC_RC32KRDY_bm));
			PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC32K_gc);
			CCP = CCP_IOREG_gc;
			OSC.CTRL &= ~OSC_RC2MEN_bm;
			OSC.CTRL &= ~OSC_RC32MEN_bm;
			FCPU = 32768;
		}
		else
		{
			if(ext_osc_freq != 0)
			{
				OSC_DFLLCTRL = OSC_RC32MCREF_XOSC32K_gc;
			}
			else OSC_DFLLCTRL = OSC_RC32MCREF_RC32K_gc;
			DFLLRC32M_CTRL |= DFLL_ENABLE_bm;
			OSC_PLLCTRL = (OSC_PLLSRC_RC2M_gc) | (core_freq / (ext_osc_freq) ? ext_osc_freq : 2000000);
			OSC_CTRL|= OSC_PLLEN_bm;
			while(~OSC_STATUS & OSC_PLLRDY_bm);
			PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_PLL_gc);
			OSC.CTRL &= ~OSC_RC32MEN_bm;
		}
	}
	else
	{
		OSC.CTRL |= OSC_RC2MEN_bm;
		while(!(OSC.STATUS & OSC_RC2MRDY_bm));
		PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC2M_gc);
		CCP = CCP_IOREG_gc;
		OSC.CTRL &= ~OSC_RC32MEN_bm;
		FCPU = int_osc_freq;
	}
}

/**********************************************************************************************
* Global functions
**********************************************************************************************/

GI::Sys::Clock::Clock()
{
	setClk(FCPU);
}

GI::Sys::Clock::~Clock()
{
	//SystemClock_Config(HSI_VALUE, 0, 0);
}

void GI::Sys::Clock::setClk(unsigned long fCpu)
{
	SystemClock_Config(2000000, EXTERNAL_CLOCK_VALUE, CORE_CLOCK_DEFAULT);
}
