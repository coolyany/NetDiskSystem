#pragma once
#include<QSettings>

typedef struct IniConfig {
	QString ip;
	quint16 port;

}IniConfig;


//单例模式
class QReadIni {
public:
	static QReadIni* getInstance();
	void readIni();
	const IniConfig &getIniConfig();

private:
	QReadIni();
	IniConfig iniConfig;
	static QReadIni* instance;
};