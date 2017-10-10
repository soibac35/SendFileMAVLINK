
// TestSendFileDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "resource.h"


// CTestSendFileDlg dialog
class CMAVLink;

class CTestSendFileDlg : public CDialogEx
{
// Construction
public:
	CTestSendFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTSENDFILE_DIALOG };

	void CTestSendFileDlg::WriteLog(LPCTSTR szFormat, ...);
	void CTestSendFileDlg::WriteStatus(LPCTSTR szFormat, ...);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CMAVLink* CTestSendFileDlg::CreateDev();
	void CTestSendFileDlg::UpdateTitle();
	void CTestSendFileDlg::SaveSettings();
	void CTestSendFileDlg::LoadSettings();

// Implementation
protected:
	HICON m_hIcon;
	UINT m_timer;
	CListBox m_lbxLogs;
	CMAVLink* m_pDev;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	
	DECLARE_MESSAGE_MAP()
};

extern CTestSendFileDlg* pMainDlg;