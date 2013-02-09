/*
 * Example for using UAS_serial class
 */

#include "../UAS_serial.hpp"
#include <iostream>

using namespace std;

UAS_serial Serial1("/dev/ttyACM0");

int main(){
	Serial1.beginPort(115200);
	static unsigned char readByte;
	static uint8_t sendByte = 40;
	for (int i = 0; i < 100; ++i)
	{
		Serial1.send(&sendByte);
		Serial1.fetch(&readByte);
		cout<<readByte<<endl;
	}
	Serial1.closePort();
	return 0;
}