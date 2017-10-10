#include "stdafx.h"
#include "RingBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAV_LINK_MSG_HEADER_SIZE 8

CRingBuff::CRingBuff(unsigned int size)
{
	maxBuffSize = size;
	firstIndex = 0;
	nextIndex = 0;
	length = 0;

	ringBuff = new char[maxBuffSize];
	linearBuff = new char[maxBuffSize];
}

CRingBuff::~CRingBuff()
{
	delete[] ringBuff;
	delete[] linearBuff;
}

void CRingBuff::AddData(char* dat, int datSize)
{
	char *src = dat;

	if (datSize > maxBuffSize)
	{
		src += datSize - maxBuffSize;
		datSize = maxBuffSize;
	}

	if (length + datSize > maxBuffSize)
	{
		int nRes = (length + datSize) % maxBuffSize;

		firstIndex = (firstIndex + nRes) % maxBuffSize;
		length = maxBuffSize;
	}
	else
		length += datSize;

	//prevIndex = nextIndex;

	for (UINT i = 0; i < datSize; i++)
	{
		*(ringBuff + nextIndex) = *(src + i);
		++nextIndex %= maxBuffSize;

		//nextIndex++;
		//nextIndex %= maxBuffSize;
	}
}

int CRingBuff::FindBuffer(char chr)
{
	for (int i = 0; i < length; i++)
	{
		int j = (firstIndex + i) % maxBuffSize;

		if (chr == ringBuff[j])
			return j;
	}

	return -1;
}

mavlink_message_t* CRingBuff::GetMsg()
{
	if (length > MAV_LINK_MSG_HEADER_SIZE)
	{
		int msgStartIndex = FindBuffer(MAVLINK_STX);

		if (msgStartIndex == -1)
			return NULL;

		char* msgBuf = GetBuffer(msgStartIndex % maxBuffSize, length);

		mavlink_message_t msg;
		mavlink_status_t status;

		for (int i = 0; i < length; ++i)
		{

			if (mavlink_parse_char(MAVLINK_COMM_0, msgBuf[i], &msg, &status))
			{
				int msgLength = msg.len + MAV_LINK_MSG_HEADER_SIZE;

				SetFirstIndex((msgStartIndex + msgLength) % maxBuffSize);

				memcpy(&curMsg, &msg, sizeof(msg));

				return &curMsg;
			}
		}
	}

	return NULL;
}

char* CRingBuff::GetBuffer(unsigned int from, unsigned int count)
{
	if (count > maxBuffSize)
		count = maxBuffSize;

	if (from + count < maxBuffSize)
		memcpy(linearBuff, ringBuff + from, count);
	else
	{
		UINT rest = from + count - maxBuffSize;
		UINT add = count - rest;

		memcpy(linearBuff, ringBuff + from, add);

		if (add < maxBuffSize)
			memcpy(linearBuff + add, ringBuff, rest);
	}

	return linearBuff;
}

BOOL CRingBuff::SetFirstIndex(unsigned index)
{
	if (index >= maxBuffSize)
		return 0;

	if (firstIndex >= nextIndex)
	{
		if (index<firstIndex && index > nextIndex)
			return 0;
	}
	else
		if (index < firstIndex || index > nextIndex)
			return 0;

	firstIndex = index;

	if (nextIndex > firstIndex)
		length = nextIndex - firstIndex;
	else if (nextIndex < firstIndex && length)
		length = nextIndex + maxBuffSize - firstIndex;
	else
		length = 0;

	return TRUE;
}

