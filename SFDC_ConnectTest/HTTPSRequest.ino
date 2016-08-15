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

const char* ssid = "Lab1-24G";
const char* password = "skynet2017";

//const char* host = "api.github.com";
const char* host = "na35.salesforce.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "00D41000000Liv1!AQoAQM5Bfx2u7bU.Y7ywbmbbSnc2WhiFaWcMyW7baVF2d.rF_QL7_lPVjCv5U67i_F5CvaVThpqgFAlb984srP1iyflu5du7";
WiFiClientSecure client;
void setup() {
  pinMode(1, OUTPUT);
  digitalWrite(1,HIGH);
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

  // Use WiFiClientSecure class to create TLS connection
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  String url = "/services/apexrest/chickencoop";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer 00D41000000Liv1!AQoAQDBzOX1B2H154v8X_HRCpOOeM400NO3pZAp5Ou2VhKbEUmTNxs5YYdyNBwTkpj_TekYx1b2U7X0hBXjG3N4KfvLwjbGn\r\n"+
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
}  

void loop() {
  digitalWrite(1,HIGH);
  while (client.available()) {
    //Serial.println("Reading and printing...");
   String c = client.readString();
    Serial.println(c);
  }
   // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    // do nothing forevermore:
    while(true);
  }
}
