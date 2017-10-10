#pragma once


class CUtil
{
public:
	CUtil();
	~CUtil();
};

double dTime();
BYTE CRCChecksum(BYTE *buf, int bufSize);
void GetAppPath(CString& appPath);
