#pragma once
#include <NewPing.h>

namespace RCCar::Sensors
{
	class Sensor 
	{
	public:
		virtual void run() = 0;
		int getValue() { return m_value; }

	protected:
		int m_value;
	};

	class UltrasonicSensor : public Sensor
	{
	public:
		UltrasonicSensor(int echoPin, int triggerPin);

		/// <summary>
		/// This runs the sensor ping and calculates the distance in cm. It is written into m_distance.
		/// </summary>
		void run();

		/// <summary>
		/// Get the distance that was last calculated by run().
		/// </summary>
		/// <returns>m_distance. This is in centimeters.</returns>
		virtual int getDistance() { return m_value; }

	private:
		int m_triggerPin;
		int m_echoPin;

		int m_duration = 0;
		int m_value = 0;

		NewPing m_sensorSonar;
	};
}
