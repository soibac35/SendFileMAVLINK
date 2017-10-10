#include "stdafx.h"
#include "ControllerDev.h"
#include "UDPSocket.h"
#include "MAVLink.h"
#include "AppDef.h"
#include "RxFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CControllerDev::CControllerDev(CMAVLink* pMavLink) : CUDPDev(pMavLink)
{
	m_pRxFile = NULL;
}

CControllerDev::~CControllerDev()
{
	if (m_pRxFile)
		delete m_pRxFile;
}

BOOL CControllerDev::Create(LPCTSTR ipAddr, UINT port)
{
	if (CUDPDev::Create(ipAddr, port))
	{
		m_sock->SetExpPort(REMOTE_PORT);
		return 1;
	}

	return 0;
}

void CControllerDev::Update(mavlink_message_t* pMsg)
{
	switch (pMsg->msgid)
	{
		case MAVLINK_MSG_ID_HEARTBEAT: //#0
			//m_pMavLink->WriteLog("Heart beat");

			mavlink_message_t msg;
			mavlink_msg_heartbeat_pack(MAV_SYSTEM_ID, MAV_COMPONENT_ID, &msg, MAV_TYPE_GENERIC, MAV_AUTOPILOT_GENERIC, MAV_MODE_GUIDED_ARMED, 0, MAV_STATE_ACTIVE);
			SendMsg(&msg);
			break;

		case MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL:
			GetFileMsg(pMsg);
			break;
	}
}

void CControllerDev::SendMsg(mavlink_message_t* msg)
{
	unsigned char buf[MAVLINK_MAX_PACKET_LEN];
	int bufSize = mavlink_msg_to_send_buffer(buf, msg);

	m_sock->Write(buf, bufSize, REMOTE_PORT, m_pMavLink->m_addr);
}

void CControllerDev::GetFileMsg(mavlink_message_t* msg)
{
	PayloadBlock block;
	mavlink_msg_file_transfer_protocol_get_payload(msg, (BYTE*)&block);

	if (block.blockIdx == 0) //first block contains file name
	{
		if (!m_pRxFile)
		{
			m_pRxFile = new CRxFile();

			if (m_pRxFile->Create(&block))
			{
				PayloadBlock res;
				res.fileId = block.fileId;
				res.blockIdx = block.blockIdx;
				res.datSize = 0;
				res.flags = MSG_ACK;

				SendFileBlock(&res);

				m_pMavLink->WriteLog("Start recv file %s", block.data);
			}
			else
			{
				PayloadBlock res;
				res.fileId = block.fileId;
				res.blockIdx = block.blockIdx;
				res.datSize = 0;
				res.flags = MSG_NAK;

				SendFileBlock(&res);

				delete m_pRxFile;
				m_pRxFile = NULL;

				m_pMavLink->WriteLog("Error: can't recv file %s", block.data);
			}
		}
	}
	else if (m_pRxFile && m_pRxFile->Add(&block))
	{
		m_pMavLink->WriteLog("Recv block #%d", block.blockIdx);

		if (block.flags == 0xFF)
		{
			m_pMavLink->WriteLog("End of rev file");

			delete m_pRxFile;
			m_pRxFile = NULL;
		}

		PayloadBlock res;
		res.fileId = block.fileId;
		res.blockIdx = block.blockIdx;
		res.datSize = 0;
		res.flags = MSG_ACK;

		SendFileBlock(&res);
	}
}

