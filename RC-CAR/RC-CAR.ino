#include <NewPing.h>
#include "RCControlMGR.hpp"
#include "RCBluetooth.hpp"
#include "RoboCar.hpp"


// THIS IS THE ROBOCAR BRANCH \\

// motor controller object
RCCar::MotorControl accellRightMotorControl(RCCar::MOTOR_PIN_LEFT_PWM_FWD, RCCar::MOTOR_PIN_LEFT_PWM_BWD);
RCCar::MotorControl accelLeftMotorControl(RCCar::MOTOR_PIN_RIGHT_PWM_FWD, RCCar::MOTOR_PIN_RIGHT_PWM_BWD);

NewPing sonar(8, 9, 500);

RCCar::RoboCar roboCar(accelLeftMotorControl, 2, accellRightMotorControl, 3, RCCar::Sensors::UltrasonicSensor(8, 9), RCCar::Sensors::UltrasonicSensor(8, 9));

// BT is not yet available on this device
//RCCar::BluetoothDevice bluetoothDevice("RC-CAR", 6);

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
	//bluetoothDevice.run();
	roboCar.run();

	//analogWrite(RCCar::MOTOR_PIN_LEFT_PWM_BWD, 255);
}