#pragma once

namespace RCCar 
{
	static constexpr int MOTOR_PIN_ACCELL_PWM = 5;
	static constexpr int MOTOR_PIN_STEER_PWM = 6;

	enum MotorDirection 
	{
		Clockwise,
		CounterClockwise
	};

	class MotorControl 
	{
	public:
		MotorControl(int pin);

		void setSpeed(int speed);
		void setDirection(MotorDirection direction);

		void run();
		void test();

	private:
		int m_pin;
		int m_speed;

		MotorDirection m_direction;
	};
}
