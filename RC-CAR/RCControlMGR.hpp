#pragma once

namespace RCCar 
{
	static constexpr int MOTOR_PIN_ACCELL_PWM = 6;
	static constexpr int MOTOR_PIN_STEER_PWM = 5;

	static constexpr int MOTORDRIVER_MAIN_DIRECTION_PIN = 10;
	static constexpr int MOTORDRIVER_STEER_DIRECTION_PIN = 7;

	enum MotorDirection 
	{
		Clockwise,
		CounterClockwise
	};

	class MotorControl 
	{
	public:
		/// <summary>
		/// Initializes a PWM motor controller. run() must be called in the main loop.
		/// </summary>
		/// <param name="PWMPin">The PWM pin. PWM signals come out of this pin.</param>
		/// <param name="DirectionPin">The Direction pin. This pin is connected to the H bridge.</param>
		MotorControl(int PWMPin, int DirectionPin);

		void setSpeed(int speed);
		void setDirection(MotorDirection direction);

		void run();

	private:
		int m_PWMPin;
		int m_directionPin;
		int m_speed;
		int m_currentSpeed;

		MotorDirection m_direction;
	};
}
