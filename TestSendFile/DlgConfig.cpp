// DlgConfig.cpp : implementation file
//

#include "stdafx.h"
#include "TestSendFile.h"
#include "DlgConfig.h"
#include "MAVLink.h"
#include "afxdialogex.h"


// CDlgConfig dialog

IMPLEMENT_DYNAMIC(CDlgConfig, CDialogEx)

CDlgConfig::CDlgConfig(CMAVLink* dev, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConfig::IDD, pParent)
	, m_ipAddr(_T(""))
	, m_port(0)
	, m_bReceiver(FALSE)
{
	if (dev)
	{
		m_ipAddr = dev->m_addr;
		m_port = dev->m_port;
		m_bReceiver = dev->m_bReceiver;
	}
}

CDlgConfig::~CDlgConfig()
{
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IPADDR, m_ipAddr);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Check(pDX, IDC_CHECK_RECEIVER, m_bReceiver);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgConfig message handlers


void CDlgConfig::OnBnClickedOk()
{
	UpdateData();


	CDialogEx::OnOK();
}
