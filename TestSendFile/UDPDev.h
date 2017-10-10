#pragma once

#define REMOTE_PORT	10000
#define MAV_SYSTEM_ID 1   //mavlink protocol parameters
#define MAV_COMPONENT_ID 1

const int NET_IDLE = 0;
const int NET_WAIT4TX = 1;
const int NET_WAIT4ANS = 2;
const int NET_DISCONN = 3;
const int NET_RESEND = 4;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CUDPSocket;
class CRingBuff;
class CMAVLink;
class CTxFile;
struct PayloadBlock;

class CUDPDev
{
public:
	CUDPDev(CMAVLink* pMavLink);
	virtual ~CUDPDev();

	virtual BOOL CUDPDev::Create(LPCTSTR ipAddr, UINT port);
	virtual BOOL CUDPDev::IsValid() { return m_sock != NULL; }
	virtual BOOL CUDPDev::IsConnected() { return m_bConnected; }
	virtual void CUDPDev::Update();
	virtual void CUDPDev::SendMsg(mavlink_message_t* msg);
	virtual void CUDPDev::SendMsgNoACK(mavlink_message_t* msg);
	virtual void CUDPDev::SendFile(CTxFile* pFile) {}
	virtual void CUDPDev::SendFileBlock(PayloadBlock* block);
	

protected:
	virtual void CUDPDev::OnIdle();
	virtual void CUDPDev::OnWaitTx();
	virtual void CUDPDev::OnWaitAns();
	virtual void CUDPDev::OnTimeOut();
	virtual void CUDPDev::ResetState();
	virtual void CUDPDev::ResendMsg();
	virtual void CUDPDev::UpdateTX();
	virtual void CUDPDev::Update(mavlink_message_t* pMsg);

	void CUDPDev::SendMsgTo(mavlink_message_t* msg);

	int m_stage;
	int m_nextStage;
	int	m_resendCnt;
	double m_lastTXTime;
	double m_lastRXTime;

	BOOL m_bConnected;
	mavlink_message_t m_lastMsg;

	CRingBuff* m_pRxBuf;
	CUDPSocket* m_sock;
	CMAVLink* m_pMavLink;
};

