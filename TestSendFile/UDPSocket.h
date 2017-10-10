#pragma once

const int DAT_BUFF_SIZE = 2048;

class CAppSetting;
class CRingBuff;

class CUDPSocket : public CAsyncSocket
{
public:
	CUDPSocket();
	~CUDPSocket();

	BOOL Create();
	BOOL Create(LPCTSTR addr, UINT port);
	BOOL CUDPSocket::IsSettingValid();

	int CUDPSocket::Read(CRingBuff* pRingRx);
	int CUDPSocket::Read(void* buffer, int nRead);
	void CUDPSocket::Write(const void* dat, int datSize, UINT hostPort, LPCTSTR hostAddr);
	void CUDPSocket::SetExpPort(UINT port) { m_expectPort = port; }

protected:
	int CUDPSocket::GetRXCount();

	UINT m_expectPort;
	char* m_rawBuf;
	int m_bufSize;
};

