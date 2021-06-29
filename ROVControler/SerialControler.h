#pragma once

#include "EnumSer.h"

#include "Serial.h"
class SerialControler
{public:
	int buildMotorPacket(short motor1, short motor2, short motor3, short motor4, char outPutaray[]);
	bool start();
	bool stop();
	bool test();
	UINT ParseROV();
	CWinThread* m_Thread;
	bool writePacket(char outPutaray[], int size);
private:
	CSerial newSerial; 
};

