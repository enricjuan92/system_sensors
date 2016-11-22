/*
Copyright (c) 2016, Embedded Adventures
All rights reserved.
Contact us at source [at] embeddedadventures.com
www.embeddedadventures.com
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
*/

// TMP275 MOD-1001 RTC and Temperature Sensor Arduino library
// Written originally by Embedded Adventures

#include "TMP275.h"
#include "Wire.h"

static int measurementDelay;	//28ms, 55ms, 110ms, 220ms

void TMP275Class::init_tmp275() {
	//Serial.println("init start");
	measurementDelay = 28;
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(CONF_REG);
	Wire.write(0x00);
	Wire.endTransmission();
	//Serial.println("init end");
}

uns8 TMP275Class::readConfigRegister() {
	uns8 config;
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(CONF_REG);
	Wire.endTransmission();
	
	Wire.requestFrom(TMP275_ADDR, 1);
	if (Wire.available())
		config = Wire.read();
	return config;
}

void TMP275Class::writeConfigRegister(uns8 config) {
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(CONF_REG);
	Wire.write(config);
	Wire.endTransmission();
}

float TMP275Class::getTemperature() {
	uns8 hi, lo;
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(TEMP_REG);
	Wire.endTransmission();
	
	delay(measurementDelay);
	
	Wire.requestFrom(TMP275_ADDR, 2);
	if (Wire.available()) {
		hi = Wire.read();
		lo = Wire.read(); 
	}
	sn16 temp = ((hi << 4) | (lo >> 4));
	return float(temp/16.0);
}

void TMP275Class::setResolution(int res) {
	uns8 config = readConfigRegister() & 0x9F;	//Mask out the res bits
	config |= uns8(res-1) << 5;
	
	writeConfigRegister(config);
	//Change measurement delay
	if (res == 1)
		measurementDelay = 28;
	else if (res == 2)
		measurementDelay = 55;
	else if (res == 3)
		measurementDelay = 110;
	else
		measurementDelay = 220;
}

void TMP275Class::enableShutdownMode(bool enable) {
	uns8 config = readConfigRegister();
	config &= 0xFE;		//Mask out LSB
	if (enable)
		config |= 0x01;
	writeConfigRegister(config);
}

void TMP275Class::enableOS() {
	enableShutdownMode(false);
	delay(measurementDelay);
	enableShutdownMode(true);
}

void TMP275Class::enableComparatorMode() {
	uns8 config = readConfigRegister();
	config &= 0xFD;	
	writeConfigRegister(config);
}

void TMP275Class::setHighTempThreshold(sn16 threshold) {
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(THIGH_REG);
	Wire.write(threshold >> 8);
	Wire.write(threshold & 0xFF);
	Wire.endTransmission();
}

void TMP275Class::setLowTempThreshold(sn16 threshold) {
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(TLOW_REG);
	Wire.write(threshold >> 8);
	Wire.write(threshold & 0xFF);
	Wire.endTransmission();
}

float TMP275Class::getHighTempThreshold() {
	uns8 hi, lo;
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(THIGH_REG);
	Wire.endTransmission();
	
	delay(measurementDelay);
	
	Wire.requestFrom(TMP275_ADDR, 2);
	if (Wire.available()) {
		hi = Wire.read();
		lo = Wire.read(); 
	}
	sn16 temp = ((hi << 4) | (lo >> 4));
	return float(temp/16.0);
}

float TMP275Class::getLowTempThreshold() {
	uns8 hi, lo;
	Wire.beginTransmission(TMP275_ADDR);
	Wire.write(TLOW_REG);
	Wire.endTransmission();
	
	delay(measurementDelay);
	
	Wire.requestFrom(TMP275_ADDR, 2);
	if (Wire.available()) {
		hi = Wire.read();
		lo = Wire.read(); 
	}
	sn16 temp = ((hi << 4) | (lo >> 4));
	return float(temp/16.0);
}

int TMP275Class::getFaults() {
	uns8 config = readConfigRegister();
	config = (config >> 3) & 0x03;
	return config;
}

TMP275Class tmp275;

