/*
 * Example for using UAS_serial class
 */

#include "../UAS_serial.hpp"
#include <iostream>

UAS_serial Serial1('/dev/ttyACM0',115200);

int main(){
	static unsigned char readByte;
	for (int i = 0; i < 100; ++i)
	{
		Serial1.send('a');
		Serial1.fetch(readByte);
		cout<<readByte<<endl;
	}
}