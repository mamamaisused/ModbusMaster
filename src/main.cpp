/**
@author : MAMAMA
@date: 2019-07-12
@description: control Panasonic DC motors via Modbus RTU
*/

#include "stdio.h"
#include "ModbusMaster.h"

int main(int argc, char* argv[])
{
	Stream _stream("COM4");
	uint8_t low = 0x0;
	uint8_t high = 0x99;
	uint16_t highlow = 0xFFFF;

	printf("hello, world!\n");
	_stream.begin(9600);
	for(int j=0; j<8 ; j++)
	{
		bitWrite(highlow,j,false);
		for(int i =7; i>=0 ; i--)
			printf("%d",bitRead(lowByte(highlow),i));
		printf("\n");
		_stream.write(3);
		_stream.flush();
	}
	printf("Receive: ");
	
	/* infinite loop */
	while(1)
	{
		if(_stream.available()>0)
		{
			uint8_t data;
			data = _stream.read();

		}
	}
	return 0;
}

