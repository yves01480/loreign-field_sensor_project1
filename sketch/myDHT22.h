#include "DHTesp.h"
const int DHT_PIN =32;

float temp, humid;


byte dht22_temp(){
  DHTesp dhtSensor;
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  temp = data.temperature;
  return temp;

  }

  byte dht22_humid(){
      DHTesp dhtSensor;
    dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
    TempAndHumidity  data = dhtSensor.getTempAndHumidity();
    
    humid = data.humidity;
    return humid;
  }
