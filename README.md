# CppSDK
CPP embedded standalone collection software.

This SDK is a mix of drivers provided by microcontroller manufacturers and library's developed by me and other developers.

I started this project with ideea that any beginner developer can start a standalone project portable on multiple platforms with no or few changes made on the project.

This project integrate drivers for most common interfaces like GPIO, UART, I2C and SPI but even for most advenced interfaces like MMCSD, ETH, USB and LCD's with a nice and simple to create GUI interfaces with or without touch screens.

This project is a translation of <a href="https://github.com/MorgothCreator/mSdk">mSdk prtoject</a> from C to C++ offering a more flexible function manipulation.

In furure this project will be extended to AVR32, AM335x, STM32F0, STM32F1, STM32F3, ATxmega and other platforms.

This project will provide some example applications, to use this examples without changes extract or clone this project in "C:\GitHub" folder.

For different platforms I use different IDE's to develop this SDK, see description on each platform.

For STM32 I use <a href="http://www.openstm32.org/HomePage">System Workbench for STM32 IDE</a>, I decide to begin with this platform because this manufacturer offer a huge low level driver support.

For NXP Kinetis E I use <a href="http://www.nxp.com/products/software-and-tools/hardware-development-tools/freedom-development-boards/kinetis-design-studio-integrated-development-environment-ide:KDS_IDE">Kinetis Design Studio IDE</a>, I extend the development on kinetis E series because is the only 32bit cortex platform that can work on 5V power voltage, this has the advantage of high noise immunity and is designed for high reliable industrial applications.

At this moment include next features:

	STM32F4 (System Workbench for STM32 IDE):
		Board:
			STM32F469I configuration board.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
			MMCSD.
			LCD using DSI interface.
		Dedicated evices:
 			OTM8009a TFT display controller.
      
      
 	STM32F7 (System Workbench for STM32 IDE):
		Board:
			STM32F769I configuration board.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
			MMCSD.
			ETH with LwIp.
			LCD using DSI interface.
		Dedicated evices:
 			OTM8009a TFT display controller.
      
 	NXP Kinetis E seties (Kinetis Design Studio IDE):
		Board:
			FRDM-KEAZ128Q80 configuration board.
		Drivers:
			I2C.
			UART.
			SPI.
			GPIO.
      
> On all platforms will be ready to work with next list of sensors:

* FT5x06 capacitive touch screen using I2C interface
* Lepton FLIR sensor using SPI interface.
* HIH613x humidity and temperature sensor using I2C interface.
* MPU60x0 and MPU9x50 accelerometer, giroscope and temperature sensor using I2C interface.
* AK8975 magnetometer sensor using I2C interface.
* BMP180 pressure, altimeter and temperature sensor using I2C interface.
* MPL3115a2 pressure altimeter and temperature sensor using I2C interface.

> On several platforms will be disponible next interfaces:

* For platforms that are provided the ETH interface driver will come with the LwIp library and some applications like HTTP server, TFTP server and other applications.
* For platforms that are provided the USB MSC host driver will mount the FS automaticaly when a USB pen drive is inserted (the FS supported are FAT12, FAT16, FAT32 and exFAT).
* For platforms that are provided the USB MSC device you need to link a MMCSD interface or a custom disk driver.

> All namespaces and classes are under "GI" namespace.

Please watch for available interfaces in "[platform directory]/board/[Board directory]/def.cpp" file.
To request controll of one of initialized interface use next examples:

> Example of blinking led 1 on the board:

```CPP
/*
 * Put the '#include <api/init.h>' first on file because first  need to call the init function from this file.
 */
#include <api/init.h>
#include<api/io_handle.h>
#include<api/timer.h>

int main(void)
{
	GI::IO led_pin = GI::IO((char *)"led-0");
	while(1)
	{
		led_pin.write(true);
		GI::Sys::Timer::delay(500);
		led_pin.write(false);
		GI::Sys::Timer::delay(500);
	}
}
```

> Blinking led 1 using non blocking timer:

```CPP
/*
 * Put the '#include <api/init.h>' first on file because first  need to call the init function from this file.
 */
#include <api/init.h>
#include<api/io_handle.h>
#include<api/timer.h>

int main(void)
{
	/*
	 * Create a 500ms nonblocking timer.
	 */
	GI::Sys::Timer blink_timer = GI::Sys::Timer(500);
	/*
	 * Take controll to "led-0" pin.
	 */
	GI::IO led_pin = GI::IO((char *)"led-0");
	while(1)
	{
		/*
		 * Ceck if is a tick.
		 */
		if(blink_timer.tick())
		{
			bool state;
			/*
			 * Get "led-0" state.
			 */
			led_pin.read(&state);
			if(state)
				/*
				 * If "led-0" state is '1' put it to '0'.
				 */
				led_pin.write(false);
			else
				/*
				 * If "led-0" state is '0' put it to '1'.
				 */
				led_pin.write(true);
		}
	}
}
```
> To create a working window:

```CPP
/*
 * Put the '#include <api/init.h>' first on file because first  need to call the init function from this file.
 */
#include <api/init.h>
#include "lib/gfx/window.h"
/*
 * Make 'MainWindow' globally visible.
 */
GI::Screen::Gfx::Window *MainWindow = NULL;
int main(void)
{
	/*
	 * Create one parent window.
	 */
	MainWindow = new GI::Screen::Gfx::Window(NULL, dev.SCREEN[0]);/*The parent window for this window is NULL, the screen is SCREEN 0*/
	MainWindow->AllowHScroll = false;/*If items on this window is outsiede borders do not allow horizontal scroll bar*/
	MainWindow->AllowVScroll = false;/*If items on this window is outsiede borders do not allow vertical scroll bar*/
	MainWindow->HideHScroll = true;/*Make horizontal scroll bal hidden*/
	MainWindow->HideVScroll = true;/*Make vertical scroll bal hidden*/
	MainWindow->MaxMinEnabled = false;/*Deactivate minimize/maximize button on header*/
	MainWindow->MinimizeButonEnabled = false;/*Deactivate minimize button on header*/
	MainWindow->CloseButonEnabled = false;/*Deactivate close button on header*/
	MainWindow->Caption->textAlign = alignLeft;/*Allign caption text to left*/
	
	/*
	 * Create a text box.
	 */
	newTextBox(MainWindow, SensorResultTextbox);
	SensorResultTextbox->Position.X = 10;
	SensorResultTextbox->Position.Y = 10;
	SensorResultTextbox->Size.X = 460;
	SensorResultTextbox->Size.Y = 200;
	SensorResultTextbox->text->setText((char *)"This is a sensor result textbox");

	/*
	 * Create a button.
	 */
	newButton(MainWindow, Buton1);
	Buton1->Position.X = 10;
	Buton1->Position.Y = SensorResultTextbox->Position.Y + SensorResultTextbox->Size.Y + 10;
	Buton1->Size.X = 100;
	Buton1->Size.Y = 50;

	/*
	 * Create a progress bar.
	 */
	newProgressBar(MainWindow, ProgressBar1);
	ProgressBar1->MinimValue  = 0;
	ProgressBar1->MaximValue = 100;
	ProgressBar1->Value = 50;
	ProgressBar1->Position.X = 10;
	ProgressBar1->Position.Y = Buton1->Position.Y + Buton1->Size.Y + 10;
	ProgressBar1->Size.X = 460;
	ProgressBar1->Size.Y = 30;

	/*
	 * Create a check box.
	 */
	newCheckBox(MainWindow, CheckBox1);
	CheckBox1->Position.X = 10;
	CheckBox1->Position.Y = ProgressBar1->Position.Y + ProgressBar1->Size.Y + 10;
	CheckBox1->Size.X = 460;
	CheckBox1->Size.Y = 30;

	/*
	 * Create a list box.
	 */
	newListBox(MainWindow, ListBox);
	ListBox->Position.X = 10;
	ListBox->Position.Y = CheckBox1->Position.Y + CheckBox1->Size.Y + 10;
	ListBox->Size.X = 460;
	ListBox->Size.Y = MainWindow->Internals.pDisplay->LcdTimings->Y - (CheckBox1->Position.Y + CheckBox1->Size.Y + 30);
	ListBox->Size.ScrollSize = 50;/* The scroll btn width size. */
	ListBox->Size.ItemSizeY = 30/* The item hecht size. */;
	ListBox->Size.MinScrollBtnSize = 30;/* minimum scroll button height size. */
	ListBox->Caption->textAlign = alignCenter;/*The text on each item will be alligned to center*/
	/*
	 * Populate list box with 256 items.
	 */
	unsigned int cnt = 0;
	char listbox_buff[32];
	char buff_tmp[10];
	for(; cnt < 256; cnt++)
	{
		strcpy((char *)listbox_buff, "Device ID ");
		utoa(cnt + 1, buff_tmp, 10);
		strcat(listbox_buff, buff_tmp);
		ListBox->add(listbox_buff);
	}
	/*
	 * Put a second window inside current window, a password protection window.
	 *
	 * The parrent window will be MainWindow, the name will be pass, 
	 *	the x space between buttons wil be 2 pixels, the y space between buttons will be 2 pixels*/
	 */
	newWindowPasswordNumeric(MainWindow, pass, 2, 2);
	
	/*
	 * Create a controll structure to pass data from touch screen to window.
	 */
	tControlCommandData control_comand;
	/*
	 * Create a nonblocking timmer to call the idle function of parrent window every 20 miliseconds .
	 */
	GI::Sys::Timer timer_touch = GI::Sys::Timer(20);

	while(1)
	{
		/*
		 * Ceck if is a tick.
		 */
		if(timer_touch.tick())
		{
			/*
			 * Clear the 'controll_comand' structure.
			 */
			memset(&control_comand, 0, sizeof(tControlCommandData));
			/*
			 * Call the idle function of the touchscreen.
			 */
			dev.CAPTOUCH[0]->idle();
			/*
			 * Copy relevant data from touchscreen to 'controll_comand' 
			 *	structure that will be passed to main window.
			 */
			control_comand.Cursor = (CursorState)dev.CAPTOUCH[0]->TouchResponse.touch_event1;
			control_comand.X = dev.CAPTOUCH[0]->TouchResponse.x1;
			control_comand.Y = dev.CAPTOUCH[0]->TouchResponse.y1;
			MainWindow->idle(&control_comand);
			/*
			 * The idle function of 'pass' window will be called by parent window, 
			 *	but the idle function on 'pass' controll need to be call by user 
			 *	returning true if the "OK" button has been hitted or false if no.
			 */
			if(pass->idle())
			{
				/*
				 * The "OK" button has been pressed, check if password is correct.
				 */
				if(pass->password->equal((char *)"1234"))
					/*
					 * If password match, make pasword window invisible.
					 * At this moment is not implemented the destroy window function, 
					 *	for this reason I will use visible = false, to hide password window.
					 */
					pass->internals.windowHandler->Visible = false;
				else
				{
					/*
					 * If password does not match print "Wrong password!" on password texbox.
					 */
					pass->clearText->set((char *)"Wrong password!");
				}
			}
		}
	}
}
```
