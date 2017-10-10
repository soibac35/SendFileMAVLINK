#include "stdafx.h"
#include "UDPDev.h"
#include "UDPSocket.h"
#include "RingBuff.h"
#include "AppSetting.h"
#include "AppDef.h"
#include "MAVLink.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RESEND_MAX			3
#define TRANSFER_TIMEOUT	5000 //5 sec

CUDPDev::CUDPDev(CMAVLink* pMavLink)
{
	m_pMavLink = pMavLink;
	m_bConnected = 0;

	m_resendCnt = 0;
	m_lastTXTime = 0;
	m_lastRXTime = 0;
	m_stage = NET_IDLE;

	m_sock = NULL;
	m_pRxBuf = new CRingBuff(2048);
}

CUDPDev::~CUDPDev()
{
	if (m_sock)
		delete m_sock;

	delete m_pRxBuf;
}

BOOL CUDPDev::Create(LPCTSTR ipAddr, UINT port)
{
	CUDPSocket* sock = new CUDPSocket();

	if (!sock->Create(ipAddr, port))
	{
		delete sock;
		return 0;
	}

	if (m_sock)
		delete m_sock;

	m_sock = sock;

	return 1;
}

void CUDPDev::Update()
{
	if (!m_sock)
		return;

	m_sock->Read(m_pRxBuf);

	while (1)
	{
		mavlink_message_t* pMsg = m_pRxBuf->GetMsg();

		if (!pMsg)
			break;

		Update(pMsg);
	}

	switch (m_stage)
	{
		case NET_IDLE:
			OnIdle();
			break;

		case NET_WAIT4TX:
			OnWaitTx();
			break;

		case NET_WAIT4ANS:
			OnWaitAns();
			break;

		case NET_RESEND:
			ResendMsg();
			break;

		case NET_DISCONN:
			ResetState();
			break;
	}

	UpdateTX();
}

void CUDPDev::UpdateTX()
{

}
void CUDPDev::Update(mavlink_message_t* pMsg)
{
}

void CUDPDev::OnIdle()
{
}

void CUDPDev::OnWaitTx()
{
}

void CUDPDev::OnWaitAns()
{
	if (dTime() - m_lastTXTime > TRANSFER_TIMEOUT)
	{
		if (m_resendCnt < RESEND_MAX)
		{
			ResendMsg();
		}
		else
			OnTimeOut();
	}
}

void CUDPDev::OnTimeOut()
{
	ResetState();
}

void CUDPDev::ResetState()
{
	m_lastTXTime = 0;
	m_resendCnt = 0;
	m_stage = NET_IDLE;
}

void CUDPDev::ResendMsg()
{
	SendMsgTo(&m_lastMsg);

	m_resendCnt++;
}

void CUDPDev::SendMsg(mavlink_message_t* msg)
{
	SendMsgTo(msg);

	memcpy(&m_lastMsg, msg, sizeof(mavlink_message_t));

	m_resendCnt = 0;
}

void CUDPDev::SendMsgTo(mavlink_message_t* msg)
{
	unsigned char buf[MAVLINK_MAX_PACKET_LEN];
	int bufSize = mavlink_msg_to_send_buffer(buf, msg);

	m_sock->Write(buf, bufSize, m_pMavLink->m_port, m_pMavLink->m_addr);

	m_stage = NET_WAIT4ANS;
	m_lastTXTime = dTime();
}

void CUDPDev::SendMsgNoACK(mavlink_message_t* msg)
{
	unsigned char buf[MAVLINK_MAX_PACKET_LEN];
	int bufSize = mavlink_msg_to_send_buffer(buf, msg);

	m_sock->Write(buf, bufSize, m_pMavLink->m_port, m_pMavLink->m_addr);
}

void CUDPDev::SendFileBlock(PayloadBlock* block)
{
	mavlink_message_t msg;
	mavlink_msg_file_transfer_protocol_pack(MAV_SYSTEM_ID, MAV_COMPONENT_ID, &msg, 0, 0, 0, (BYTE*)block);

	SendMsg(&msg);
}

