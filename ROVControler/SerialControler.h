#pragma once

#include "EnumSer.h"

#include "Serial.h"
class SerialControler
{public:
	int buildMotorPacket(float motor1, float motor2, float motor3, float motor4, char outPutaray[]);
	bool start();
	bool stop();
	bool test();
	UINT ParseROV();
	CWinThread* m_Thread;
	bool writePacket(char outPutaray[], int size);
private:
	CSerial newSerial; 
};

