#include "RoboCar.hpp"
#include <Wsh_SoftTimer.h>

Wsh_SoftTimer initButtonTimer(AUTORESET);

namespace RCCar 
{
	RCCar::RoboCar::RoboCar(MotorControl motorLeft, int motorLeftMeasureSpeedPin, MotorControl motorRight, int motorRightMeasureSpeedPin, Sensors::UltrasonicSensor fronLeftSensor, Sensors::UltrasonicSensor frontRightSensor, int controlButtonPin)
		: rbc_motorLeft(motorLeft), rbc_motorRight(motorRight), rbc_frontLeftSensor(fronLeftSensor), rbc_frontRightSensor(frontRightSensor), controlButtonPin(controlButtonPin)
	{
		pinMode(motorLeftMeasureSpeedPin, INPUT);
		pinMode(motorRightMeasureSpeedPin, INPUT);
		pinMode(controlButtonPin, INPUT);

		attachInterrupt(digitalPinToInterrupt(motorLeftMeasureSpeedPin), SpeedMeasureInterruptHandler_LEFTMOTOR, RISING);
		attachInterrupt(digitalPinToInterrupt(motorRightMeasureSpeedPin), SpeedMeasureInterruptHandler_RIGHTMOTOR, RISING);
	}

	unsigned long test = 0;
	void RoboCar::run()
	{

		if (rbc_lastPingTime == -1)
			rbc_lastPingTime = millis();

		if (rbc_lastMotorCorrectionTime == -1)
			rbc_lastMotorCorrectionTime = millis();

		// Non blocking ping every 50ms
		if (millis() - rbc_lastPingTime > (50))
		{
			rbc_lastPingTime = millis();
			rbc_frontLeftSensor.run();
			rbc_frontRightSensor.run();

			/*Serial.print("Front Left: ");
			Serial.print(rbc_frontLeftSensor.getDistance());
			Serial.print("  |  Front Right: ");
			Serial.println(rbc_frontRightSensor.getDistance());*/
		}

		//Serial.print("Left motor speed: ");
		//Serial.print(rbc_motorLeftSpeed);
		//Serial.print("  |  Right motor speed: ");
		//Serial.println(rbc_motorRightSpeed);
		//Serial.println();

		if (millis() - rbc_lastMotorCorrectionTime > 100)
		{
			rbc_lastMotorCorrectionTime = millis();

			rbc_currentMotorSpeedDifference = rbc_motorLeftSpeed - rbc_motorRightSpeed;
			double correctionFactor = 1.0 * (abs(rbc_currentMotorSpeedDifference) * correctionFactorMultiplier);

			if (rbc_motorLeftSpeed > rbc_motorRightSpeed)
			{
				rbc_motorRight.setCorrectionFactor(correctionFactor);
				rbc_motorLeft.setCorrectionFactor(1);
				//rbc_motorLeft.increaseCorrection(1);
			}
			else if (rbc_motorLeftSpeed < rbc_motorRightSpeed)
			{
				rbc_motorRight.setCorrectionFactor(1);
				rbc_motorLeft.setCorrectionFactor(correctionFactor);
				//rbc_motorLeft.decreaseCorrection(1);
			}
		}

		Serial.print("Left motor speed: ");
		Serial.print(rbc_motorLeftSpeed);
		Serial.print("  |  Right motor speed: ");
		Serial.println(rbc_motorRightSpeed);

		// Check when the wheel interrupts were last measured. If it was more than 100ms ago, set speed to 0.
		if (millis() - rbc_motorLeftSpeedLastMeasureTime > 100)
		{
			rbc_motorLeftSpeed = 0;
		}

		if (millis() - rbc_motorRightSpeedLastMeasureTime > 100)
		{
			rbc_motorRightSpeed = 0;
		}

		/*Serial.print("Current state: ");
		Serial.print(rbc_currentState);
		Serial.print("  |  Left Distance: ");
		Serial.print(rbc_frontLeftSensor.getDistance());
		Serial.print("  |  Right Distance: ");
		Serial.println(rbc_frontRightSensor.getDistance());*/

		/*if (rbc_currentState != INIT) 
		{
			Serial.print("State: ");
			Serial.println(rbc_currentState);
		}*/

		switch (rbc_currentState)
		{
		case RCCar::INIT:

			rbc_motorLeft.setSpeed(0, false);
			rbc_motorRight.setSpeed(0, false);

			if (digitalRead(controlButtonPin) == HIGH)
			{
				initButtonTimer.reset();
				controlButtonPressed = true;
			}

			if (controlButtonPressed && digitalRead(controlButtonPin) == LOW)
			{
				controlButtonPressed = false;
				driveDistanceMeters++;
			}

			if (initButtonTimer.elapsed(1000) && driveDistanceMeters > 0) 
			{
				rbc_currentState = DRIVE;
				rbc_motorRightInterruptCounter = 0;
			}
			

			Serial.print("Drive distance: ");
			Serial.println(driveDistanceMeters);

			break;
		case RCCar::DRIVE:
			rbc_motorLeft.setSpeed(200);
			rbc_motorRight.setSpeed(200);

			if ((rbc_frontLeftSensor.getDistance() < 1200 || rbc_frontRightSensor.getDistance() < 1200) && (rbc_frontRightSensor.getDistance() > 0 && rbc_frontRightSensor.getDistance() > 0))
			{
				rbc_stateStartMillis = millis();
				rbc_currentState = STOP;
			}

			// Check if we reached the distance. OR if the is even one
			if (driveDistanceMeters > 0 && rbc_motorRightInterruptCounter >= driveDistanceMeters * 20)
			{
				rbc_motorRightInterruptCounter = 0;
				driveDistanceMeters = 0;
				rbc_currentState = INIT;
			}

			break;
		case RCCar::STOP:
			rbc_motorLeft.setSpeed(0, false);
			rbc_motorRight.setSpeed(0, false);

			if (millis() - rbc_stateStartMillis > 1000)
				rbc_motorRight.setSpeed(100);

			if ((rbc_frontLeftSensor.getDistance() >= 1200 || rbc_frontRightSensor.getDistance() >= 1200) && (rbc_frontRightSensor.getDistance() > 0 && rbc_frontRightSensor.getDistance() > 0))
			{
				rbc_stateStartMillis = millis();
				rbc_currentState = DRIVE;
			}
			break;
		case RCCar::TURN_LEFT:
			break;
		case RCCar::TURN_RIGHT:
			break;
		default:
			break;
		}

		// Check motor speeds for difference. If there is, adjust them with the correction factor. ++ the correction factor of the slower motor, -- the correction factor of the faster motor.
		// If the difference is 0, leave the correction factor as is.

		//this->setSpeed(200);

		/*rbc_motorLeft.setSpeed(255);
		rbc_motorRight.setSpeed(255);*/

		rbc_motorLeft.run();
		rbc_motorRight.run();

		// Write the motor speeds to serial.
		/*Serial.print("Left motor speed: ");
		Serial.print(rbc_motorLeftSpeed);
		Serial.print(" Right motor speed: ");
		Serial.println(rbc_motorRightSpeed);*/
	}

	// Set the speed of both motors.
	void RoboCar::setSpeed(int speed)
	{
		// HOW THIS WORKS:
		// 1.: Get the speed of both motors.
		// 2.: Slowly inch them closer to eachother.. Subtract the correction factor from the motor that is faster. Add the correction factor to the motor that is slower.

		RoboCar::rbc_motorLeft.setSpeed(speed);
		RoboCar::rbc_motorRight.setSpeed(speed);
	}

	void RoboCar::SpeedMeasureInterruptHandler_RIGHTMOTOR()
	{
		if (rbc_motorRightSpeedLastMeasureTime == 0) 
		{
			rbc_motorRightSpeedLastMeasureTime = millis();
			return;
		}

		/*Serial.print("Time diff right: ");
		Serial.println((millis() / 64) - rbc_motorRightSpeedLastMeasureTime);*/
		/*rbc_motorRightSpeed = (millis() / 64) - rbc_motorRightSpeedLastMeasureTime;*/


		// Calculate it in m/s. WHeel is 65cm in diameter.
		/*rbc_motorRightSpeed = 0.0105 / (((double)(millis() - rbc_motorRightSpeedLastMeasureTime)) / 1000);*/

		// 20 Interrupts per rotation
		// Wheel diameter is 66mm

		rbc_motorRightInterruptCounter++;

		// FORMULA: (20 * 0.066) / (time between interrupts in seconds)
		rbc_motorRightSpeed = (20 * 0.066) / (((double)(millis()) - rbc_motorRightSpeedLastMeasureTime) / 1000);

		rbc_motorRightSpeedLastMeasureTime = millis();
	}

	void RoboCar::SpeedMeasureInterruptHandler_LEFTMOTOR()
	{
		// MAX speed of motors is 17ms between interrupts. That is 255.
		// LOWEST speed of motors is 75ms between interrupts. That is 50.

		if (rbc_motorLeftSpeedLastMeasureTime == 0)
		{
			rbc_motorLeftSpeedLastMeasureTime = millis();
			return;
		}

		/*Serial.print("Time diff left: ");
		Serial.println((millis() / 64) - rbc_motorLeftSpeedLastMeasureTime);*/
		/*rbc_motorLeftSpeed = (millis() / 64) - rbc_motorLeftSpeedLastMeasureTime;*/
		/*rbc_motorLeftSpeed = 0.0105 / (((double)(millis() - rbc_motorLeftSpeedLastMeasureTime)) / 1000);*/

		// 20 Interrupts per rotation
		// Wheel diameter is 66mm

		// FORMULA: (20 * 0.066) / (time between interrupts in seconds)
		rbc_motorLeftSpeed = (20 * 0.066) / (((double)(millis()) - rbc_motorLeftSpeedLastMeasureTime) / 1000);

		rbc_motorLeftSpeedLastMeasureTime = millis();
	}

}