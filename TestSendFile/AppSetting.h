#pragma once


class CAppSetting
{
public:
	CAppSetting(LPCTSTR fileName);
	~CAppSetting(void);

	BOOL IsValid();
	CString GetLocation();

	int GetKey(LPCTSTR sec, LPCTSTR key, int def);
	int GetKey(LPCTSTR sec, LPCTSTR key, COLORREF def);
	UINT GetKey(LPCTSTR sec, LPCTSTR key, UINT def);
	double GetKey(LPCTSTR sec, LPCTSTR key, double def);
	bool GetKey(LPCTSTR sec, LPCTSTR key, bool def);
	bool GetKey(LPCTSTR sec, LPCTSTR key, LPCTSTR def, CString& ret);
	bool GetKey(LPCTSTR sec, LPCTSTR key, void* p, int size);
	CString GetKey(LPCTSTR sec, LPCTSTR key, LPCTSTR def);

	bool SetKey(LPCTSTR sec, LPCTSTR key, COLORREF value);
	bool SetKey(LPCTSTR sec, LPCTSTR key, LPCTSTR value);
	bool SetKey(LPCTSTR sec, LPCTSTR key, UINT value);
	bool SetKey(LPCTSTR sec, LPCTSTR key, int value);
	bool SetKey(LPCTSTR sec, LPCTSTR key, double value);
	bool SetKey(LPCTSTR sec, LPCTSTR key, void* value, int size);

	bool ClearSection(LPCTSTR sec);
	bool ClearAll();

	static bool Clear(LPCTSTR fileName);

protected:
	CString m_fileName;

};

