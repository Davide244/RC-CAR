#include "RoboCar.hpp"

namespace RCCar 
{
	RCCar::RoboCar::RoboCar(MotorControl motorLeft, int motorLeftMeasureSpeedPin, MotorControl motorRight, int motorRightMeasureSpeedPin, Sensors::UltrasonicSensor fronLeftSensor, Sensors::UltrasonicSensor frontRightSensor)
		: rbc_motorLeft(motorLeft), rbc_motorRight(motorRight), rbc_frontLeftSensor(fronLeftSensor), rbc_frontRightSensor(frontRightSensor)
	{
		pinMode(motorLeftMeasureSpeedPin, INPUT);
		pinMode(motorRightMeasureSpeedPin, INPUT);

		attachInterrupt(digitalPinToInterrupt(motorLeftMeasureSpeedPin), SpeedMeasureInterruptHandler_LEFTMOTOR, RISING);
		attachInterrupt(digitalPinToInterrupt(motorRightMeasureSpeedPin), SpeedMeasureInterruptHandler_RIGHTMOTOR, RISING);
	}

	unsigned long test = 0;
	void RoboCar::run()
	{
		currentMillis = millis();

		if (rbc_lastPingTime == -1)
			rbc_lastPingTime = millis();

		// Non blocking ping every 50ms
		if (millis() - rbc_lastPingTime > (50 * 64))
		{
			rbc_lastPingTime = millis();
			rbc_frontLeftSensor.run();
			rbc_frontRightSensor.run(); 
		}

		Serial.print("Left motor speed: ");
		Serial.println(rbc_motorLeftSpeed);
		Serial.print(" Right motor speed: ");
		Serial.println(rbc_motorRightSpeed);
		Serial.println();

		this->setSpeed(200);

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

		if (rbc_motorLeftSpeed > rbc_motorRightSpeed)
		{
			rbc_currentMotorSpeedDifference = rbc_motorLeftSpeed - rbc_motorRightSpeed;
			rbc_motorRight.setCorrectionFactor(rbc_motorLeft.getCorrectionFactor() - 5);
			//rbc_motorRight.setCorrectionFactor(rbc_motorLeft.getCorrectionFactor() + 1);
		}
		else if (rbc_motorLeftSpeed < rbc_motorRightSpeed)
		{
			rbc_currentMotorSpeedDifference = rbc_motorLeftSpeed - rbc_motorRightSpeed;
			rbc_motorRight.setCorrectionFactor(rbc_motorLeft.getCorrectionFactor() + 5);
			//rbc_motorRight.setCorrectionFactor(rbc_motorLeft.getCorrectionFactor() - 1);
		}

		RoboCar::rbc_motorLeft.setSpeed(speed);
		RoboCar::rbc_motorRight.setSpeed(speed);
	}

	void RoboCar::SpeedMeasureInterruptHandler_RIGHTMOTOR()
	{
		if (rbc_motorRightSpeedLastMeasureTime == 0) 
		{
			rbc_motorRightSpeedLastMeasureTime = millis() / 64;
			return;
		}

		/*Serial.print("Time diff right: ");
		Serial.println((millis() / 64) - rbc_motorRightSpeedLastMeasureTime);*/
		/*rbc_motorRightSpeed = (millis() / 64) - rbc_motorRightSpeedLastMeasureTime;*/


		// Calculate it in m/s. WHeel is 65cm in diameter.
		rbc_motorRightSpeed = 0.0105 / (((double)(millis() - rbc_motorRightSpeedLastMeasureTime)) / 1000);



		rbc_motorRightSpeedLastMeasureTime = millis() / 64;
	}

	void RoboCar::SpeedMeasureInterruptHandler_LEFTMOTOR()
	{
		// MAX speed of motors is 17ms between interrupts. That is 255.
		// LOWEST speed of motors is 75ms between interrupts. That is 50.

		if (rbc_motorLeftSpeedLastMeasureTime == 0)
		{
			rbc_motorLeftSpeedLastMeasureTime = millis() / 64;
			return;
		}

		/*Serial.print("Time diff left: ");
		Serial.println((millis() / 64) - rbc_motorLeftSpeedLastMeasureTime);*/
		/*rbc_motorLeftSpeed = (millis() / 64) - rbc_motorLeftSpeedLastMeasureTime;*/
		/*rbc_motorLeftSpeed = 0.0105 / (((double)(millis() - rbc_motorLeftSpeedLastMeasureTime)) / 1000);*/

		rbc_motorLeftSpeedLastMeasureTime = millis() / 64;
	}

}