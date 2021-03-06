﻿/**
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2017 Sensnology AB
* Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*/

#include "MyHwTeensy3.h"


/*
int8_t pinIntTrigger = 0;
void wakeUp()	 //place to send the interrupts
{
	pinIntTrigger = 1;
}
void wakeUp2()	 //place to send the second interrupts
{
	pinIntTrigger = 2;
}

// Watchdog Timer interrupt service routine. This routine is required
// to allow automatic WDIF and WDIE bit clearance in hardware.
ISR (WDT_vect)
{
	// WDIE & WDIF is cleared in hardware upon entering this ISR
	wdt_disable();
}
*/

bool hwInit(void)
{
	MY_SERIALDEVICE.begin(MY_BAUD_RATE);
#if defined(MY_GATEWAY_SERIAL)
	while (!MY_SERIALDEVICE) {}
#endif
	return true;
}

void hwWatchdogReset(void)
{
	// TODO: Not supported!
}

void hwReboot(void)
{
	SCB_AIRCR = 0x05FA0004;
	while (true);
}

int8_t hwSleep(unsigned long ms)
{
	// TODO: Not supported!
	(void)ms;
	return MY_SLEEP_NOT_POSSIBLE;
}

int8_t hwSleep(uint8_t interrupt, uint8_t mode, unsigned long ms)
{
	// TODO: Not supported!
	(void)interrupt;
	(void)mode;
	(void)ms;
	return MY_SLEEP_NOT_POSSIBLE;
}

int8_t hwSleep(uint8_t interrupt1, uint8_t mode1, uint8_t interrupt2, uint8_t mode2,
               unsigned long ms)
{
	// TODO: Not supported!
	(void)interrupt1;
	(void)mode1;
	(void)interrupt2;
	(void)mode2;
	(void)ms;
	return MY_SLEEP_NOT_POSSIBLE;
}

bool hwUniqueID(unique_id_t *uniqueID)
{
#if defined(__MKL26Z64__)
	memcpy((uint8_t*)uniqueID, &SIM_UIDMH, 12);
	memset((uint8_t*)uniqueID + 12, 0, 4);
#else
	memcpy((uint8_t*)uniqueID, &SIM_UIDH, 16);
#endif
	return true;
}

uint16_t hwCPUVoltage()
{
	// TODO: Not supported!
	return 0;
}

uint16_t hwCPUFrequency()
{
	// TODO: currently reporting compile time frequency (in 1/10MHz)
	return F_CPU / 100000UL;
}

uint16_t hwFreeMem()
{
	// TODO: Not supported!
	return 0;
}

void hwDebugPrint(const char *fmt, ...)
{
	char fmtBuffer[MY_SERIAL_OUTPUT_SIZE];
#ifdef MY_GATEWAY_FEATURE
	// prepend debug message to be handled correctly by controller (C_INTERNAL, I_LOG_MESSAGE)
	snprintf_P(fmtBuffer, sizeof(fmtBuffer), PSTR("0;255;%d;0;%d;%lu "),
	           C_INTERNAL, I_LOG_MESSAGE, hwMillis());
	MY_SERIALDEVICE.print(fmtBuffer);
#else
	// prepend timestamp
	MY_SERIALDEVICE.print(hwMillis());
	MY_SERIALDEVICE.print(F(" "));
#endif
	va_list args;
	va_start(args, fmt);
#ifdef MY_GATEWAY_FEATURE
	// Truncate message if this is gateway node
	vsnprintf_P(fmtBuffer, sizeof(fmtBuffer), fmt, args);
	fmtBuffer[sizeof(fmtBuffer) - 2] = '\n';
	fmtBuffer[sizeof(fmtBuffer) - 1] = '\0';
#else
	vsnprintf_P(fmtBuffer, sizeof(fmtBuffer), fmt, args);
#endif
	va_end(args);
	MY_SERIALDEVICE.print(fmtBuffer);
}