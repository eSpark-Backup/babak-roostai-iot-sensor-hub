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
// This file contains static methods for API requests using Wifi / MQTT
#include <Client.h>
#include <rBase64.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "time.h"

#include <MQTT.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "ciotc_config.h" // Update this file with your configuration

// !!REPLACEME!!
// The MQTT callback function for commands and configuration updates
// Place your message handler code here.
void messageReceived(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);
  
  if (payload[0] == 'y') {
    mqtt_flag = true;
  }

  else if (payload[0] == 'n') {
    mqtt_flag = false;
  }
  
  //mqtt_flag = true;
  
  if (payload[1] == '0') {
    samp_time = 300000;
  }

  if (payload[1] == '1') {
    samp_time = 30000;
  }

  if (payload[1] == '2') {
    samp_time = 60000;
  }

  if (payload[1] == '3') {
    samp_time = 90000;
  }

  if (payload[1] == '4') {
    samp_time = 120000;
  }

  if (payload[1] == '5') {
    samp_time = 150000;
  }

  if (payload[1] == '6') {
    samp_time = 180000;
  }

  if (payload[1] == '7') {
    samp_time = 210000;
  }

  if (payload[1] == '8') {
    samp_time = 240000;
  }

  if (payload[1] == '9') {
    samp_time = 270000;
  }

}
///////////////////////////////

// Initialize WiFi and MQTT for this board
Client *netClient;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
MQTTClient *mqttClient;
unsigned long iat = 0;
String jwt;


char macValue[13];

String Chip;

byte pak_day;
byte pak_month;
int  pak_year;

byte pak_hour;
byte pak_min;
byte pak_sec;

///////////////////////////////
// Helpers specific to this board
///////////////////////////////
String getDefaultSensor(){
  return "Wifi: " + String(WiFi.RSSI()) + "db";
}

String ChipID()
{
//uint64_t mac = ESP.getEfuseMac();
//uint32_t hi = mac >> 32;
//uint32_t lo = mac;
//sprintf(macValue, "%04X%08X", hi, lo);
//Serial.print("ESP32_ChipID: ");
//Serial.print(macValue);
return dummy_macValue;

}

void printLocalTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  pak_day = timeinfo.tm_mday;
  pak_month = timeinfo.tm_mon+ 1;
  pak_year = timeinfo.tm_year+ 1900;

  pak_hour = timeinfo.tm_hour;
  pak_min  = timeinfo.tm_min;
  pak_sec  = timeinfo.tm_sec;

  //Serial.println(" month day ");
  //Serial.print(pak_day);
}


String getpayloadJSON(){
  return String("{\"ChipID\"~\"")+String(dummy_macValue)+String(",")+String("\"Timestamp\"~\"")+
  String(pak_month)+String("/")+String(pak_day)+String("/")+String(pak_year)+String(" ")+ 
  String(pak_hour)+String(":")+String(pak_min)+String(":")+ String(pak_sec)+String(",")+
  String("\"Temperature\"~\"")+String(tempf)+String(",")+String("\"Humidity\"~\"")+String(humidity)+String(",")+String("\"LUX\"~\"")+String(lux)+String(",")+
  String("\"Movement\"~\"")+String(movement)+String("\"}");
}


String gettimestampJSON(){
return String("{\"ChipID\":\"")+String(ChipID())+String("\"Timestamp\":\"")+ String(pak_day)+String("/")+String(pak_month)+String("/")+String(pak_year)+String(" ")+ String(pak_hour)+String(":")+String(pak_min)+String(":")+ String(pak_sec)+String("\"}");
}


String getJwt(){
  iat = time(nullptr);
  Serial.println("Refreshing JWT");
  jwt = device->createJWT(iat, jwt_exp_secs);
  Serial.println(jwt);
  return jwt;
}

void setupWifi(){
  Serial.println("Starting wifi");

  WiFi.mode(WIFI_STA);
  // WiFi.setSleep(false); // May help with disconnect? Seems to have been removed from WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
  }

  configTime(0, 0, ntp_primary, ntp_secondary);
  Serial.println("Waiting on time sync...");
  while (time(nullptr) < 1510644967){
    delay(10);
  }
}

void connectWifi(){
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
}

///////////////////////////////
// Orchestrates various methods from preceeding code.
///////////////////////////////
bool publishTelemetry(String data){
  return mqtt->publishTelemetry(data);
}

bool publishTelemetry(const char *data, int length){
  return mqtt->publishTelemetry(data, length);
}

bool publishTelemetry(String subfolder, String data){
  return mqtt->publishTelemetry(subfolder, data);
}

bool publishTelemetry(String subfolder, const char *data, int length){
  return mqtt->publishTelemetry(subfolder, data, length);
}

void connect(){
  connectWifi();
  mqtt->mqttConnect();
}

void setupCloudIoT(){
  device = new CloudIoTCoreDevice(
      project_id, location, registry_id, device_id,
      private_key_str);

  setupWifi();
  netClient = new WiFiClientSecure();
  mqttClient = new MQTTClient(512);
  mqttClient->setOptions(180, true, 1000); // keepAlive, cleanSession, timeout
  mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
  mqtt->setUseLts(true);
  mqtt->startMQTT();
}
