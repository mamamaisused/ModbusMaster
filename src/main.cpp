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
	ModbusMaster node;
	uint8_t result;
	_stream.begin(115200);
	node.begin(1,_stream);
	printf("Receive: ");
	result = node.readHoldingRegisters(2, 6);
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

