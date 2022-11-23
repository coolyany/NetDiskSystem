#include "myProtocol.h"

PDU * getPDU(uint uiMsgLen)
{
	uint PDULen = sizeof(PDU) + uiMsgLen;
	PDU* pdu = (PDU*)malloc(PDULen);
	if (pdu == NULL)
	{
		exit(EXIT_FAILURE);
	}
	//内存清零
	memset(pdu, 0, PDULen);
	//结构体赋值
	pdu->PDULen = PDULen;
	pdu->MsgLen = uiMsgLen;

	return pdu;
}
