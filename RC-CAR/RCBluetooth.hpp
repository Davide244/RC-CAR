#pragma once

#include "RCControlMGR.hpp"

#define ARDUINO_RX_PIN 0
#define ARDUINO_TX_PIN 1

namespace RCCar 
{
	class BluetoothDevice 
	{
	public:
		BluetoothDevice(char* DeviceName, int DeviceNameLenght);

		/// <summary>
		/// This function reads the bluetooth serial input. Put this in the main loop.
		/// </summary>
		void run();

		short* getMotorAngle() { return &currentMotorAngle; }
		short* getMotorSpeed() { return &currentMotorSpeed; }
		MotorDirection* getMotorDirection() { return &currentMotorDirection; }
		MotorDirection* getSteerDirection() { return &currentSteerDirection; }

	private:

		char msgBuf[128];
		char msg[128];

		short currentMotorAngle = 0;
		short currentMotorSpeed = 0;
		MotorDirection currentMotorDirection = MotorDirection::Clockwise;
		MotorDirection currentSteerDirection = MotorDirection::Clockwise;

	};
}
