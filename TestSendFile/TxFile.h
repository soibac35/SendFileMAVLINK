#pragma once

struct PayloadBlock;

class CTxFile
{
public:
	CTxFile(LPCTSTR fileName);
	~CTxFile();

	BOOL CTxFile::Create();
	BOOL CTxFile::GetNextBlock(char* buf, UINT& bufSize);
	BOOL CTxFile::GetNextBlock(PayloadBlock* block);
	BOOL CTxFile::IsEOF() { return m_eof; }
	
	int m_curBlock;
	UINT m_fileId;

protected:
	BOOL m_eof;
	CString m_fileName;
	CFile m_file;
};

