#include "RCControlMGR.hpp"
#include <Arduino.h>

namespace RCCar::Utils 
{
	double lerp(double a, double b, double f)
	{
		return a + f * (b - a);
	}
}

namespace RCCar 
{
	MotorControl::MotorControl(int PWMPin_FWD, int PWMPin_BWD)
		: m_PWMPin_FWD(PWMPin_FWD), m_PWMPin_BWD(PWMPin_BWD), m_speed(0), m_currentSpeed(0)
	{
		pinMode(m_PWMPin_FWD, OUTPUT);
		pinMode(m_PWMPin_BWD, OUTPUT);
		pinMode(m_measurePin, INPUT);
	}

	void MotorControl::setSpeed(int speed, bool lerpChange)
	{
		m_speed = (double)speed + m_correctionFactor;
	}

	void MotorControl::setCorrectionFactor(int factor)
	{
		m_correctionFactor = factor;
	}

	void MotorControl::run()
	{
		m_currentSpeed = Utils::lerp(m_currentSpeed, m_speed, 0.01f);

		// IF speed is 0, set all PWM pins to LOW
		if (m_currentSpeed == 0) 
		{
			digitalWrite(m_PWMPin_FWD, LOW);
			digitalWrite(m_PWMPin_BWD, LOW);
			return;
		}

		if (m_speed > 0) 
		{
			analogWrite(m_PWMPin_FWD, (int)abs(m_currentSpeed));
			digitalWrite(m_PWMPin_BWD, LOW);
		}
		else if (m_speed < 0)
		{
			digitalWrite(m_PWMPin_FWD, LOW);
			analogWrite(m_PWMPin_BWD, (int)abs(m_currentSpeed));
		}

		// Write the motor speed to HW.
		//analogWrite((int)abs(m_speed) > 0 ? m_PWMPin_FWD : m_PWMPin_BWD, (int)abs(m_currentSpeed));
	}
}
