#include <ArduinoBLE.h>
#include "RCControlMGR.hpp"
#include "RCBluetooth.hpp"

// motor controller object
RCCar::MotorControl steerMotorControl(RCCar::MOTOR_PIN_STEER_PWM);
RCCar::MotorControl accelMotorControl(RCCar::MOTOR_PIN_ACCELL_PWM);

RCCar::BluetoothDevice bluetoothDevice("RC-CAR", 6);

void setup()
{
}

void loop()
{
	steerMotorControl.setSpeed(255);
	steerMotorControl.run();

	accelMotorControl.setSpeed(50);
	accelMotorControl.run();

	bluetoothDevice.run();
}
