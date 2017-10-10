#include "stdafx.h"
#include "RxFile.h"
#include "Util.h"
#include "AppDef.h"


CRxFile::CRxFile()
{
	m_curBlock = 0;
	m_fileId = 0;
}

CRxFile::~CRxFile()
{
}

BOOL CRxFile::Create(PayloadBlock* block)
{
	CString appPath;
	GetAppPath(appPath);

	m_fileName.Format("%s\\%s", appPath, block->data);

	if (!m_file.Open(m_fileName, CFile::modeCreate | CFile::modeWrite))
		return 0;

	m_fileId = block->fileId;

	return 1;
}

BOOL CRxFile::Add(PayloadBlock* block)
{
	if (m_fileId != block->fileId)
		return 0;

	if (m_curBlock + 1 != block->blockIdx)
		return 0;

	m_file.Write(block->data, block->datSize);

	m_curBlock = block->blockIdx;

	return 1;
}
