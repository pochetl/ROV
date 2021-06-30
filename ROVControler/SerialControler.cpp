#include "pch.h"
#include "SerialControler.h"
#define BUFFER_SIZE 50

bool SerialControler::writePacket(char outPutaray[], int size) {

	newSerial.Write(outPutaray, size);
	
	return true;

}



int  SerialControler::buildMotorPacket(float LeftMotor, float RightMotor, float motor3, float motor4, char outPutaray[]) {


	outPutaray[0] = 0xFE;
	outPutaray[1] = 0x8;
	outPutaray[2] = LeftMotor * 127;
	outPutaray[3] = RightMotor * 127;
	outPutaray[4] = motor3 * 127;
	outPutaray[5] = motor4 * 127;
	outPutaray[6] = 0;
	outPutaray[7] = 0;

	CString Temp;
	Temp.Format("%.1f %.1f", LeftMotor / 256, RightMotor / 256);
	OutputDebugString(Temp);

	int counter = 0;


	for (int i = 0; i < 8; i++) {
		counter = counter + outPutaray[i];
	}
	outPutaray[8] = counter&0xff;
	return 9;

}


bool SerialControler::test() {

	newSerial.Write("Chuck  ");
	return false;

}
	
UINT SerialControler::ParseROV() {
	
	
	unsigned long iBytesRead = 1;
	bool Continue = true;
	char dataout[BUFFER_SIZE];
	int length = 0;
	memset(dataout, 0, BUFFER_SIZE);



	while (Continue)
	{
		if (newSerial.Read(&dataout[length], BUFFER_SIZE - length, &iBytesRead) == ERROR_SUCCESS)
		{
			Sleep(10);
			CString temp;
			temp.Format("Got Bytes %d %d \n", iBytesRead, length);

			if (iBytesRead > 0) {
				OutputDebugString(temp);
			}
			else {
				OutputDebugString(".");
			}
			
		}
		else
		{
			Sleep(100);
		}


		length += iBytesRead;

		int BytesUsed = 1;




		while (length > 3 && BytesUsed > 0)
		{
	
			if ((dataout[0]&0xFF) == 0xFE) {

				length = length -4;
				//Send message
				memmove(&dataout[0], &dataout[4], length);
				OutputDebugString("Good");
			}
			else {
				length = length - 1;
				memmove(&dataout[0], &dataout[1], length);


			}



	

		}
	}
	return 0;
}



UINT Cycler(SerialControler* Parent)
{

	Parent->ParseROV();
	return 0;
}



bool SerialControler::start() {

	if (newSerial.IsOpen())
		newSerial.Close();
	
	
	
	m_Thread = AfxBeginThread((AFX_THREADPROC)Cycler, this, THREAD_PRIORITY_LOWEST);


	int count = 0;
	CString portname ="com4";
	portname.MakeUpper();
	portname.Insert(0, "\\\\.\\");
	if (newSerial.Open(portname, 1024, 1024, false) == ERROR_SUCCESS)
	{

	}
	else
	{
		AfxMessageBox("No serial Port");
	}

	if (!newSerial.IsOpen())
	{

		//report problem and quit?
		return 0;
	}
	newSerial.SetupHandshaking(CSerial::EHandshakeOff);
	newSerial.Setup(CSerial::EBaudrate(115200), CSerial::EData8, CSerial::EParNone, CSerial::EStop1);

	return false;
}



bool SerialControler::stop(){
	return false;
}