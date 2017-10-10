#pragma once
#include "UDPDev.h"

class CTxFile;

class CRemoteDev : public CUDPDev
{
public:
	CRemoteDev(CMAVLink* pMavLink);
	~CRemoteDev();

	BOOL Create(LPCTSTR ipAddr, UINT port);
	void Update(mavlink_message_t* pMsg);
	void CRemoteDev::SendFile(CTxFile* pFile);

protected:
	void CRemoteDev::UpdateTX();
	void CRemoteDev::GetFileMsg(mavlink_message_t* msg);
	void CRemoteDev::CloseSendingFile();

	void CRemoteDev::ResendMsg();
	void CRemoteDev::OnTimeOut();
	void CRemoteDev::OnIdle();

	double m_lastBeatTXTime;
	double m_lastRXMsgTime;

	CTxFile* m_pTxFile;
};

