#pragma once


class CRingBuff
{
public:
	CRingBuff(unsigned int size);
	~CRingBuff();

	void CRingBuff::AddData(char* dat, int datSize);

	mavlink_message_t* GetMsg();

protected:
	BOOL CRingBuff::SetFirstIndex(unsigned index);
	char* CRingBuff::GetBuffer(unsigned int from, unsigned int count);
	int CRingBuff::FindBuffer(char chr);

	char* ringBuff;
	char* linearBuff;

	unsigned int length;
	unsigned int firstIndex;
	unsigned int nextIndex;
	unsigned int maxBuffSize;

	mavlink_message_t curMsg;
};

