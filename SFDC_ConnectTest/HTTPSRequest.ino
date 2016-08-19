/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <string.h>
#include <ArduinoJson.h>
#define MAX_STRING_LEN 40
const char* ssid = "Lab1-24G";
const char* password = "skynet2017";

//const char* host = "api.github.com";
const char* host = "na35.salesforce.com";
const int httpsPort = 443;
StaticJsonBuffer<200> jsonBuffer;
// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "00D41000000Liv1!AQoAQM5Bfx2u7bU.Y7ywbmbbSnc2WhiFaWcMyW7baVF2d.rF_QL7_lPVjCv5U67i_F5CvaVThpqgFAlb984srP1iyflu5du7";
WiFiClientSecure client;
void setup() {
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(0,LOW);
  digitalWrite(2,LOW);
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}  

void loop() {
  //check if wifi is still connected, if not, try to connect back
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP());
    Serial.println();
  }
  //call Salesforce and check if door needs to open or close
  // Use WiFiClientSecure class to create TLS connection
  //Serial.print("connecting to "+ host);
  if (!client.connect(host, httpsPort)) {
    //Serial.println("connection failed");
    return;
  }
  String url = "/services/apexrest/chickencoop";
  //Serial.print(".");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer 00D41000000Liv1!AQoAQAPPm3U.P1qfe5QZlWRDZTrl9hecbBm0oyqHrbY_C3WlX70hDXYDUCWOd5oEwuDgHv5XHaRSRcRJODz2BuJnMk4Ii0l9\r\n"+
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("Waiting on response from SFDC...");
  if(!client.available()){
    //Serial.print(".");
    delay(500);
  }
  int j = 1;
  Serial.println();
  while (client.available()) {
    //Serial.println("Reading and printing...");
    String line = client.readStringUntil('\n');
    //String c = client.readString();
    Serial.println(line);
    if(j == 10){
      char json[200];
      Serial.println(line);
      line.toCharArray(json,200);
      Serial.print("json buffer: ");
      Serial.println(json);
      JsonObject& root = jsonBuffer.parseObject(json);
      // Test if parsing succeeds.
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }
      String message = root["message"];
      Serial.println(message);
      if(message == "OPEN"){
        Serial.println("blinking time!!");
        digitalWrite(0,HIGH);
        delay(500);
        digitalWrite(0,LOW);   
        delay(500); 
        digitalWrite(0,HIGH);
        delay(500);
        digitalWrite(0,LOW);   
        delay(500); 
        digitalWrite(0,HIGH);
        delay(500);
        digitalWrite(0,LOW);   
        delay(500); 
      }else{
        Serial.println("no blinky blynky!!");
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);   
        delay(500); 
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);   
        delay(500); 
        digitalWrite(2,HIGH);
        delay(500);
        digitalWrite(2,LOW);   
        delay(500); 
      }
    }
    j++;
  }
   // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    while(true);
  }
  //delay(1000);
}
