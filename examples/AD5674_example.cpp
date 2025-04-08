/*
Copyright (c) 2025 Loris Mendolia

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

AD5674_example.cpp - Example code for the AD5674 and AD5674R DACs using the AD567X16 library.
This example generates a sawtooth wave on every other channel of the AD5674 between 0 and the external reference voltage,
and sets half the channels of the AD5674R to increasing voltages between 0 and 1.8V, using the internal reference.

Author: Loris Mendolia
Version: 1.0.0
Date: 2025-01-31
*/

#include <Arduino.h>
#include <AD567X16.h>

// Define the pins
#define CS_PIN 10
#define LDAC_PIN 9
#define RESET_PIN 8

#define CS_PIN_R 7
#define LDAC_PIN_R 6
#define RESET_PIN_R 5

// Constructor examples:
// Define AD5674 and AD5674R objects
AD5674Class myDAC(CS_PIN, LDAC_PIN, RESET_PIN, 3.3);  // The reference pin is set to 3.3V
AD5674RClass myDAC_R(CS_PIN_R, LDAC_PIN_R, RESET_PIN_R);

void setup(){
	// Power up/down example:
	// Power down half of the channels of the AD5674R
	uint8_t channels[8] = {8, 9, 10, 11, 12, 13, 14, 15}; // Channels to power down
	bool power_down[16] = {0, 0, 0, 0, 0, 0, 0, 0};
	myDAC_R.powerUpDown(channels, power_down, 8);

	// Channel setting example with float values:
	// Set the powered-up channels of the AD5674R to increasing voltages between 0 and 1.8V, using the internal reference
	for(int i = 0; i < 8; i++){
		myDAC_R.setChannel(i, (float) 0.225*i, true); // Set DAC_update to true to update the DAC output immediately
	}
}

void loop(){
	// Generate a sawtooth wave on every other channel of the AD5674 between 0 and the external reference voltage
	static word sawtooth_value = 0;
	static bool direction = true;
	for(int i = 0; i < 16; i+=2){
		// Channel setting example with word values:
		myDAC.setChannel(i, sawtooth_value); // Leave DAC_update to false (default) to use the input registers and update the DAC outputs later
	}

	// Update the DAC output
	myDAC.updateDAC(); // Update the DAC output of the AD5674, using the LDAC pin to update all channels at once

	// Update the sawtooth value
	if(direction){
		sawtooth_value += 100;
		if(sawtooth_value >= 4095){
			direction = false;
			sawtooth_value = 4095;
		}
	}
	else{
		sawtooth_value -= 100;
		if(sawtooth_value <= 0){
			direction = true;
			sawtooth_value = 0;
		}
	}

	delay(10);
}