#include "Servo.h"
#include "Arduino.h"
#include "platform.h"


Servo::Servo()
{
	//empty
}

uint8_t Servo::attach(int pin)
{
    ch = pin;
    uint32_t pwm_num = variant_pin_map[ch].pwm_num;

    pinMode(pin, OUTPUT); // declare pwm pin to be an output:
    min = SERVO_MIN;
    max = SERVO_MAX;
   // Since PWM register does not reset immeditaly with change, we are reducing the
    // Timing tick from 1us to 200ns to run the PWM logic run 5 time faster - this timer
    // value need to corrected for MPW-7 onward - Atten - Dinesh A
    *((volatile uint32_t*)(TIMER_BASE_ADDR+TIMER_GLBL_CFG)) = 0x1;

    // Configure PWM Port for one Time
    *((volatile uint32_t*)(GLBL_BASE_ADDR+GLBL_MULTI_FUNC))  |= 1 << pwm_num;

    return ch;
}

uint8_t Servo::attach(int pin, int min, int max)
{


    ch = pin;
    uint32_t pwm_num = variant_pin_map[ch].pwm_num;
    pinMode(pin, OUTPUT); // declare pwm pin to be an output:
   
    min = SERVO_MIN;
    max = SERVO_MAX;

   // Since PWM register does not reset immeditaly with change, we are reducing the
    // Timing tick from 1us to 200ns to run the PWM logic run 5 time faster - this timer
    // value need to corrected for MPW-7 onward - Atten - Dinesh A
    *((volatile uint32_t*)(TIMER_BASE_ADDR+TIMER_GLBL_CFG)) = 0x1;

    // Configure PWM Port for one Time
    *((volatile uint32_t*)(GLBL_BASE_ADDR+GLBL_MULTI_FUNC))  |= 1 << pwm_num;


    return ch;
}

void Servo::write(int angle)
{
    long value;
     uint32_t pwm_num = variant_pin_map[ch].pwm_num;

	if (angle < 0)
		angle = 0;
	else if (angle > 180)
		angle = 180;

	//value = map(angle, 0, 180, min, max);
        *((volatile uint32_t*) (pwm_num*4 + PWM_BASE_ADDR +PWM_CFG_LOW_BASE))   = (value << 16) | (max-value) ;
        Serial.print(value);
        Serial.print(":");
        Serial.print(min);
        Serial.print(":");
        Serial.print(max);
        Serial.print(":");
        Serial.println(max-value);
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
        uint32_t pwm_num = variant_pin_map[ch].pwm_num;
        *((volatile uint32_t*)(GLBL_BASE_ADDR+GLBL_MULTI_FUNC))  &= ~(1 << pwm_num);
	return;
}