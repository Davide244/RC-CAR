#include "RCControlMGR.hpp"
#include <Arduino.h>

namespace RCCar 
{
	MotorControl::MotorControl(int pin)
		: m_pin(pin), m_speed(0)
	{
		pinMode(pin, OUTPUT);
	}

	void MotorControl::setSpeed(int speed)
	{
		m_speed = speed;
	}

	void MotorControl::setDirection(MotorDirection direction)
	{
		m_direction = direction;
	}

	void MotorControl::run()
	{
		analogWrite(m_pin, m_speed);
	}

	void MotorControl::test()
	{
		digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(1000);              // wait for a second
		digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
		delay(1000);              // wait for a second
	}
}
