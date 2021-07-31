/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/


#include <WiFi.h>
#include "time.h"


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 18000;
const int   daylightOffset_sec = 3600;
const int PIR_IN = 33;


//Project: Babak
//Author : Aman Ullah Ghazi

//``````````````````````````Sensors initilization part```````````````````````````````````````````````````````````````````````````

//~~~~~~~~~~~~~lux meter part


#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

//~~~~~~~~~~~~~temp & humidity part

 
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>

float lux = 0;
int movement = 0;
float humidity = 0;
float tempf = 0;
bool mqtt_flag = false;
int counter = 1;
int samp_time = 30000;
int old_state = 0;
int mvmt_cntr = 0, mvmt_cmp = 0;
String dummy_macValue;


//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;

//---------------------------------------------------------------
//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````

#if defined(ARDUINO_SAMD_MKR1000) or defined(ESP8266)
#define __SKIP_ESP32__
#endif

#if defined(ESP32)
#define __ESP32_MQTT_H__
#endif

#ifdef __SKIP_ESP32__

#include <Arduino.h>

void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.println("Hello World");
}

#endif

#ifdef __ESP32_MQTT_H__
#include "esp32-mqtt.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  uint64_t mac = ESP.getEfuseMac();
  uint32_t hi = mac >> 32;
  uint32_t lo = mac;
  sprintf(macValue, "%04X%08X", hi, lo);
  Serial.println(macValue);
//  dummy_macValue = "9CC0EFAE114C";
//  Serial.println(dummy_macValue);


  
   //`````````````````````````````````````````````````Sensors Setup Part````````````````````````````````````````````````
   //~~~~~~~~~~~~~~~~~~~~lux meter part

  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));


  //~~~~~~~~~~~temp & humidity part

  //Initialize the I2C sensors and ping them
  sensor.begin();

 //````````````````````````````````````````````````````````````````````````````````````````````````` 
  
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_IN, INPUT);
  setupCloudIoT();
 
    
}

unsigned long lastMillis = 0;
void loop() {

//```````````````````````````````````````````````````````````Sensors Loop Part`````````````````````````````````````````````````````


//~~~~~~~~~~~~~~~~~Lux meter part


  //lux = lightMeter.readLightLevel();
  //Serial.println("\n");
  //Serial.println("Light: ");
  //Serial.print(lux);
  //Serial.println(" lx");
  delay(50);
    
    //~~~~~~~~~~~~~~~~~temp & humidity part
    
    //Get readings from all sensors
    //getWeather();
    printInfo();
    delay(50);

//motion sensor part

// read the input on analog pin 0:
  if (millis() > 40000) {
  int motionSensor = digitalRead(PIR_IN);
  int state = motionSensor;
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  // print out the value you read:
  
  if(state != old_state)
  {
    if (state == HIGH)
    {
      mvmt_cntr++;
      movement = 1;
    }

    else
    {
      movement = 0;
    }
  
    delay(100);
  }

  old_state = state;
  }

//sensorspayload();
printLocalTime();

//````````````````````````````````````````````````````````````````````````````````````````````````````````````````  
  mqtt->loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqttClient->connected()) {
    connect();
  }

  // TODO: replace with your code
  // publish a message roughly every second.
  if (millis() - lastMillis >= samp_time) {
    lastMillis = millis();
    getWeather();
    delay(50);
    //publishTelemetry(mqttClient, "/sensors", getDefaultSensor());
    //publishTelemetry(getDefaultSensor());
    if (mqtt_flag) {
      Serial.print(" Sent Packet -> ");
      Serial.println(counter);
      Serial.println(getpayloadJSON());
      publishTelemetry(getpayloadJSON());
      counter++;
    }
    //publishTelemetry(gettimestampJSON());

  }

  if(mvmt_cntr - mvmt_cmp == 1)
  {
    if (mqtt_flag) {
      getWeather();
      delay(50);
      Serial.print(" Sent Packet -> ");
      Serial.println(counter);
      Serial.println(getpayloadJSON());
      publishTelemetry(getpayloadJSON());
      Serial.println(" Motion Detected ");
      counter++;     
    }
    
    mvmt_cmp = mvmt_cntr;
  }
}
#endif
