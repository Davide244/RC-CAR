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

	volatile static int rbc_motorRightInterruptCounter = 0;

	enum RoboCarState
	{
		INIT,
		DRIVE,
		DRIVE_FIXED_DISTANCE,
		STOP,
		TURN_LEFT,
		TURN_RIGHT

	};

	class RoboCar
	{
	public:
		RoboCar(MotorControl motorLeft, int motorLeftMeasureSpeedPin, MotorControl motorRight, int motorRightMeasureSpeedPin, Sensors::UltrasonicSensor fronLeftSensor, Sensors::UltrasonicSensor frontRightSensor, int controlButtonPin);

		void run();

	private:
		MotorControl rbc_motorLeft;
		MotorControl rbc_motorRight; 

		const double correctionFactorMultiplier = 0.0001;

		unsigned long rbc_lastPingTime = -1;
		unsigned long rbc_lastMotorCorrectionTime = -1;
		unsigned long rbc_stateStartMillis = -1;
		Sensors::UltrasonicSensor rbc_frontLeftSensor;
		Sensors::UltrasonicSensor rbc_frontRightSensor;
		//Sensors::UltrasonicSensor m_backLeftSensor;
		//Sensors::UltrasonicSensor m_backRightSensor;

		RoboCarState rbc_currentState = DRIVE;

		int rbc_motorLeftMeasureSpeedPin;
		int rbc_motorRightMeasureSpeedPin;

		int correctionalFactor = 0;
		int driveDistanceMeters = 0;
		int controlButtonPin;
		bool controlButtonPressed = false;

		int rbc_currentMotorSpeedDifference = 0;
		int rbc_currentMotorsOptimalSpeed = 0;

		void setSpeed(int speed);

		static void SpeedMeasureInterruptHandler_LEFTMOTOR();
		static void SpeedMeasureInterruptHandler_RIGHTMOTOR();
	};

}
