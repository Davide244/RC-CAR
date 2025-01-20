#include "SensorHLP.hpp"

namespace RCCar::Sensors 
{
	RCCar::Sensors::UltrasonicSensor::UltrasonicSensor(int echoPin, int triggerPin)
		: m_triggerPin(triggerPin), m_echoPin(echoPin), m_sensorSonar(triggerPin, echoPin)
	{
		pinMode(m_triggerPin, OUTPUT);
		pinMode(m_echoPin, INPUT);
	}

	void RCCar::Sensors::UltrasonicSensor::run(int max_distance)
	{
		/*int value = m_sensorSonar.ping_cm();
		m_value = m_sensorSonar.ping_cm() == 0 ? -1 : value;*/

		// We are using normal ping because the NewPing library is not working correctly.
		digitalWrite(m_triggerPin, LOW);
		delayMicroseconds(2);
		digitalWrite(m_triggerPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(m_triggerPin, LOW);

		m_duration = pulseIn(m_echoPin, HIGH);

		m_value = m_duration <= max_distance ? m_duration : max_distance;
		if (m_value < 0) m_value = 3500;

		//m_value = m_duration;
	}
}
