#include "stdafx.h"
#include "MAVLink.h"
#include "RemoteDev.h"
#include "ControllerDev.h"
#include "AppSetting.h"
#include "AppDef.h"
#include "TxFile.h"
#include "Util.h"
#include "TestSendFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMAVLink::CMAVLink()
{
	m_bConnected = 0;
	m_dev = NULL;
}

CMAVLink::~CMAVLink()
{
	if (m_dev)
		delete m_dev;

	m_dev = NULL;
}

BOOL CMAVLink::Create()
{
	LoadSettings();

	return Create(m_addr, m_port);
}

BOOL CMAVLink::Create(LPCTSTR ipAddr, UINT port)
{
	if (m_dev)
		delete m_dev;

	if (m_bReceiver)
		m_dev = new CControllerDev(this);
	else
		m_dev = new CRemoteDev(this);

	return m_dev->Create(ipAddr, port);
}

BOOL CMAVLink::ChangeSettings(LPCTSTR ipAddr, UINT port, BOOL bReceiver)
{
	if (m_bReceiver != bReceiver || m_addr != ipAddr || m_port != port)
	{
		m_bReceiver = bReceiver;
		m_addr = ipAddr;
		m_port = port;

		Create(ipAddr, port); //fix remote with port 10000

		SaveSettings();

		return 1;
	}

	return 0;
}

void CMAVLink::GetDesc(CString& desc)
{
	desc.Format("%s:%d - %s", m_addr, m_port, m_bReceiver ? "Receiver" : "Sender");
}

void CMAVLink::SaveSettings()
{
	CAppSetting reg("settings.ini");
	reg.SetKey("dev", "receiver", m_bReceiver);
	reg.SetKey("dev", "ipaddr", m_addr);
	reg.SetKey("dev", "port", m_port);
}

void CMAVLink::LoadSettings()
{
	CAppSetting reg("settings.ini");
	m_bReceiver = reg.GetKey("dev", "receiver", FALSE);
	m_addr = reg.GetKey("dev", "ipaddr", "");
	m_port = reg.GetKey("dev", "port", 10443);
}

void CMAVLink::Update()
{
	if (m_dev)
	{
		m_dev->Update();

		if (!m_bReceiver)
		{
			if (m_bConnected != m_dev->IsConnected())
			{
				m_bConnected = m_dev->IsConnected();

				WriteStatus("%s", m_bConnected ? "Connected" : "Disconnected");
			}
		}

		if (m_bConnected)
		{
		}
	}
}

void CMAVLink::WriteLog(LPCTSTR szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	CString str;
	str.FormatV(szFormat, args);
	va_end(args);

	pMainDlg->WriteLog(str);
}

void CMAVLink::WriteStatus(LPCTSTR szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	CString str;
	str.FormatV(szFormat, args);
	va_end(args);

	pMainDlg->WriteStatus(str);
}

BOOL CMAVLink::SendFile(LPCTSTR fileName)
{
	if (m_dev)
	{
		CTxFile* pFile = new CTxFile(fileName);
		if (pFile->Create())
		{
			m_dev->SendFile(pFile);
			return 1;
		}

		delete pFile;
	}

	return 0;
}

