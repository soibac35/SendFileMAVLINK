#pragma once


// CDlgConfig dialog
class CMAVLink;

class CDlgConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CMAVLink* dev, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgConfig();

// Dialog Data
	enum { IDD = IDD_CONFIG };

	CString m_ipAddr;
	UINT m_port;
	BOOL m_bReceiver;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
};
