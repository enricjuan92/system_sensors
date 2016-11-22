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

#ifndef __TMP275_h
#define __TMP275_h

#include "Arduino.h"
#include "inttypes.h"

#define uns8 uint8_t
#define sn16 int16_t

#define TMP275_ADDR 0x48
#define TEMP_REG	0x00
#define CONF_REG	0x01
#define TLOW_REG	0x02
#define THIGH_REG	0x03


class TMP275Class {
private:
	
	uns8 readConfigRegister();
	void writeConfigRegister(uns8 config);
public:
	void  init_tmp275();
	float getTemperature();
	void  setResolution(int res);	//1 = 9bit, 2 = 10bit, 3 = 11bit, 4 = 12bit
	void  enableShutdownMode(bool enable);
	void  enableOS();
	void  enableComparatorMode();
	void  setHighTempThreshold(sn16 threshold);
	void  setLowTempThreshold(sn16 threshold);
	float getHighTempThreshold();
	float getLowTempThreshold();
	int   getFaults();
};

extern TMP275Class tmp275;

#endif