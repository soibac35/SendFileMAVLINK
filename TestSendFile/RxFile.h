#pragma once

struct PayloadBlock;

class CRxFile
{
public:
	CRxFile();
	~CRxFile();

	BOOL CRxFile::Create(PayloadBlock* block);
	BOOL CRxFile::Add(PayloadBlock* block);

	int m_curBlock;
	UINT m_fileId;

protected:
	CString m_fileName;
	CFile m_file;

};

