/*
 *           LUFA Library
 *   Copyright (C) Dean Camera, 2014.
 * 
 * dean [at] fourwalledcubicle [dot] com
 *         www.lufa-lib.org
 */

/*
 * Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)
 * 
 * Permission to use, copy, modify, distribute, and sell this
 * software and its documentation for any purpose is hereby granted
 * without fee, provided that the above copyright notice appear in
 * all copies and that both that the copyright notice and this
 * permission notice and warranty disclaimer appear in supporting
 * documentation, and that the name of the author not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * 
 * The author disclaims all warranties with regard to this
 * software, including all implied warranties of merchantability
 * and fitness.  In no event shall the author be liable for any
 * special, indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits, whether
 * in an action of contract, negligence or other tortious action,
 * arising out of or in connection with the use or performance of
 * this software.
 */

/*
  Modified 2 May 2014 by Justin Mattair
     for MattairTech MT-DB-Ux boards (www.mattairtech.com)
*/

/** \file
 *
 *  Header file for Bootloader.c.
 */

#ifndef _CDC_H_
#define _CDC_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/boot.h>
		#include <avr/eeprom.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <stdbool.h>
		#include <util/delay.h>
		#include <util/delay_basic.h>
		
		#include "Board.h"

		#include "Descriptors.h"
		#include "Config/AppConfig.h"

		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		
	/* Preprocessor Checks: */
		#if !defined(__OPTIMIZE_SIZE__)
			#error This bootloader requires that it be optimized for size, not speed, to fit into the target device. Change optimization settings and try again.
		#endif

	/* Macros: */
		/** Version major of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MAJOR     0x01

		/** Version minor of the CDC bootloader. */
		#define BOOTLOADER_VERSION_MINOR     0x00

		/** Hardware version major of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MAJOR   0x01

		/** Hardware version minor of the CDC bootloader. */
		#define BOOTLOADER_HWVERSION_MINOR   0x00

		/** Eight character bootloader firmware identifier reported to the host when requested */
		#define SOFTWARE_IDENTIFIER          "MT-CDC "
		
		/** Magic bootloader key to unlock forced application start mode. */
		#define MAGIC_BOOT_KEY               0x7777

	/* Enums: */
		/** Possible memory types that can be addressed via the bootloader. */
		enum AVR109_Memories
		{
			MEMORY_TYPE_FLASH  = 'F',
			MEMORY_TYPE_EEPROM = 'E',
		};

		/** Possible commands that can be issued to the bootloader. */
		enum AVR109_Commands
		{
			AVR109_COMMAND_Sync                     = 27,
			AVR109_COMMAND_ReadEEPROM               = 'd',
			AVR109_COMMAND_WriteEEPROM              = 'D',
			AVR109_COMMAND_ReadFLASHWord            = 'R',
			AVR109_COMMAND_WriteFlashPage           = 'm',
			AVR109_COMMAND_FillFlashPageWordLow     = 'c',
			AVR109_COMMAND_FillFlashPageWordHigh    = 'C',
			AVR109_COMMAND_GetBlockWriteSupport     = 'b',
			AVR109_COMMAND_BlockWrite               = 'B',
			AVR109_COMMAND_BlockRead                = 'g',
			AVR109_COMMAND_ReadExtendedFuses        = 'Q',
			AVR109_COMMAND_ReadHighFuses            = 'N',
			AVR109_COMMAND_ReadLowFuses             = 'F',
			AVR109_COMMAND_ReadLockbits             = 'r',
			AVR109_COMMAND_WriteLockbits            = 'l',
			AVR109_COMMAND_EraseFLASH               = 'e',
			AVR109_COMMAND_ReadSignature            = 's',
			AVR109_COMMAND_ReadBootloaderSWVersion  = 'V',
			AVR109_COMMAND_ReadBootloaderHWVersion  = 'v',
			AVR109_COMMAND_ReadBootloaderIdentifier = 'S',
			AVR109_COMMAND_ReadBootloaderInterface  = 'p',
			AVR109_COMMAND_SetCurrentAddress        = 'A',
			AVR109_COMMAND_ReadAutoAddressIncrement = 'a',
			AVR109_COMMAND_ReadPartCode             = 't',
			AVR109_COMMAND_EnterProgrammingMode     = 'P',
			AVR109_COMMAND_LeaveProgrammingMode     = 'L',
			AVR109_COMMAND_SelectDeviceType         = 'T',
			AVR109_COMMAND_SetLED                   = 'x',
			AVR109_COMMAND_ClearLED                 = 'y',
			AVR109_COMMAND_ExitBootloader           = 'E',
		};

#if defined(__AVR_AT90USB162__)
		#define DEVICE_CODE	0x49
#elif defined(__AVR_ATmega32U2__)
		#define DEVICE_CODE	0x4A
#elif defined(__AVR_ATmega32U4__)
		#define DEVICE_CODE	0x47
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__)
		#define DEVICE_CODE	0x43
#elif defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
		#define DEVICE_CODE	0x43
#else
		#define DEVICE_CODE	0x44
#endif

	/* Type Defines: */
		/** Type define for a non-returning pointer to the start of the loaded application in flash memory. */
		typedef void (*AppPtr_t)(void) ATTR_NO_RETURN;

	/* Function Prototypes: */
		static void CDC_Task(void);
		void SetupHardware(void);

		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

		#if defined(INCLUDE_FROM_BOOTLOADER_C) || defined(__DOXYGEN__)
			static void LED_Task(void);
			static void SetupTimer(void);
			#if !defined(NO_BLOCK_SUPPORT)
			static void    ReadWriteMemoryBlock(const uint8_t Command);
			#endif
			static uint8_t FetchNextCommandByte(void);
			static void    WriteNextResponseByte(const uint8_t Response);
		#endif

#endif

