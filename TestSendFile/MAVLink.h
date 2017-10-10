#pragma once

class CUDPDev;
class CTxFile;

class CMAVLink
{
public:
	CMAVLink();
	~CMAVLink();

	BOOL CMAVLink::Create();
	BOOL CMAVLink::Create(LPCTSTR ipAddr, UINT port);
	BOOL CMAVLink::ChangeSettings(LPCTSTR ipAddr, UINT port, BOOL bReceiver);
	void CMAVLink::WriteLog(LPCTSTR szFormat, ...);
	void CMAVLink::WriteStatus(LPCTSTR szFormat, ...);
	void GetDesc(CString& desc);
	void Update();
	void SaveSettings();
	void LoadSettings();

	BOOL CMAVLink::SendFile(LPCTSTR fileName);

	CString m_addr;
	UINT m_port;
	BOOL m_bReceiver;

protected:
	BOOL m_bConnected;
	CUDPDev* m_dev;
};

