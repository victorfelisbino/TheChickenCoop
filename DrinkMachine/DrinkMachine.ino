
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"

int yellow = 5;
int red = 4;

// Counting channel details
unsigned long counterChannelNumber = SECRET_CH_ID_COUNTER;
const char * myCounterReadAPIKey = SECRET_READ_APIKEY_COUNTER;
unsigned int counterFieldNumber = 1; 

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
void setup() {
  Serial.begin(115200);  // Initialize serial

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  wifiManager.autoConnect("DrinkMachine");
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  digitalWrite(yellow,HIGH);
  digitalWrite(red,HIGH);
  delay(500);
  digitalWrite(yellow,LOW);
  digitalWrite(red,LOW);
}

void loop() {
  int statusCode = 0;

  // Read in field 1 of the private channel which is a counter  
  String json = ThingSpeak.readStringField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);  
  StaticJsonDocument<200> doc;
   // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200){
    Serial.println("Counter: " + String(json));
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, json);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    // Fetch values.
    String Pump1 = doc["Pump1"];
    Serial.println("Pump1: " + Pump1);
    
  }else{
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
  }
  
  delay(5000); // No need to read the counter too often.
}
