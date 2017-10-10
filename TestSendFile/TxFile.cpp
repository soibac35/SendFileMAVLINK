#include "stdafx.h"
#include "TxFile.h"
#include "AppDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTxFile::CTxFile(LPCTSTR fileName)
{
	m_fileName = fileName;
	m_fileId = 0;
	m_curBlock = 0;
	m_eof = FALSE;
}

CTxFile::~CTxFile()
{
}

BOOL CTxFile::Create()
{
	if (!m_file.Open(m_fileName, CFile::modeRead))
	{
		return 0;
	}

	long size = m_file.GetLength();
	long blockCnt = size / TX_DATA_SIZE;

	if (size % TX_DATA_SIZE)
		blockCnt++;

	return 1;
}

BOOL CTxFile::GetNextBlock(char* buf, UINT& bufSize)
{
	UINT readSize = m_file.Read(buf, bufSize);
	if (readSize == 0)
		return 0;

	m_curBlock++;

	return 1;
}

BOOL CTxFile::GetNextBlock(PayloadBlock* block)
{
	if (m_curBlock == 0)
	{
		//first block contains file name
		int idx = m_fileName.ReverseFind('\\');
		if (idx != -1)
		{
			CString fName = m_fileName.Mid(idx + 1);
			strcpy(block->data, fName);

			block->datSize = fName.GetLength();
			block->fileId = m_fileId;
			block->blockIdx = 0;
			block->flags = 0;

			m_curBlock++;

			return 1;
		}
	}
	else
	{
		UINT readSize = m_file.Read(block->data, TX_DATA_SIZE);
		if (readSize > 0)
		{
			block->datSize = (BYTE)readSize;
			block->fileId = m_fileId;
			block->blockIdx = m_curBlock++;
			block->flags = 1;

			if (readSize < TX_DATA_SIZE)
			{
				block->flags = 0xFF;
				m_eof = TRUE;
			}

			return 1;
		}
	}

	return 0;
}


