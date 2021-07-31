void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTempF();

  lux = lightMeter.readLightLevel();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}
//---------------------------------------------------------------
void printInfo()
{
//This function prints the weather data out to the default Serial Port

//  Serial.print("Temp:");
//  Serial.print(tempf);
//  Serial.print("F, ");
//
//  Serial.print("Humidity:");
//  Serial.print(humidity);
//  Serial.println("%");
}

void sensorspayload()
{
//  Serial.println(" Payload ");
//  Serial.println(lux);
//  Serial.println(movement);
//  Serial.println(tempf);
//  Serial.println(humidity);
  
  }
