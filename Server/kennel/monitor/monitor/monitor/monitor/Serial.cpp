//	Serial.cpp - Implementation of the CSerial class
//
//	Copyright (C) 1999-2001 Ramon de Klein (R.de.Klein@iaf.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


//////////////////////////////////////////////////////////////////////
// Include the standard header files

#define STRICT
#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////
// Include module headerfile

#include "Serial.h"


//////////////////////////////////////////////////////////////////////
// Disable warning C4127: conditional expression is constant, which
// is generated when using the _RPTF and _ASSERTE macros.

#pragma warning(disable: 4127)


//////////////////////////////////////////////////////////////////////
// Enable debug memory manager

#ifdef _DEBUG

#ifdef THIS_FILE
#undef THIS_FILE
#endif

static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW

#endif


//////////////////////////////////////////////////////////////////////
// Code

CSerial::CSerial ()
	: m_lLastError(ERROR_SUCCESS)
	, m_hFile(0)
	, m_eEvent(EEventNone)
	, m_hevtOverlapped(0)
{
}

CSerial::~CSerial ()
{
	// If the device is already closed,
	// then we don't need to do anything.
	if (m_hFile)
	{
		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::~CSerial - Serial port not closed\n");

		// Close implicitly
		Close();
	}
}

CSerial::EPort CSerial::CheckPort (LPCTSTR lpszDevice)
{
	// Try to open the device
	HANDLE hFile = ::CreateFile(lpszDevice, 
						   GENERIC_READ|GENERIC_WRITE, 
						   0, 
						   0, 
						   OPEN_EXISTING, 
						   FILE_FLAG_OVERLAPPED, 
						   0);

	// Check if we could open the device
	if (hFile == INVALID_HANDLE_VALUE)
	{
		// Display error
		switch (::GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			// The specified COM-port does not exist
			return EPortNotAvailable;

		case ERROR_ACCESS_DENIED:
			// The specified COM-port is in use
			return EPortInUse;

		default:
			// Something else is wrong
			return EPortUnknownError;
		}
	}

	// Close handle
	::CloseHandle(hFile);

	// Port is available
	return EPortAvailable;
}

LONG CSerial::Open (LPCTSTR lpszDevice, DWORD dwInQueue, DWORD dwOutQueue)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the port isn't already opened
	if (m_hFile)
	{
		m_lLastError = ERROR_ALREADY_INITIALIZED;
		_RPTF0(_CRT_WARN,"CSerial::Open - Port already opened\n");
		return m_lLastError;
	}

	// Open the device
	m_hFile = ::CreateFile(lpszDevice,
						   GENERIC_READ|GENERIC_WRITE,
						   0,
						   0,
						   OPEN_EXISTING,
						   FILE_FLAG_OVERLAPPED,
						   0);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		// Reset file handle
		m_hFile = 0;

		// Display error
		m_lLastError = ::GetLastError();
		_RPTF0(_CRT_WARN, "CSerial::Open - Unable to open port\n");
		return m_lLastError;
	}

	// We cannot have an event handle yet
	_ASSERTE(m_hevtOverlapped == 0);

	// Create the event handle for internal overlapped operations (manual reset)
	m_hevtOverlapped = ::CreateEvent(0,true,false,0);
	if (m_hevtOverlapped == 0)
	{
		// Obtain the error information
		m_lLastError = ::GetLastError();
		_RPTF0(_CRT_WARN,"CSerial::Open - Unable to create event\n");

		// Close the port
		::CloseHandle(m_hFile);
		m_hFile = 0;

		// Return the error
		return m_lLastError;
	}

	// Setup the COM-port
	if (!::SetupComm(m_hFile,dwInQueue,dwOutQueue))
	{
		// Display a warning
		long lLastError = ::GetLastError();
		_RPTF0(_CRT_WARN,"CSerial::Open - Unable to setup the COM-port\n");

		// Close the port
		Close();

		// Save last error from SetupComm
		m_lLastError = lLastError;
		return m_lLastError;
	}

	// Setup the default communication mask
	SetMask();

	// Setup the device for default settings
	Setup();

	// Non-blocking reads is default
	SetupReadTimeouts(EReadTimeoutNonblocking);

	// Default is no handshaking
	SetupHandshaking(EHandshakeOff);

	// Return successful
	return m_lLastError;
}

LONG CSerial::Close (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// If the device is already closed,
	// then we don't need to do anything.
	if (m_hFile == 0)
	{
		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::Close - Method called when device is not open\n");
		return m_lLastError;
	}

	// Free event handle
	::CloseHandle(m_hevtOverlapped);
	m_hevtOverlapped = 0;

	// Close COM port
	::CloseHandle(m_hFile);
	m_hFile = 0;

	// Return successful
	return m_lLastError;
}

LONG CSerial::Setup (EBaudrate eBaudrate, EDataBits eDataBits, EParity eParity, EStopBits eStopBits)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::Setup - Device is not opened\n");
		return m_lLastError;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::Setup - Unable to obtain DCB information\n");
		return m_lLastError;
	}

	// Set the new data
	dcb.BaudRate = DWORD(eBaudrate);
	dcb.ByteSize = BYTE(eDataBits);
	dcb.Parity   = BYTE(eParity);
	dcb.StopBits = BYTE(eStopBits);

	// Determine if parity is used
	dcb.fParity  = (eParity != EParNone);

	// Set the new DCB structure
	if (!::SetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::Setup - Unable to set DCB information\n");
		return m_lLastError;
	}

	// Return successful
	return m_lLastError;
}

LONG CSerial::SetEventChar (BYTE bEventChar, bool fAdjustMask)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::SetEventChar - Device is not opened\n");
		return m_lLastError;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetEventChar - Unable to obtain DCB information\n");
		return m_lLastError;
	}

	// Set the new event character
	dcb.EvtChar = char(bEventChar);

	// Adjust the event mask, to make sure the event will be received
	if (fAdjustMask)
	{
		// Enable 'receive event character' event.  Note that this
		// will generate an EEventNone if there is an asynchronous
		// WaitCommEvent pending.
		SetMask(GetEventMask() | EEventRcvEv);
	}

	// Set the new DCB structure
	if (!::SetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetEventChar - Unable to set DCB information\n");
		return m_lLastError;
	}

	// Return successful
	return m_lLastError;
}

LONG CSerial::SetMask (DWORD dwMask)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::SetMask - Device is not opened\n");
		return m_lLastError;
	}

	// Set the new mask. Note that this will generate an EEventNone
	// if there is an asynchronous WaitCommEvent pending.
	if (!::SetCommMask(m_hFile,dwMask))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetMask - Unable to set event mask\n");
		return m_lLastError;
	}

	// Return successful
	return m_lLastError;
}

LONG CSerial::WaitEvent (LPOVERLAPPED lpOverlapped, DWORD dwTimeout)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::WaitEvent - Device is not opened\n");
		return m_lLastError;
	}

	// Wait for the event to happen
	OVERLAPPED ovInternal;
	if (lpOverlapped == 0)
	{
		// Setup our own overlapped structure
		memset(&ovInternal,0,sizeof(ovInternal));
		ovInternal.hEvent = m_hevtOverlapped;

		// Use our internal overlapped structure
		lpOverlapped = &ovInternal;
	}

	// Make sure the overlapped structure isn't busy
	_ASSERTE(HasOverlappedIoCompleted(lpOverlapped));

	// Wait for the COM event
	if (!::WaitCommEvent(m_hFile,LPDWORD(&m_eEvent),lpOverlapped))
	{
		// Set the internal error code
		long lLastError = ::GetLastError();

		// Overlapped operation in progress is not an actual error
		if (lLastError != ERROR_IO_PENDING)
		{
			// Save the error
			m_lLastError = lLastError;

			// Issue an error and quit
			_RPTF0(_CRT_WARN,"CSerial::WaitEvent - Unable to wait for COM event\n");
			return m_lLastError;
		}

		// We need to block if the client didn't specify an overlapped structure
		if (lpOverlapped == &ovInternal)
		{
			// Wait for the overlapped operation to complete
			switch (::WaitForSingleObject(lpOverlapped->hEvent,dwTimeout))
			{
			case WAIT_OBJECT_0:
				// The overlapped operation has completed
				break;

			case WAIT_TIMEOUT:
				// Cancel the I/O operation
				::CancelIo(m_hFile);

				// The operation timed out. Set the internal error code and quit
				m_lLastError = ERROR_TIMEOUT;
				return m_lLastError;

			default:
				// Set the internal error code
				m_lLastError = ::GetLastError();

				// Issue an error and quit
				_RPTF0(_CRT_WARN,"CSerial::WaitEvent - Unable to wait until COM event has arrived\n");
				return m_lLastError;
			}
		}
	}
	else
	{
		// The operation completed immediatly. Just to be sure
		// we'll set the overlapped structure's event handle.
		::SetEvent(lpOverlapped->hEvent);
	}

	// Return successfully
	return m_lLastError;
}


LONG CSerial::SetupHandshaking (EHandshake eHandshake)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::SetupHandshaking - Device is not opened\n");
		return m_lLastError;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetupHandshaking - Unable to obtain DCB information\n");
		return m_lLastError;
	}

	// Set the handshaking flags
	switch (eHandshake)
	{
	case EHandshakeOff:
		dcb.fOutxCtsFlow = false;					// Disable CTS monitoring
		dcb.fOutxDsrFlow = false;					// Disable DSR monitoring
		dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR monitoring
		dcb.fOutX = false;							// Disable XON/XOFF for transmission
		dcb.fInX = false;							// Disable XON/XOFF for receiving
		dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
		break;

	case EHandshakeHardware:
		dcb.fOutxCtsFlow = true;					// Enable CTS monitoring
		dcb.fOutxDsrFlow = true;					// Enable DSR monitoring
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;	// Enable DTR handshaking
		dcb.fOutX = false;							// Disable XON/XOFF for transmission
		dcb.fInX = false;							// Disable XON/XOFF for receiving
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;	// Enable RTS handshaking
		break;

	case EHandshakeSoftware:
		dcb.fOutxCtsFlow = false;					// Disable CTS (Clear To Send)
		dcb.fOutxDsrFlow = false;					// Disable DSR (Data Set Ready)
		dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR (Data Terminal Ready)
		dcb.fOutX = true;							// Enable XON/XOFF for transmission
		dcb.fInX = true;							// Enable XON/XOFF for receiving
		dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
		break;

	default:
		// This shouldn't be possible
		_ASSERTE(false);
		m_lLastError = E_INVALIDARG;
		return m_lLastError;
	}

	// Set the new DCB structure
	if (!::SetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetupHandshaking - Unable to set DCB information\n");
		return m_lLastError;
	}

	// Return successful
	return m_lLastError;
}

LONG CSerial::SetupReadTimeouts (EReadTimeout eReadTimeout)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::SetupReadTimeouts - Device is not opened\n");
		return m_lLastError;
	}

	// Determine the time-outs
	COMMTIMEOUTS cto;
	if (!::GetCommTimeouts(m_hFile,&cto))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetupReadTimeouts - Unable to obtain timeout information\n");
		return m_lLastError;
	}

	// Set the new timeouts
	switch (eReadTimeout)
	{
	case EReadTimeoutBlocking:
		cto.ReadIntervalTimeout = 0;
		cto.ReadTotalTimeoutConstant = 0;
		cto.ReadTotalTimeoutMultiplier = 0;
		break;
	case EReadTimeoutNonblocking:
		cto.ReadIntervalTimeout = MAXDWORD;
		cto.ReadTotalTimeoutConstant = 0;
		cto.ReadTotalTimeoutMultiplier = 0;
		break;
	default:
		// This shouldn't be possible
		_ASSERTE(false);
		m_lLastError = E_INVALIDARG;
		return m_lLastError;
	}

	// Set the new DCB structure
	if (!::SetCommTimeouts(m_hFile,&cto))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::SetupReadTimeouts - Unable to set timeout information\n");
		return m_lLastError;
	}

	// Return successful
	return m_lLastError;
}

CSerial::EBaudrate CSerial::GetBaudrate (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetBaudrate - Device is not opened\n");
		return EBaudUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetBaudrate - Unable to obtain DCB information\n");
		return EBaudUnknown;
	}

	// Return the appropriate baudrate
	return EBaudrate(dcb.BaudRate);
}

CSerial::EDataBits CSerial::GetDataBits (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetDataBits - Device is not opened\n");
		return EDataUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetDataBits - Unable to obtain DCB information\n");
		return EDataUnknown;
	}

	// Return the appropriate bytesize
	return EDataBits(dcb.ByteSize);
}

CSerial::EParity CSerial::GetParity (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetParity - Device is not opened\n");
		return EParUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetParity - Unable to obtain DCB information\n");
		return EParUnknown;
	}

	// Check if parity is used
	if (!dcb.fParity)
	{
		// No parity
		return EParNone;
	}

	// Return the appropriate parity setting
	return EParity(dcb.Parity);
}

CSerial::EStopBits CSerial::GetStopBits (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetStopBits - Device is not opened\n");
		return EStopUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetStopBits - Unable to obtain DCB information\n");
		return EStopUnknown;
	}

	// Return the appropriate stopbits
	return EStopBits(dcb.StopBits);
}

DWORD CSerial::GetEventMask (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetEventMask - Device is not opened\n");
		return 0;
	}

	// Obtain the COM mask
	DWORD dwMask = 0;
	if (!::GetCommMask(m_hFile, &dwMask))
	{
		// Set the internal error code
		m_lLastError = ::GetLastError();

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetEventMask - Unable to get the event mask\n");
		return 0;
	}

	// Return the event mask
	return dwMask;
}

BYTE CSerial::GetEventChar (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetEventChar - Device is not opened\n");
		return 0;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetEventChar - Unable to obtain DCB information\n");
		return 0;
	}

	// Set the new event character
	return BYTE(dcb.EvtChar);
}

CSerial::EHandshake CSerial::GetHandshaking (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetHandshaking - Device is not opened\n");
		return EHandshakeUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetHandshaking - Unable to obtain DCB information\n");
		return EHandshakeUnknown;
	}

	// Check if hardware handshaking is being used
	if ((dcb.fDtrControl == DTR_CONTROL_HANDSHAKE) && (dcb.fRtsControl == RTS_CONTROL_HANDSHAKE))
		return EHandshakeHardware;

	// Check if software handshaking is being used
	if (dcb.fOutX && dcb.fInX)
		return EHandshakeSoftware;

	// No handshaking is being used
	return EHandshakeOff;
}

LONG CSerial::Write (const void* pData, size_t iLen, DWORD* pdwWritten, LPOVERLAPPED lpOverlapped, DWORD dwTimeout)
{
	// Overlapped operation should specify the pdwWritten variable
	_ASSERTE(!lpOverlapped || pdwWritten);

	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Use our own variable for read count
	DWORD dwWritten;
	if (pdwWritten == 0)
	{
		pdwWritten = &dwWritten;
	}

	// Reset the number of bytes written
	*pdwWritten = 0;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::Write - Device is not opened\n");
		return m_lLastError;
	}

	// Wait for the event to happen
	OVERLAPPED ovInternal;
	if (lpOverlapped == 0)
	{
		// Setup our own overlapped structure
		memset(&ovInternal,0,sizeof(ovInternal));
		ovInternal.hEvent = m_hevtOverlapped;

		// Use our internal overlapped structure
		lpOverlapped = &ovInternal;
	}

	// Make sure the overlapped structure isn't busy
	_ASSERTE(HasOverlappedIoCompleted(lpOverlapped));

	// Write the data
	if (!::WriteFile(m_hFile,pData,iLen,pdwWritten,lpOverlapped))
	{
		// Set the internal error code
		long lLastError = ::GetLastError();

		// Overlapped operation in progress is not an actual error
		if (lLastError != ERROR_IO_PENDING)
		{
			// Save the error
			m_lLastError = lLastError;

			// Issue an error and quit
			_RPTF0(_CRT_WARN,"CSerial::Write - Unable to write the data\n");
			return m_lLastError;
		}

		// We need to block if the client didn't specify an overlapped structure
		if (lpOverlapped == &ovInternal)
		{
			// Wait for the overlapped operation to complete
			switch (::WaitForSingleObject(lpOverlapped->hEvent,dwTimeout))
			{
			case WAIT_OBJECT_0:
				// The overlapped operation has completed
				if (!::GetOverlappedResult(m_hFile,lpOverlapped,pdwWritten,FALSE))
				{
					// Set the internal error code
					m_lLastError = ::GetLastError();

					_RPTF0(_CRT_WARN,"CSerial::Write - Overlapped completed without result\n");
					return m_lLastError;
				}
				break;

			case WAIT_TIMEOUT:
				// Cancel the I/O operation
				::CancelIo(m_hFile);

				// The operation timed out. Set the internal error code and quit
				m_lLastError = ERROR_TIMEOUT;
				return m_lLastError;

			default:
				// Set the internal error code
				m_lLastError = ::GetLastError();

				// Issue an error and quit
				_RPTF0(_CRT_WARN,"CSerial::Write - Unable to wait until data has been sent\n");
				return m_lLastError;
			}
		}
	}
	else
	{
		// The operation completed immediatly. Just to be sure
		// we'll set the overlapped structure's event handle.
		::SetEvent(lpOverlapped->hEvent);
	}

	// Return successfully
	return m_lLastError;
}

LONG CSerial::Write (LPCSTR pString, DWORD* pdwWritten, LPOVERLAPPED lpOverlapped, DWORD dwTimeout)
{
	// Determine the length of the string
	return Write(pString,strlen(pString),pdwWritten,lpOverlapped,dwTimeout);
}

LONG CSerial::Read (void* pData, size_t iLen, DWORD* pdwRead, LPOVERLAPPED lpOverlapped, DWORD dwTimeout)
{
	// Overlapped operation should specify the pdwRead variable
	_ASSERTE(!lpOverlapped || pdwRead);

	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Use our own variable for read count
	DWORD dwRead;
	if (pdwRead == 0)
	{
		pdwRead = &dwRead;
	}

	// Reset the number of bytes read
	*pdwRead = 0;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::Read - Device is not opened\n");
		return m_lLastError;
	}

	// Wait for the event to happen
	OVERLAPPED ovInternal;
	if (lpOverlapped == 0)
	{
		// Setup our own overlapped structure
		memset(&ovInternal,0,sizeof(ovInternal));
		ovInternal.hEvent = m_hevtOverlapped;

		// Use our internal overlapped structure
		lpOverlapped = &ovInternal;
	}

	// Make sure the overlapped structure isn't busy
	_ASSERTE(HasOverlappedIoCompleted(lpOverlapped));

#ifdef _DEBUG
	// The debug version fills the entire data structure with
	// 0xDC bytes, to catch buffer errors as soon as possible.
	memset(pData,0xDC,iLen);
#endif
	
	// Read the data
	if (!::ReadFile(m_hFile,pData,iLen,pdwRead,lpOverlapped))
	{
		// Set the internal error code
		long lLastError = ::GetLastError();

		// Overlapped operation in progress is not an actual error
		if (lLastError != ERROR_IO_PENDING)
		{
			// Save the error
			m_lLastError = lLastError;

			// Issue an error and quit
			_RPTF0(_CRT_WARN,"CSerial::Read - Unable to read the data\n");
			return m_lLastError;
		}

		// We need to block if the client didn't specify an overlapped structure
		if (lpOverlapped == &ovInternal)
		{
			// Wait for the overlapped operation to complete
			switch (::WaitForSingleObject(lpOverlapped->hEvent,dwTimeout))
			{
			case WAIT_OBJECT_0:
				// The overlapped operation has completed
				if (!::GetOverlappedResult(m_hFile,lpOverlapped,pdwRead,FALSE))
				{
					// Set the internal error code
					m_lLastError = ::GetLastError();

					_RPTF0(_CRT_WARN,"CSerial::Read - Overlapped completed without result\n");
					return m_lLastError;
				}
				break;

			case WAIT_TIMEOUT:
				// Cancel the I/O operation
				::CancelIo(m_hFile);

				// The operation timed out. Set the internal error code and quit
				m_lLastError = ERROR_TIMEOUT;
				return m_lLastError;

			default:
				// Set the internal error code
				m_lLastError = ::GetLastError();

				// Issue an error and quit
				_RPTF0(_CRT_WARN,"CSerial::Read - Unable to wait until data has been read\n");
				return m_lLastError;
			}
		}
	}
	else
	{
		// The operation completed immediatly. Just to be sure
		// we'll set the overlapped structure's event handle.
		::SetEvent(lpOverlapped->hEvent);
	}

	// Return successfully
	return m_lLastError;
}

LONG CSerial::Purge()
{
	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::Purge - Device is not opened\n");
		return m_lLastError;
	}

	if (!::PurgeComm(m_hFile, PURGE_TXCLEAR | PURGE_RXCLEAR))
	{
		// Set the internal error code
		m_lLastError = ::GetLastError();
		_RPTF0(_CRT_WARN,"CSerial::Purge - Overlapped completed without result\n");
	}
	
	// Return successfully
	return m_lLastError;
}

CSerial::EEvent CSerial::GetEventType (void)
{
	// Obtain the event
	EEvent eEvent = m_eEvent;

	// Reset internal event type
	m_eEvent = EEventNone;

	// Return the current cause
	return eEvent;
}

CSerial::EError CSerial::GetError (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_HANDLE;

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetError - Device is not opened\n");
		return EErrorUnknown;
	}

	// Obtain COM status
	DWORD dwErrors = 0;
	if (!::ClearCommError(m_hFile,&dwErrors,0))
	{
		// Set the internal error code
		m_lLastError = ::GetLastError();

		// Issue an error and quit
		_RPTF0(_CRT_WARN,"CSerial::GetError - Unable to obtain COM status\n");
		return EErrorUnknown;
	}

	// Return the error
	return EError(dwErrors);
}

bool CSerial::GetCTS (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Obtain the modem status
	DWORD dwModemStat = 0;
	if (!::GetCommModemStatus(m_hFile,&dwModemStat))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetCTS - Unable to obtain the modem status\n");
		return false;
	}

	// Determine if CTS is on
	return (dwModemStat & MS_CTS_ON) != 0;
}

bool CSerial::GetDSR (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Obtain the modem status
	DWORD dwModemStat = 0;
	if (!::GetCommModemStatus(m_hFile,&dwModemStat))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetDSR - Unable to obtain the modem status\n");
		return false;
	}

	// Determine if DSR is on
	return (dwModemStat & MS_DSR_ON) != 0;
}

bool CSerial::GetRing (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Obtain the modem status
	DWORD dwModemStat = 0;
	if (!::GetCommModemStatus(m_hFile,&dwModemStat))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetRing - Unable to obtain the modem status");
		return false;
	}

	// Determine if Ring is on
	return (dwModemStat & MS_RING_ON) != 0;
}

bool CSerial::GetRLSD (void)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Obtain the modem status
	DWORD dwModemStat = 0;
	if (!::GetCommModemStatus(m_hFile,&dwModemStat))
	{
		// Obtain the error code
		m_lLastError = ::GetLastError();

		// Display a warning
		_RPTF0(_CRT_WARN,"CSerial::GetRLSD - Unable to obtain the modem status");
		return false;
	}

	// Determine if RLSD is on
	return (dwModemStat & MS_RLSD_ON) != 0;
}

/******************************************************************************
 *	Examples:																  *
 *																			  *
 *		HelloWorld Project													  *
 *		Listener Project													  *
 *		Overlapped Project													  *
 *																			  *
 ******************************************************************************/

int HelloWord ()
{
    CSerial serial;
	LONG    lLastError = ERROR_SUCCESS;

    lLastError = serial.Open(_T("COM1"));
	if (lLastError != ERROR_SUCCESS)
		return 1;

    lLastError = serial.Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
		return 1;

    lLastError = serial.SetupHandshaking(CSerial::EHandshakeHardware);
	if (lLastError != ERROR_SUCCESS)
		return 1;

    lLastError = serial.Write("Hello world\n");
	if (lLastError != ERROR_SUCCESS)
		return 1;

    serial.Close();
	return 0;
}

#include <stdio.h>

enum { EOF_Char = 27 };

int Listener ()
{
    CSerial serial;
	LONG    lLastError = ERROR_SUCCESS;

    // Attempt to open the serial port (COM1)
    lLastError = serial.Open(_T("COM1"));
	if (lLastError != ERROR_SUCCESS)
		return 1;

    // Setup the serial port (9600,8N1, which is the default setting)
    lLastError = serial.Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
		return 1;

	// Setup handshaking (default is no handshaking)
    lLastError = serial.SetupHandshaking(CSerial::EHandshakeHardware);
	if (lLastError != ERROR_SUCCESS)
		return 1;

    // Register only for the receive event
    lLastError = serial.SetMask(CSerial::EEventBreak |
								CSerial::EEventCTS   |
								CSerial::EEventDSR   |
								CSerial::EEventError |
								CSerial::EEventRing  |
								CSerial::EEventRLSD  |
								CSerial::EEventRecv);
	if (lLastError != ERROR_SUCCESS)
		return 1;

	// Use 'non-blocking' reads, because we don't know how many bytes
	// will be received. This is normally the most convenient mode
	// (and also the default mode for reading data).
    lLastError = serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
	if (lLastError != ERROR_SUCCESS)
		return 1;

    // Keep reading data, until an EOF (CTRL-Z) has been received
	bool fContinue = true;
	do
	{
		// Wait for an event
		lLastError = serial.WaitEvent();
		if (lLastError != ERROR_SUCCESS)
			return 1;

		// Save event
		const CSerial::EEvent eEvent = serial.GetEventType();

		// Handle break event
		if (eEvent & CSerial::EEventBreak)
		{
			printf("\n### BREAK received ###\n");
		}

		// Handle CTS event
		if (eEvent & CSerial::EEventCTS)
		{
			printf("\n### Clear to send %s ###\n", serial.GetCTS()?"on":"off");
		}

		// Handle DSR event
		if (eEvent & CSerial::EEventDSR)
		{
			printf("\n### Data set ready %s ###\n", serial.GetDSR()?"on":"off");
		}

		// Handle error event
		if (eEvent & CSerial::EEventError)
		{
			printf("\n### ERROR: ");
			switch (serial.GetError())
			{
			case CSerial::EErrorBreak:		printf("Break condition");			break;
			case CSerial::EErrorFrame:		printf("Framing error");			break;
			case CSerial::EErrorIOE:		printf("IO device error");			break;
			case CSerial::EErrorMode:		printf("Unsupported mode");			break;
			case CSerial::EErrorOverrun:	printf("Buffer overrun");			break;
			case CSerial::EErrorRxOver:		printf("Input buffer overflow");	break;
			case CSerial::EErrorParity:		printf("Input parity error");		break;
			case CSerial::EErrorTxFull:		printf("Output buffer full");		break;
			default:						printf("Unknown");					break;
			}
			printf(" ###\n");
		}

		// Handle ring event
		if (eEvent & CSerial::EEventRing)
		{
			printf("\n### RING ###\n");
		}

		// Handle RLSD/CD event
		if (eEvent & CSerial::EEventRLSD)
		{
			printf("\n### RLSD/CD %s ###\n", serial.GetRLSD()?"on":"off");
		}

		// Handle data receive event
		if (eEvent & CSerial::EEventRecv)
		{
			// Read data, until there is nothing left
			DWORD dwBytesRead = 0;
			char szBuffer[101];
			do
			{
				// Read data from the COM-port
				lLastError = serial.Read(szBuffer,sizeof(szBuffer)-1,&dwBytesRead);
				if (lLastError != ERROR_SUCCESS)
					return 1;

				if (dwBytesRead > 0)
				{
					// Finalize the data, so it is a valid string
					szBuffer[dwBytesRead] = '\0';

					// Display the data
					printf("%s", szBuffer);

					// Check if EOF has been specified
					if (strchr(szBuffer,EOF_Char))
						fContinue = false;
				}
			}
		    while (dwBytesRead == sizeof(szBuffer)-1);
		}
	}
	while (fContinue);

    // Close the port again
    serial.Close();
    return 0;
}

int Overlapped ()
{
    CSerial serial;
	LONG    lLastError = ERROR_SUCCESS;

    // Attempt to open the serial port (COM1)
    lLastError = serial.Open(_T("COM1"));
	if (lLastError != ERROR_SUCCESS)
		return 1;

    // Setup the serial port (9600,8N1, which is the default setting)
    lLastError = serial.Setup(CSerial::EBaud9600,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
	if (lLastError != ERROR_SUCCESS)
		return 1;

	// Setup handshaking (default is no handshaking)
    lLastError = serial.SetupHandshaking(CSerial::EHandshakeHardware);
	if (lLastError != ERROR_SUCCESS)
		return 1;

    // Register only for the receive event
    lLastError = serial.SetMask(CSerial::EEventBreak |
								CSerial::EEventCTS   |
								CSerial::EEventDSR   |
								CSerial::EEventError |
								CSerial::EEventRing  |
								CSerial::EEventRLSD  |
								CSerial::EEventRecv);
	if (lLastError != ERROR_SUCCESS)
		return 1;

	// Use 'non-blocking' reads, because we don't know how many bytes
	// will be received. This is normally the most convenient mode
	// (and also the default mode for reading data).
    lLastError = serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
	if (lLastError != ERROR_SUCCESS)
		return 1;

	// Create a handle for the overlapped operations
	HANDLE hevtOverlapped = ::CreateEvent(0,TRUE,FALSE,0);;
	if (hevtOverlapped == 0)
		return 1;

	// Setup the overlapped structure
	OVERLAPPED ov = {0};
	ov.hEvent = hevtOverlapped;

	// Open the "STOP" handle
	HANDLE hevtStop = ::CreateEvent(0,TRUE,FALSE,_T("Overlapped_Stop_Event"));
	if (hevtStop == 0)
		return 1;

    // Keep reading data, until an EOF (CTRL-Z) has been received
	bool fContinue = true;
	do
	{
		// Wait for an event
		lLastError = serial.WaitEvent(&ov);
		if (lLastError != ERROR_SUCCESS)
			return 1;

		// Setup array of handles in which we are interested
		HANDLE ahWait[2];
		ahWait[0] = hevtOverlapped;
		ahWait[1] = hevtStop;

		// Wait until something happens
		switch (::WaitForMultipleObjects(sizeof(ahWait)/sizeof(*ahWait),ahWait,FALSE,INFINITE))
		{
		case WAIT_OBJECT_0:
			{
				// Save event
				const CSerial::EEvent eEvent = serial.GetEventType();

				// Handle break event
				if (eEvent & CSerial::EEventBreak)
				{
					printf("\n### BREAK received ###\n");
				}

				// Handle CTS event
				if (eEvent & CSerial::EEventCTS)
				{
					printf("\n### Clear to send %s ###\n", serial.GetCTS()?"on":"off");
				}

				// Handle DSR event
				if (eEvent & CSerial::EEventDSR)
				{
					printf("\n### Data set ready %s ###\n", serial.GetDSR()?"on":"off");
				}

				// Handle error event
				if (eEvent & CSerial::EEventError)
				{
					printf("\n### ERROR: ");
					switch (serial.GetError())
					{
					case CSerial::EErrorBreak:		printf("Break condition");			break;
					case CSerial::EErrorFrame:		printf("Framing error");			break;
					case CSerial::EErrorIOE:		printf("IO device error");			break;
					case CSerial::EErrorMode:		printf("Unsupported mode");			break;
					case CSerial::EErrorOverrun:	printf("Buffer overrun");			break;
					case CSerial::EErrorRxOver:		printf("Input buffer overflow");	break;
					case CSerial::EErrorParity:		printf("Input parity error");		break;
					case CSerial::EErrorTxFull:		printf("Output buffer full");		break;
					default:						printf("Unknown");					break;
					}
					printf(" ###\n");
				}

				// Handle ring event
				if (eEvent & CSerial::EEventRing)
				{
					printf("\n### RING ###\n");
				}

				// Handle RLSD/CD event
				if (eEvent & CSerial::EEventRLSD)
				{
					printf("\n### RLSD/CD %s ###\n", serial.GetRLSD()?"on":"off");
				}

				// Handle data receive event
				if (eEvent & CSerial::EEventRecv)
				{
					// Read data, until there is nothing left
					DWORD dwBytesRead = 0;
					do
					{
						char szBuffer[101];

						// Read data from the COM-port
						lLastError = serial.Read(szBuffer,sizeof(szBuffer)-1,&dwBytesRead);
						if (lLastError != ERROR_SUCCESS)
							return 1;

						if (dwBytesRead > 0)
						{
							// Finalize the data, so it is a valid string
							szBuffer[dwBytesRead] = '\0';

							// Display the data
							printf("%s", szBuffer);
						}
					}
					while (dwBytesRead > 0);
				}
			}
			break;

		case WAIT_OBJECT_0+1:
			{
				// Set the continue bit to false, so we'll exit
				fContinue = false;
			}
			break;

		default:
			{
				// Something went wrong
				return 1;
			}
			break;
		}
	}
	while (fContinue);

    // Close the port again
    serial.Close();
    return 0;
}