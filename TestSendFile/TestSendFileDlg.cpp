
// TestSendFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestSendFile.h"
#include "TestSendFileDlg.h"
#include "DlgConfig.h"
#include "AppSetting.h"
#include "MAVLink.h"
#include "AppDef.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestSendFileDlg* pMainDlg = NULL;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestSendFileDlg dialog



CTestSendFileDlg::CTestSendFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestSendFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_timer = 0;
	m_pDev = NULL;
	pMainDlg = this;;
}

void CTestSendFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGS, m_lbxLogs);
}

BEGIN_MESSAGE_MAP(CTestSendFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CTestSendFileDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CTestSendFileDlg::OnBnClickedButtonSend)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestSendFileDlg message handlers

BOOL CTestSendFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_pDev = CreateDev();

	UpdateTitle();

	m_timer = SetTimer(100, 100, NULL);

	TRACE("test %d, %d\n", sizeof(uint16_t), sizeof(PayloadBlock));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestSendFileDlg::UpdateTitle()
{
	CString desc;
	m_pDev->GetDesc(desc);

	SetWindowText(desc);
}

void CTestSendFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestSendFileDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestSendFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestSendFileDlg::SaveSettings()
{
}

void CTestSendFileDlg::LoadSettings()
{
}

void CTestSendFileDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pDev)
		m_pDev->Update();

	CDialogEx::OnTimer(nIDEvent);
}


void CTestSendFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


void CTestSendFileDlg::OnDestroy()
{
	SaveSettings();

	if (m_timer > 0)
		KillTimer(m_timer);

	CDialogEx::OnDestroy();
}

void CTestSendFileDlg::OnBnClickedButtonConfig()
{
	if (!m_pDev)
		m_pDev = CreateDev();

	CDlgConfig dlg(m_pDev);
	if (dlg.DoModal() == IDOK)
	{
		if (m_pDev->ChangeSettings(dlg.m_ipAddr, dlg.m_port, dlg.m_bReceiver))
		{ 
			WriteLog("Change settings to %s:%d", dlg.m_ipAddr, dlg.m_port);
		}

		UpdateTitle();
	}
}

void CTestSendFileDlg::OnBnClickedButtonSend()
{
	CFileDialog dlg(TRUE, "py", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Python Files(*.py)|*.py|All Files(*.*)|*.*|", NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();

		//WriteLog("Sending file %s", dlg.GetFileName());

		m_pDev->SendFile(fileName);
	}
}

void CTestSendFileDlg::WriteLog(LPCTSTR szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	CString str;
	str.FormatV(szFormat, args);
	va_end(args);

	int idx = m_lbxLogs.AddString(str);
	m_lbxLogs.SetCurSel(idx);
}

void CTestSendFileDlg::WriteStatus(LPCTSTR szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	CString str;
	str.FormatV(szFormat, args);
	va_end(args);

	SetDlgItemText(IDC_STATIC_STATUS, str);
}

CMAVLink* CTestSendFileDlg::CreateDev()
{
	CMAVLink* dev = new CMAVLink;
	dev->Create();

	return dev;
}

void CTestSendFileDlg::OnCancel()
{
	if (m_pDev)
	{
		delete m_pDev;
		m_pDev = NULL;
	}

	CDialogEx::OnCancel();
}
