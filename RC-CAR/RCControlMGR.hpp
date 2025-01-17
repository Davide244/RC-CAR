#pragma once
#include "SensorHLP.hpp"

namespace RCCar 
{
	// Motor left PWM pin definitions
	static constexpr int MOTOR_PIN_LEFT_PWM_FWD = 10; // B-IA
	static constexpr int MOTOR_PIN_LEFT_PWM_BWD = 11; // B-IB

	// Motor right PWM pin definitions
	static constexpr int MOTOR_PIN_RIGHT_PWM_FWD = 5; // A-IA
	static constexpr int MOTOR_PIN_RIGHT_PWM_BWD = 6; // A-IB

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

		/// <summary>
		/// Set the speed of the motor.
		/// </summary>
		/// <param name="speed">The speed of the motor. Has to be between -255 and 255.</param>
		/// <param name="lerpChange">If false, the speed is set instantly, otherwise the speed is slowly set to the speed via a lerp function.</param>
		void setSpeed(int speed, bool lerpChange = true);

		void setCorrectionFactor(int factor);
		int getCorrectionFactor() { return m_correctionFactor; }

		/// <summary>
		/// Measures the actual speed of the motor for WatchDog. Outputs the PWM signal to the motor and sets the direction by setting the DIR pin.
		/// </summary>
		void run();

		/*static void SpeedMeasureInterruptHandler();*/

	private:
		int m_PWMPin_FWD;
		int m_PWMPin_BWD;

		double m_measurePin;
		double m_speed;				// Goal speed, This is the speed that the motor should be at
		double m_currentSpeed;		// The current speed of the motor. This is the lerped value.
		int m_correctionFactor = 0;	// The multiplier for the correction of the speed.

		unsigned long m_lastSpeedMeasureTime;	// The last time the speed was measured.

		int m_measuredSpeed;	// The measured speed of the motor. This is the physical speed of the motor, read on the previous loop cycle.
	};
}
