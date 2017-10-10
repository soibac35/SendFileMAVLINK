#include "stdafx.h"
#include "AppSetting.h"
#include "Util.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAppSetting::CAppSetting(LPCTSTR fileName)
{
	CString appPath;
	GetAppPath(appPath);

	m_fileName = appPath + "\\" + fileName;
}

CAppSetting::~CAppSetting(void)
{
}

BOOL CAppSetting::IsValid()
{
	return _access(m_fileName, 0) == 0;
}

CString CAppSetting::GetLocation()
{
	int idx = m_fileName.ReverseFind('\\');

	return m_fileName.Left(idx + 1);
}

int CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, int def)
{
	int val = GetPrivateProfileInt(sec, key, def, m_fileName);

	return val;
}

UINT CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, UINT def)
{
	int val = GetPrivateProfileInt(sec, key, def, m_fileName);

	return val;
}

int CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, COLORREF def)
{
	int val = GetPrivateProfileInt(sec, key, def, m_fileName);

	return val;
}

double CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, double def)
{
	char buffer[80];

	DWORD count = GetPrivateProfileString(sec, key, NULL, buffer, 80, m_fileName);

	if (count > 0)
		return atof(buffer);

	return def;
}

bool CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, bool def)
{
	int val = GetPrivateProfileInt(sec, key, def, m_fileName);

	return val == 0 ? false : true;
}

CString CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, LPCTSTR def)
{
	char buffer[1024];
	GetPrivateProfileString(sec, key, def, buffer, 1024, m_fileName);

	return CString(buffer);
}

bool CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, LPCTSTR def, CString& ret)
{
	char buffer[1024];
	GetPrivateProfileString(sec, key, NULL, buffer, 1024, m_fileName);

	if (buffer != NULL)
	{
		ret = buffer;
		return true;
	}

	return false;
}

bool CAppSetting::GetKey(LPCTSTR sec, LPCTSTR key, void* p, int size)
{
	return GetPrivateProfileStruct(sec, key, p, size, m_fileName) ? true : false;
}

bool CAppSetting::SetKey(LPCTSTR sec, LPCTSTR key, LPCTSTR value)
{
	return WritePrivateProfileString(sec, key, value, m_fileName) ? true : false;
}

bool CAppSetting::SetKey(LPCTSTR sec, LPCTSTR key, int value)
{
	CString szValue;
	szValue.Format("%d", value);

	return SetKey(sec, key, "" + szValue);
}

bool CAppSetting::SetKey(LPCTSTR sec, LPCTSTR key, UINT value)
{
	CString szValue;
	szValue.Format("%d", value);

	return SetKey(sec, key, "" + szValue);
}

bool CAppSetting::SetKey(LPCTSTR sec, LPCTSTR key, COLORREF value)
{
	CString szValue;
	szValue.Format("%d", value);

	return SetKey(sec, key, "" + szValue);
}

bool CAppSetting::SetKey(LPCTSTR sec, LPCTSTR key, double value)
{
	CString szValue;
	szValue.Format("%f", value);

	return SetKey(sec, key, szValue);
}

bool CAppSetting::SetKey(LPCTSTR sec, LPCTSTR key, void* value, int size)
{
	return WritePrivateProfileStruct(sec, key, value, size, m_fileName) ? true : false;
}

bool CAppSetting::ClearSection(LPCTSTR sec)
{
	return WritePrivateProfileSection(sec, "", m_fileName) ? true : false;
}

bool CAppSetting::ClearAll()
{
	return _unlink(m_fileName) == 0 ? true : false;
}

bool CAppSetting::Clear(LPCTSTR fileName)
{
	return _unlink(fileName) == 0 ? true : false;
}
