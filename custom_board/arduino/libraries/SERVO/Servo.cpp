#include "Servo.h"


Servo::Servo()
{
	//empty
}

uint8_t Servo::attach(int pin)
{
    ch = pin;
	return ch;
}

void Servo::write(int angle)
{
    long value;

	if (angle < 0)
		angle = 0;
	else if (angle > 180)
		angle = 180;

	value = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
        analogWrite(ch, value);
	servoIndex = angle;		
		
	delayMicroseconds(SERVO_DELAY);
}

int Servo::read()
{
	return servoIndex;	
} 

bool Servo::attached()
{
	return((ch == prev_ch) ? 1 : 0);
}

void Servo::detach()
{
	return;
}