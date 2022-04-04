// CMyEvent.cpp: implementation of the CMyEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"

CMyEvent::CMyEvent()
{
	m_hHandle = 0;
}

CMyEvent::~CMyEvent()
{
	Close();
}

BOOL CMyEvent::Create(CString csName)
{
	BOOL rV = FALSE;
	m_hHandle = CreateEventW(NULL, FALSE, FALSE, csName);
	if (m_hHandle) rV = TRUE;
	return rV;
}

//----------------------------------
//	Post
//		This function is used to
//	the object to the signaled state.
//
// return value
//	TRUE if succesfull
//	FALSE on fail
//--------------------------------------
BOOL CMyEvent::Post()
{
	if (m_hHandle) {
		if (SetEvent(m_hHandle))
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------
// Pend
//	This function will wait for an
//	Event to be signaled.
// Parameter:
//	timeout---Time, in milliseconds
//	that the method will wait for
//	the object to be signalled.
//	The default is Infinate
//
//	return value:
//		WAIT_OBJECT_0....Success
//		WAIT_TIMEOUT.....Timeout
//		WAIT_FALED.......ERROR
//		WAIT_MY_HANDLE...ERROR, band handle
//------------------------------------
DWORD CMyEvent::Pend(DWORD timeout)
{
	DWORD rV = WAIT_MY_HANDLE_ERROR;	//object not created error
	if (m_hHandle)
		rV = ::WaitForSingleObject(m_hHandle, timeout);
	return rV;
}

BOOL CMyEvent::Close()
{
	return CloseHandle(m_hHandle);
}


