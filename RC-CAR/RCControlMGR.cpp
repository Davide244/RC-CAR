#include "RCControlMGR.hpp"
#include <Arduino.h>

namespace RCCar::Utils 
{
	int lerp(int a, int b, float f)
	{
		return (int)((float)a + f * ((float)b - (float)a));
	}
}

namespace RCCar 
{
	MotorControl::MotorControl(int PWMPin, int DirectionPin)
		: m_PWMPin(PWMPin), m_directionPin(DirectionPin), m_speed(0)
	{
		pinMode(PWMPin, OUTPUT);
		pinMode(DirectionPin, OUTPUT);
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
		// lerp the speed
		if (m_currentSpeed != m_speed)
		{
			m_currentSpeed = Utils::lerp(m_currentSpeed, m_speed, 0.1f);
		}

		analogWrite(m_PWMPin, m_currentSpeed);

		// Set the direction of the motor
		if (m_direction == MotorDirection::Clockwise)
			digitalWrite(m_directionPin, HIGH);
		else
			digitalWrite(m_directionPin, LOW);

	}
}
