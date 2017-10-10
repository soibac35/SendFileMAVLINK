#pragma once
#include "UDPDev.h"

class CRxFile;

class CControllerDev : public CUDPDev
{
public:
	CControllerDev(CMAVLink* pMavLink);
	~CControllerDev();

	BOOL Create(LPCTSTR ipAddr, UINT port);
	void Update(mavlink_message_t* pMsg);
	void SendMsg(mavlink_message_t* msg);
	void CControllerDev::GetFileMsg(mavlink_message_t* msg);

protected:
	CRxFile* m_pRxFile;
};

