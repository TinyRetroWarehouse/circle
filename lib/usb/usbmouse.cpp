//
// usbmouse.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2018  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <circle/usb/usbmouse.h>
#include <circle/usb/usbhid.h>
#include <circle/logger.h>
#include <assert.h>

#define REPORT_SIZE	3

static const char FromUSBMouse[] = "umouse";

CUSBMouseDevice::CUSBMouseDevice (CUSBFunction *pFunction)
:	CUSBHIDDevice (pFunction, REPORT_SIZE),
	m_pMouseDevice (0)
{
}

CUSBMouseDevice::~CUSBMouseDevice (void)
{
	delete m_pMouseDevice;
	m_pMouseDevice = 0;
}

boolean CUSBMouseDevice::Configure (void)
{
	if (!CUSBHIDDevice::Configure ())
	{
		CLogger::Get ()->Write (FromUSBMouse, LogError, "Cannot configure HID device");

		return FALSE;
	}

	m_pMouseDevice = new CMouseDevice;
	assert (m_pMouseDevice != 0);

	return TRUE;
}

void CUSBMouseDevice::ReportHandler (const u8 *pReport)
{
	if (pReport != 0)
	{
		u8 ucHIDButtons = pReport[0];

		unsigned nButtons = 0;
		if (ucHIDButtons & USBHID_BUTTON1)
		{
			nButtons |= MOUSE_BUTTON_LEFT;
		}
		if (ucHIDButtons & USBHID_BUTTON2)
		{
			nButtons |= MOUSE_BUTTON_RIGHT;
		}
		if (ucHIDButtons & USBHID_BUTTON3)
		{
			nButtons |= MOUSE_BUTTON_MIDDLE;
		}

		if (m_pMouseDevice != 0)
		{
			m_pMouseDevice->ReportHandler (nButtons, (char) pReport[1],
						       (char) pReport[2]);
		}
	}
}
