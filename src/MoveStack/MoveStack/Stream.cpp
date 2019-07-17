/**
@author : MAMAMA
@date: 2019-07-12
@description: transmition though searial port
*/
#include "Arduino.h"

void Stream::_init(const char* portname, 
				   int baudrate,
				   char parity, 
				   char stopbit)
{
	_hcom = NULL;
	_read_buffer_size = 1024;
	_send_buffer_size = 1024;
	_port_name = portname;
	_com_dcb_.BaudRate = baudrate;
	_com_dcb_.ByteSize = 8;
	_com_dcb_.Parity = parity;
	_com_dcb_.StopBits = stopbit;	
}


Stream::Stream(const char* portname)
{
	_init(portname,9600,EVENPARITY,ONESTOPBIT);
}

Stream::Stream(const char* portname, 
			   int baudrate,
			   char parity, 
			   char stopbit)
{
	_init(portname,baudrate,parity,stopbit);
}

Stream::~Stream(void)
{
	;
}

bool Stream::begin(int baudrate)
{
	DCB sysdcb;
	_com_dcb_.BaudRate = baudrate;
	_hcom = CreateFileA(	_port_name, //串口名
		GENERIC_READ | GENERIC_WRITE, //支持读写
		0, //独占方式，串口不支持共享
		NULL,//安全属性指针，默认值为NULL
		OPEN_EXISTING, //打开现有的串口文件
		0, //0：同步方式，FILE_FLAG_OVERLAPPED：异步方式
		NULL);//用于复制文件句柄，默认值为NULL，对串口而言该参数必须置为NULL
	if(_hcom == INVALID_HANDLE_VALUE)
	{
		printf("[Error]: Open Port Error!\n");
		return false;
	}
	if(!SetupComm(_hcom,_read_buffer_size,_send_buffer_size))
	{
		printf("[Error]: Set Buffer Error!\n");
		return false;
	}
	/* first get com state then set it. */
	GetCommState(_hcom, &sysdcb);
	sysdcb.BaudRate = _com_dcb_.BaudRate;
	sysdcb.Parity = _com_dcb_.Parity;
	sysdcb.StopBits = _com_dcb_.StopBits;
	sysdcb.ByteSize = _com_dcb_.ByteSize;
	if(!SetCommState(_hcom,&sysdcb))
	{
		printf("[Error]: Set Parameter False!\n");
		return false;
		
	}
	return true;
}


void Stream::close(void)
{
	if( _hcom != NULL && _hcom != INVALID_HANDLE_VALUE)
		CloseHandle(_hcom);
}

DWORD Stream::write(uint8_t data)
{
	DWORD ret;
	if(!WriteFile(_hcom,&data,1,&ret,NULL))
		ret = 0;
	return ret;
}

DWORD Stream::available(void)
{
	COMSTAT _comstat;
	DWORD _error;
	if(ClearCommError(_hcom,&_error,&_comstat))
		return _comstat.cbInQue;
	else
		return 0;
}

uint8_t Stream::read(void)
{
	uint8_t data;
	DWORD _datacnt;
	if(!ReadFile(_hcom,&data,1,&_datacnt,NULL))
	{
		return 0;
		printf("[WARNING]:No Data Received !\n");
	}
	else
	{
#ifdef DEBUG_ENABLE
		printf("%02X ",data);
#endif
		return data;
	}
}

void Stream::flush(void)
{
	COMSTAT _comstat;
	DWORD _error;
	if(ClearCommError(_hcom,&_error,&_comstat))
	{
		while(_comstat.cbOutQue > 0)
		{
			ClearCommError(_hcom,&_error,&_comstat);
		}
	}
	else
		return;
}

uint32_t millis(void)
{
	return ::GetTickCount();
}

uint16_t word(uint8_t highbyte, uint8_t lowbyte)
{
	uint16_t ret;
	ret = (uint16_t)highbyte;
	ret = (ret << 8) + (uint16_t)lowbyte;
	return ret;
}

uint16_t word(uint8_t lowbyte)
{
	return word(0, lowbyte);
}

uint8_t lowByte(uint16_t data)
{
	uint8_t ret;
	ret = 0x00FF & data;
	return ret;
}

uint8_t highByte(uint16_t data)
{
	uint8_t ret;
	ret = (0xFF00 & data) >> 8;
	return ret;
}

bool bitRead(uint8_t data, uint8_t n)
{
	uint8_t ret = 1;
	ret = (ret << n) & data;
	ret = ret >> n;
	if(ret == 0)
		return false;
	else
		return true;
}

void bitWrite(uint16_t &data, uint8_t n, bool b)
{
	uint16_t mask;
	if(b)
	{
		mask = 1;
		data = data | (mask << n);
	}
	else
	{
		mask = 0xFFFE;
		data = data & (mask << n);
	}
	return;
}