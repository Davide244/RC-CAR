#pragma once
#include "SensorHLP.hpp"
#include "RCControlMGR.hpp"

namespace RCCar 
{
	volatile static unsigned long rbc_motorLeftSpeedLastMeasureTime = 0;
	volatile static unsigned long rbc_motorRightSpeedLastMeasureTime = 0;
	volatile static unsigned long currentMillis = 0;

	volatile static double rbc_motorLeftSpeed;
	volatile static double rbc_motorRightSpeed;

	class RoboCar
	{
	public:
		RoboCar(MotorControl motorLeft, int motorLeftMeasureSpeedPin, MotorControl motorRight, int motorRightMeasureSpeedPin, Sensors::UltrasonicSensor fronLeftSensor, Sensors::UltrasonicSensor frontRightSensor);

		void run();

	private:
		MotorControl rbc_motorLeft;
		MotorControl rbc_motorRight; 

		unsigned long rbc_lastPingTime = -1;
		Sensors::UltrasonicSensor rbc_frontLeftSensor;
		Sensors::UltrasonicSensor rbc_frontRightSensor;
		//Sensors::UltrasonicSensor m_backLeftSensor;
		//Sensors::UltrasonicSensor m_backRightSensor;

		int rbc_motorLeftMeasureSpeedPin;
		int rbc_motorRightMeasureSpeedPin;

		int correctionalFactor = 0;

		int rbc_currentMotorSpeedDifference = 0;
		int rbc_currentMotorsOptimalSpeed = 0;

		void setSpeed(int speed);

		static void SpeedMeasureInterruptHandler_LEFTMOTOR();
		static void SpeedMeasureInterruptHandler_RIGHTMOTOR();
	};

}
