#include "RCBluetooth.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>

//HardwareSerial SerialBLE;

static const char* BLEGreeting = "Hello, from RC-CAR!";

static constexpr uint8_t PacketSignature1 = 0xAA;
static constexpr uint8_t PacketSignature2 = 0x01;
static constexpr uint8_t PacketEndSignature = 0x1C;

namespace RCCar 
{
	RCCar::BluetoothDevice::BluetoothDevice(char* DeviceName, int DeviceNameLenght)
	{

		// Set the onboard LED to HIGH
		pinMode(LED_BUILTIN, OUTPUT);
	}

    enum RCDataType
    {
        MotorStateChange
    };

	byte lastRecievedData[5];
    uint8_t Data[1];
	byte currentByte;
	int8_t currentDataPosition = 0;

    void BluetoothDevice::run()
    {
        //Serial.println("Running Bluetooth Device!");
        while (Serial.available()) 
        {
            currentByte = Serial.readBytes(Data, 1);
            currentByte = Data[0];
            //Serial.println(currentByte);

            switch (currentDataPosition)
            {
                case 0: // Signature 1 Check
                    if (currentByte == PacketSignature1)
                        currentDataPosition++;

                    break;
                case 1: // Signature 2 Check
                    if (currentByte == PacketSignature2)
                        currentDataPosition++;
					else // Signature mismatch
                        currentDataPosition = 0;
                    break;
                case 7: // End Signature Check
                    if (currentByte == PacketEndSignature)
                    {
                        Serial.print("End Signature Found!");
                        digitalWrite(LED_BUILTIN, HIGH);
                        switch (lastRecievedData[0]) 
                        {
						case MotorStateChange:
							// IF byte 3 is 0xFF, then the value is negative
							// IF byte 3 is 0x00, then the value is positive
                            //if (lastRecievedData[2] == 0xFF)

							currentMotorSpeed = lastRecievedData[1];
							currentMotorDirection = lastRecievedData[2] == 0xFF ? MotorDirection::CounterClockwise : MotorDirection::Clockwise;

							currentMotorAngle = lastRecievedData[3];
							currentSteerDirection = lastRecievedData[4] == 0xFF ? MotorDirection::CounterClockwise : MotorDirection::Clockwise;

							Serial.print("Motor Steer Angle: ");
							Serial.println(currentMotorAngle);
							Serial.print("Motor Speed: ");
							Serial.println(currentMotorSpeed);
							break;
                        }
                    }
					else // Signature mismatch
                        currentDataPosition = 0;
                        

                    break;
                default:
                    //Serial.print("Data Found! VALUE: ");
                    //Serial.println(currentByte);
                    lastRecievedData[currentDataPosition - 2] = currentByte;
                    currentDataPosition++;
                    break;
                    
            }
        }
    }
}
