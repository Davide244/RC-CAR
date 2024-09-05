#include "RCBluetooth.hpp"
#include <Arduino.h>

SoftwareSerial bluetoothSerial(ARDUINO_RX_PIN, ARDUINO_TX_PIN);

namespace RCCar 
{
	RCCar::BluetoothDevice::BluetoothDevice(char* DeviceName, int DeviceNameLenght)
	{
		pinMode(ARDUINO_RX_PIN, INPUT);
		pinMode(ARDUINO_TX_PIN, OUTPUT);

		bluetoothSerial = SoftwareSerial(ARDUINO_RX_PIN, ARDUINO_TX_PIN);

		// Open Serial Ports
		Serial.begin(9600);
		bluetoothSerial.begin(9600);

		// Send AT commands to the bluetooth module to set the name
		bluetoothSerial.print("AT+BMTestDevice\r\n");
	}

	void BluetoothDevice::run()
	{
		if (bluetoothSerial.available() > 0) 
		{
			// Empty the message buffer
			memset(msgBuf, 0, sizeof(msgBuf));

			// loop through all the available characters
			for (int i = 0; i < 128; i++) 
			{
				// read the next character
				char c = bluetoothSerial.read();
				// if the character is a newline or a semicolon, break out of the loop
				if (c == '\n' || c == ';')
				{
					break;
				}
				// add the character to the message buffer
				msgBuf[i] = c;
			}
			Serial.println(msgBuf);
		}
	}
}
