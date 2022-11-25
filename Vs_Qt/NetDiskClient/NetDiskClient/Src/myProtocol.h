#pragma once
/* 自定义通信协议 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define REGISTER_OK			"register ok"
#define REGISTER_FAILED		"register failed, name is existed"
#define LOGIN_OK			"login ok"
#define LOGIN_FAILED		"login failed"

enum ENUM_MSG_TYPE {
	ENUM_MSG_TYPE_MIN = 0,

	ENUM_MSG_TYPE_REGIST_REQUEST,	//注册请求
	ENUM_MSG_TYPE_REGIST_RESPONSE,  //注册回复
	ENUM_MSG_TYPE_LOGIN_REQUEST,  //登录请求
	ENUM_MSG_TYPE_LOGIN_RESPONSE,  //登录回复

	ENUM_MSG_TYPE_MAX = 0x00ffffff
};

typedef struct protocol_data_unit{
	uint PDULen;//整个PDU大小
	uint MsgType;//信息类型
	char caData[64];
	uint MsgLen;//信息大小
	int caMsg[];//动态信息内容

} PDU;

PDU* getPDU(uint uiMsgLen);//传入信息大小，分配PUD内存，得到结构体指针，