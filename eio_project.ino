
#include <Wire.h>
#include "SHT2x.h"
#include "TMP275.h"

#define TMP_RESOLUTION 4

#define TMP_ADDRESS 0
#define RH_ADDRESS  1

typedef float packet[3]; 
/*
  packet => [ ADDRESS | SENSOR_DATA | CRC ?]
*/

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  while(!Serial) {};

  setup_TMP275();
}

void loop() 
{
  float temp_data, rh_data;
  packet temp_packet, rh_packet;
  /* Temperature acquisition */
  temp_data = get_temperature();
  wrapup_packet(TMP_ADDRESS, temp_data, &temp_packet);
  show_data("TEMP", temp_data);
  delay(500);

  /* Relative Humidity acquisition */
  rh_data = get_humidity();
  wrapup_packet(RH_ADDRESS, rh_data, &rh_packet);
  show_data("RH", rh_data);
  delay(500);
}

static void setup_TMP275(void)
{
  tmp275.init_tmp275();
  Serial.println("Initializing Temperature Sensor TMP275 ...");
  tmp275.setResolution(TMP_RESOLUTION);
  tmp275.enableShutdownMode(true);
}

static float get_temperature(void) 
{ 
  tmp275.enableOS();
  return tmp275.getTemperature();
}

static void show_data(String type, float data)
{
  if(type == "TEMP"){
    Serial.print("Temperature (Celsius):\t");
    Serial.println(data);
  }else if (type == "RH"){
    Serial.print("Humidity(%RH):\t");
    Serial.print(data);
  }else{
    Serial.print("Wrong data type !!");
  }

  Serial.print("\n");
}

static float get_humidity(void)
{
  return SHT2x.GetHumidity();
}

static void wrapup_packet(int address, float input_data, packet *data)
{
  (*data)[0] = address;
  (*data)[1] = input_data; 
}

