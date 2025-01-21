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
		if (speed == 0) 
		{
			m_speed = 0;
			m_currentSpeed = 0;

			resetCorrection();
		}
		else m_speed = (double)speed;

		if (!lerpChange)
			m_currentSpeed = m_speed;
	}

	void MotorControl::setCorrectionFactor(double factor)
	{
		m_correctionFactor = factor;
	}

	void MotorControl::increaseCorrection(int amount)
	{
		m_correctionFactor += amount;
	}

	void MotorControl::decreaseCorrection(int amount)
	{
		m_correctionFactor -= amount;
	}

	void MotorControl::resetCorrection()
	{
		m_correctionFactor = 0;
	}

	void MotorControl::run()
	{
		m_currentSpeed = Utils::lerp(m_currentSpeed, m_speed * m_correctionFactor, 0.05f);
		if (m_currentSpeed > 255) m_currentSpeed = 255;
		//Serial.println(m_correctionFactor);

		//Serial.println(m_correctionFactor);

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
