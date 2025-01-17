#include "SensorHLP.hpp"

namespace RCCar::Sensors 
{
	RCCar::Sensors::UltrasonicSensor::UltrasonicSensor(int echoPin, int triggerPin)
		: m_triggerPin(triggerPin), m_echoPin(echoPin), m_sensorSonar(triggerPin, echoPin)
	{
		pinMode(m_triggerPin, OUTPUT);
		pinMode(m_echoPin, INPUT);
	}

	void RCCar::Sensors::UltrasonicSensor::run()
	{
		m_value = m_sensorSonar.ping_cm();
	}
}
