#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

typedef struct protocol_data_unit{
	uint PDULen;//整个PDU大小
	uint MsgType;//信息类型
	char caData[64];
	uint MsgLen;//信息大小
	int caMsg[];//动态信息内容

} PDU;

PDU* getPDU(uint uiMsgLen);//传入信息大小，分配PUD内存，得到结构体指针，