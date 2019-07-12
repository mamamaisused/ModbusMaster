/**
 @author : MAMAMA
 @date: 2019-07-12
 @description: though the name of this file is Arduino.h, it is used to run ModbusMaster.h in windows environment
*/

#ifndef _ARDUINO_H_
#define _ARDUINO_H_

//Uncommit the next line to enable debug output
#define DEBUG_ENABLE

#include "windows.h"
#include "stdio.h"
#include "string.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

class Stream
{
private:
	HANDLE _hcom;
	DCB _com_dcb_;
	DWORD _read_buffer_size;
	DWORD _send_buffer_size;
	const char* _port_name;
	void _init(const char* portname, 
			int baudrate,
			char parity, 
			char stopbit);
public:
	Stream(const char* portname);
	Stream(	const char* portname, 
			int baudrate,
			char parity, 
			char stopbit);
	~Stream(void);
	void flush(void);
	bool begin(int baudrate);
	void close(void);
	DWORD write(uint8_t data);
	uint8_t read(void);
	DWORD available(void);
};

/**
@param : input data
@return : low byte of the data
*/
uint8_t lowByte(uint16_t data);

/**
@param : input data
@return : high byte of the data
*/
uint8_t highByte(uint16_t data);

/**
@descripion: read bit value of data
@param :
	data : data to read
	n : bit to get
	b(for bitWrite) : value to write
@return : high byte of the data
*/
bool bitRead(uint8_t data, uint8_t n);
void bitWrite(uint16_t &data, uint8_t n, bool b);
uint16_t word(uint8_t highbyte, uint8_t lowbyte);
uint16_t word(uint8_t lowbyte);
uint32_t millis(void);

#endif
/* end of file */

