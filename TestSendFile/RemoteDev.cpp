#include "stdafx.h"
#include "RemoteDev.h"
#include "UDPSocket.h"
#include "MAVLink.h"
#include "AppDef.h"
#include "TxFile.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CRemoteDev::CRemoteDev(CMAVLink* pMavLink) : CUDPDev(pMavLink)
{
	m_lastBeatTXTime = 0;
	m_lastRXMsgTime = 0;

	m_pTxFile = NULL;
}

CRemoteDev::~CRemoteDev()
{
	if (m_pTxFile)
		delete m_pTxFile;
}

BOOL CRemoteDev::Create(LPCTSTR ipAddr, UINT port)
{
	if (CUDPDev::Create(ipAddr, REMOTE_PORT))
	{
		m_sock->SetExpPort(port);
		return 1;
	}

	return 0;
}

void CRemoteDev::Update(mavlink_message_t* pMsg)
{
	m_bConnected = TRUE;
	m_lastRXMsgTime = dTime();

	switch (pMsg->msgid)
	{
	case MAVLINK_MSG_ID_HEARTBEAT: //#0
		break;

	case MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL:
		GetFileMsg(pMsg);
		break;
	}
}

void CRemoteDev::OnIdle()
{/*
 if (dTime() - m_lastBeatTXTime > 1000) //send heart beat each 1 sec
 {
 mavlink_message_t msg;
 mavlink_msg_heartbeat_pack(MAV_SYSTEM_ID, MAV_COMPONENT_ID, &msg, MAV_TYPE_GENERIC, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
 //SendMsg(&msg);
 SendMsgNoACK(&msg);

 m_lastBeatTXTime = dTime();
 }*/
}

void CRemoteDev::UpdateTX()
{
	if (dTime() - m_lastBeatTXTime > 1000) //send heart beat each 1 sec
	{
		mavlink_message_t msg;
		mavlink_msg_heartbeat_pack(MAV_SYSTEM_ID, MAV_COMPONENT_ID, &msg, MAV_TYPE_GENERIC, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
		SendMsgNoACK(&msg);

		m_lastBeatTXTime = dTime();
	}

	if (dTime() - m_lastRXMsgTime > 5000) //lost connection ?
	{
		m_bConnected = FALSE;
	}
}

void CRemoteDev::SendFile(CTxFile* pFile)
{
	m_pTxFile = pFile;

	PayloadBlock block;
	if (m_pTxFile->GetNextBlock(&block))
	{
		m_pMavLink->WriteLog("Start sending file %s", block.data);

		SendFileBlock(&block);
	}
}

void CRemoteDev::GetFileMsg(mavlink_message_t* msg)
{
	PayloadBlock block;
	mavlink_msg_file_transfer_protocol_get_payload(msg, (BYTE*)&block);

	if (m_pTxFile && m_pTxFile->m_fileId == block.fileId)
	{
		if (block.flags == MSG_ACK)
		{
			m_stage = NET_IDLE;

			if (m_pTxFile->IsEOF())
			{
				m_pMavLink->WriteLog("Send file completed");

				delete m_pTxFile;
				m_pTxFile = NULL;
			}
			else
			{
				PayloadBlock block;
				if (m_pTxFile->GetNextBlock(&block))
				{
					SendFileBlock(&block);

					m_pMavLink->WriteLog("Send block #%d", block.blockIdx);
				}
			}
		}
		else if (block.flags == MSG_NAK)
		{
			m_stage = NET_IDLE; //stop

			m_pMavLink->WriteLog("Receiver get error. Send file failed");

			delete m_pTxFile;
			m_pTxFile = NULL;
		}
	}
}

void CRemoteDev::ResendMsg()
{
	CUDPDev::ResendMsg();

	m_pMavLink->WriteLog("Resend block");
}

void CRemoteDev::OnTimeOut()
{
	CUDPDev::OnTimeOut();

	m_pMavLink->WriteLog("TimeOut");

	if (m_pTxFile)
	{
		delete m_pTxFile;
		m_pTxFile = NULL;
	}
}