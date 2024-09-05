#include "RCControlMGR.hpp"
#include "RCBluetooth.hpp"

// motor controller object
RCCar::MotorControl steerMotorControl(RCCar::MOTOR_PIN_STEER_PWM);
RCCar::MotorControl accelMotorControl(RCCar::MOTOR_PIN_ACCELL_PWM);

RCCar::BluetoothDevice bluetoothDevice("RC-CAR", 6);

void setup() 
{
	while (!Serial);
	Serial.begin(9600);
}

void loop() 
{
	bluetoothDevice.run();

	steerMotorControl.setSpeed(*bluetoothDevice.getMotorAngle());
	steerMotorControl.run();

	accelMotorControl.setSpeed(*bluetoothDevice.getMotorSpeed());
	accelMotorControl.run();
}
