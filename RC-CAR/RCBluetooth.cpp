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
		RCMotorSpeed,
		RCMotorAngle
	};

	byte lastRecievedData[3];
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
                    {
                        currentDataPosition++;
                        //Serial.println("Signature 1 Found!");
                    }
                    break;
                case 1: // Signature 2 Check
                    if (currentByte == PacketSignature2)
                    {
                        currentDataPosition++;
                        //Serial.println("Signature 2 Found!");
                    }
                    else 
                    {
                        currentDataPosition = 0;
                        //Serial.print("Signature 2 MISMATCH!!!! GOT VALUE: ");
                        //Serial.println(currentByte);
                    }
                    break;
                case 5: // End Signature Check
                    if (currentByte == PacketEndSignature)
                    {
                        Serial.print("End Signature Found!");
                        digitalWrite(LED_BUILTIN, HIGH);
                        switch (lastRecievedData[0]) 
                        {
                            case RCMotorSpeed:
                                Serial.print(" SPEED: ");
                                currentMotorSpeed = (lastRecievedData[1] != 0) ? -(short)lastRecievedData[1] : (short)lastRecievedData[1];
                                Serial.println(currentMotorSpeed);
                                Serial.println((short)lastRecievedData[1]);
                                break;
                            case RCMotorAngle:
                                Serial.print(" ROT: ");
                                currentMotorAngle = (lastRecievedData[1] != 0) ? -(short)lastRecievedData[1] : (short)lastRecievedData[1];
                                Serial.println(currentMotorAngle);
                                break;
                        }
                    }
                    else 
                    {
                        //Serial.print("End Signature MISMATCH!!!! GOT VALUE: ");
                        //Serial.println(currentByte);
                        currentDataPosition = 0;
                    }
                        

                    break;
                default:
                    //Serial.print("Data Found! VALUE: ");
                    Serial.println(currentByte);
                    lastRecievedData[currentDataPosition - 2] = currentByte;
                    currentDataPosition++;
                    break;
                    
            }
        }
    }
}
