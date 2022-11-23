#include "QReadIni.h"
#pragma once

QReadIni* QReadIni::instance = nullptr;

QReadIni * QReadIni::getInstance()
{
	if (instance == NULL)
	{
		instance = new QReadIni();
	}
	return instance;
}

//构造
QReadIni::QReadIni() 
{
	this->readIni();
}

void QReadIni::readIni()
{
	//读取ini文件
	QSettings * configIniRead = new QSettings(":/config/client.ini", QSettings::IniFormat);//初始化读取Ini文件对象
	iniConfig.ip = configIniRead->value("conn/ip").toString();                          //IP地址
	iniConfig.port = configIniRead->value("conn/port").toString().toUShort();                    //端口
}

const IniConfig & QReadIni::getIniConfig()
{
	// TODO: 在此处插入 return 语句

	return iniConfig;
}
