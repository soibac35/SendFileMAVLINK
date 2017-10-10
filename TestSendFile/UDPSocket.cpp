#include "stdafx.h"
#include "UDPSocket.h"
#include "RingBuff.h"
#include "Util.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CUDPSocket::CUDPSocket()
{
	m_expectPort = 0;
	m_rawBuf = new char[DAT_BUFF_SIZE];
}

CUDPSocket::~CUDPSocket()
{
	if (m_rawBuf)
		delete[] m_rawBuf;
}

BOOL CUDPSocket::Create(LPCTSTR addr, UINT port)
{
	if (!CAsyncSocket::Create(port, SOCK_DGRAM, FD_READ | FD_WRITE | FD_CLOSE, NULL))
		return 0;

	return 1;
}

int CUDPSocket::GetRXCount()
{
	DWORD dwRead = 0;

	if (IOCtl(FIONREAD, &dwRead))
		return (int)dwRead;

	return 0;
}

int CUDPSocket::Read(CRingBuff* pRingRx)
{
	int nRead = GetRXCount();
	if (nRead > 0)
	{
		if (nRead > DAT_BUFF_SIZE)
			nRead = DAT_BUFF_SIZE;

		CString srcAddr;
		UINT srcPort;
		int ret = ReceiveFrom(m_rawBuf, nRead, srcAddr, srcPort);

		if (ret == 0 || ret == SOCKET_ERROR)
		{
			//TRACE("UDP Read failed, err code = %d\n", ret);
		}
		else if (srcPort == m_expectPort)
		{
			pRingRx->AddData(m_rawBuf, ret);

			return nRead;
		}
		else
			TRACE("Receiver's not me: %s:%d\n", srcAddr, srcPort);
	}

	return 0;
}

int CUDPSocket::Read(void* buffer, int nRead)
{
	return 0;
}

void CUDPSocket::Write(const void* dat, int datSize, UINT hostPort, LPCTSTR hostAddr)
{
	int ret = SendTo(dat, datSize, hostPort, hostAddr);
	if (ret == SOCKET_ERROR)
	{
		int err = GetLastError();

		TRACE("UDP Write failed, err code = %d\n", err);
	}
}

BOOL CUDPSocket::IsSettingValid()
{/*
	if (m_addr == "")
		return 0;

	if (m_port == 0)
		return 0;

	if (!GetPeerName(m_addr, m_port))
		return 0;*/

	return 1;
}

