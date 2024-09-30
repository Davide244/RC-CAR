#include "RCControlMGR.hpp"
#include "RCBluetooth.hpp"

// motor controller object
RCCar::MotorControl steerMotorControl(RCCar::MOTOR_PIN_STEER_PWM, RCCar::MOTORDRIVER_STEER_DIRECTION_PIN);
RCCar::MotorControl accelMotorControl(RCCar::MOTOR_PIN_ACCELL_PWM, RCCar::MOTORDRIVER_MAIN_DIRECTION_PIN);

RCCar::BluetoothDevice bluetoothDevice("RC-CAR", 6);

void setup() 
{
	while (!Serial);
	Serial.begin(9600);

	//---------------------------------------------- Set PWM frequency for D5 & D6 -------------------------------
	//NOTE: Changing this timer 0 affects millis() and delay!
	TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz    with this the motor doesn't whine
	// TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
	// TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
	// TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
	// TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
}

void loop() 
{
	bluetoothDevice.run();

	steerMotorControl.setDirection(*bluetoothDevice.getSteerDirection());
	steerMotorControl.setSpeed(*bluetoothDevice.getMotorAngle());
	steerMotorControl.run();

	accelMotorControl.setDirection(*bluetoothDevice.getMotorDirection());
	accelMotorControl.setSpeed(*bluetoothDevice.getMotorSpeed());
	accelMotorControl.run();
}
