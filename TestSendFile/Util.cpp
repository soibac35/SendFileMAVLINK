#include "stdafx.h"
#include "Util.h"
#include <math.h>
#include <sys/timeb.h> 
#include <time.h>


CUtil::CUtil()
{
}

CUtil::~CUtil()
{
}

double dTime()
{
	struct _timeb ftime;
	_ftime_s(&ftime);

	return (ftime.time*1000.0 + ftime.millitm);
}

BYTE CRCChecksum(BYTE *buf, int bufSize)
{
	int idx = 0;
	unsigned char chkSum = 0;

	while (idx < bufSize)
	{
		//chkSum ^= *str;
		chkSum += *buf;
		buf++;
		idx++;
	}

	return ~chkSum;
}

void GetAppPath(CString& appPath)
{
	char buff[_MAX_PATH];
	::GetModuleFileName(NULL, buff, _MAX_PATH);

	CString szPathName(buff);
	int pos = szPathName.ReverseFind('\\');

	if (pos != -1)
		szPathName = szPathName.Left(pos);

	appPath = szPathName;
}
