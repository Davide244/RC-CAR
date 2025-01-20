#include <Wsh_SoftTimer.h>
#include <NewPing.h>
#include "RCControlMGR.hpp"
#include "RCBluetooth.hpp"
#include "RoboCar.hpp"


// THIS IS THE ROBOCAR BRANCH \\

// motor controller object
RCCar::MotorControl accellRightMotorControl(RCCar::MOTOR_PIN_LEFT_PWM_FWD, RCCar::MOTOR_PIN_LEFT_PWM_BWD);
RCCar::MotorControl accelLeftMotorControl(RCCar::MOTOR_PIN_RIGHT_PWM_FWD, RCCar::MOTOR_PIN_RIGHT_PWM_BWD);
RCCar::Sensors::UltrasonicSensor FrontLeft(A5, A4);
RCCar::Sensors::UltrasonicSensor FrontRight(A0, A1);

NewPing sonar(8, 9, 500);

RCCar::RoboCar roboCar(accelLeftMotorControl, 2, accellRightMotorControl, 3, RCCar::Sensors::UltrasonicSensor(A5, A4), RCCar::Sensors::UltrasonicSensor(A0, A1), A3);

// BT is not yet available on this device
//RCCar::BluetoothDevice bluetoothDevice("RC-CAR", 6);

void setup() 
{
	while (!Serial);
	Serial.begin(9600);

	
	
	//---------------------------------------------- Set PWM frequency for D5 & D6 -------------------------------
	//NOTE: Changing this timer 0 affects millis() and delay!
	//TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz    with this the motor doesn't whine
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

	//FrontRight.run();

	/*digitalWrite(A1, LOW);
	delayMicroseconds(2);
	digitalWrite(A1, HIGH);
	delayMicroseconds(10);
	digitalWrite(A1, LOW);*/

	//int duration = pulseIn(A0, HIGH);

	/*Serial.print("Front Right: ");
	Serial.println(FrontRight.getDistance());*/
}