/**
@author : MAMAMA
@date: 2019-07-12
@description: control Panasonic DC motors via Modbus RTU
*/

//#define USE232
#define USE485

#include <stdio.h>
#include <conio.h>
#include "ModbusMaster.h"
#include "PanasonicMotor.h"

#define KEY_ESC 27
#define KEY_I 105
#define KEY_K 107
#define KEY_O 111
#define KEY_L 108
#define KEY_SPACE 32

void motorSpeedUp(PanasonicMotor &motor);
void motorSpeedDown(PanasonicMotor &motor);
void motorStop(PanasonicMotor &motor);

/**
see details of com name
https://github.com/ayowin/WzSerialPort
*/
#define RS485PORT "\\\\.\\COM14"
#define RS232PORT "COM4"

int main(int argc, char* argv[])
{
	int keyval;
	bool exitflag = false;
	//int motorvel = 0;

#ifdef USE485
	Stream stream(RS485PORT);
#elif defined USE232
	Stream stream(RS232PORT);
#endif	

	if(stream.begin(115200))
	{
		ModbusMaster node;
		//create motor instance, device modbus address 0x01, use stream to send command
		PanasonicMotor motor1(1,&stream);
		PanasonicMotor motor2(2,&stream);

		motor1.connectMotor();
		motor2.connectMotor();		
		printf("Receive: ");
		//motor.setVeloctiy(motorvel);
		/* infinite loop */
		while(!exitflag)
		{
			if(_kbhit())
			{
				keyval = _getch();
				switch(keyval)
				{
				case KEY_ESC:
					exitflag = true;
					break;
				case KEY_I:
					motorSpeedUp(motor1);
					break;
				case KEY_K:
					motorSpeedDown(motor1);
					break;
				case KEY_O:
					motorSpeedUp(motor2);
					break;
				case KEY_L:
					motorSpeedDown(motor2);
					break;
				case KEY_SPACE:
					motorStop(motor1);
					motorStop(motor2);
					break;
				default:
					break;
				}
			}
			if(stream.available()>0)
			{
				uint8_t data;
				data = stream.read();
			}
		}
	}
	printf("[INFO]: System terminated.\n");
	return 0;
}

void motorSpeedUp(PanasonicMotor &motor)
{
	int vel;
	vel = motor.getMotorCommandVel();
	vel += 50;
	motor.setVeloctiy(vel);
}

void motorSpeedDown(PanasonicMotor &motor)
{
	int vel;
	vel = motor.getMotorCommandVel();
	vel -= 50;
	motor.setVeloctiy(vel);
}

void motorStop(PanasonicMotor &motor)
{
	motor.setVeloctiy(0);
}

