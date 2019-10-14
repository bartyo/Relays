#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
// Fingerprint for demo URL, expires on June 2, 2019, needs to be updated well before this date
//const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};
const uint8_t fingerprint[20] = {0x08, 0x3B, 0x71, 0x72, 0x02, 0x43, 0x6E, 0xCA, 0xED, 0x42, 0x86, 0x93, 0xBA, 0x7E, 0xDF, 0x81, 0xC4, 0xBC, 0x62, 0x30};//Heroku
//const uint8_t fingerprint[20] = {0x5B, 0xFB, 0xD1, 0xD4, 0x49, 0xD3, 0x0F, 0xA9, 0xC6, 0x40, 0x03, 0x34, 0xBA, 0xE0, 0x24, 0x05, 0xAA, 0xD2, 0xE2, 0x01};//000webhostapp

//const char* url = "https://intense-thicket-18308.herokuapp.com";
//const char* url = "https://cautomatix.000webhostapp.com/essai_wifi.php";
//const char* url = "https://projsante.herokuapp.com/api/auth";
const char* url = "https://projsante.herokuapp.com/api/relays";

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("TP-LINK_0330", "14537486");//"Go Concept", "Grenoble2018"
  //WiFiMulti.addAP("Go Concept", "Grenoble2018");//"Go Concept", "Grenoble2018"
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, url)) {  // HTTPS https://projsante.herokuapp.com/api/users ; https://cautomatix.000webhostapp.com/essai_wifi.php

      //Serial.print("[HTTPS]"+url+"  POST...\n");
      // start connection and send HTTP header
      //https.addHeader("Content-Type", "application/json");
      https.addHeader("x-auth-token", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyIjp7ImlkIjoiNWQ4ZTU5ZTc0MTllZDYwYTk0MzRjYTc3In0sImlhdCI6MTU3MDcxMzE4MywiZXhwIjoxNjAyMjQ5MTgzfQ.9JP_zqftCCD0C9WTAN-AkdAfm53Y_uFFlPURpqv2feQ");
      //https.addHeader("upgrade-insecure-requests", "1");//application/x-www-form-urlencoded
      //https.addHeader("Accept", "*/*");
      //String data = "{ \"firstname\" : \"yoann\", \"email\" : \"yb@gc.com\", \"password\" : \"goconcept\" }";
      //String data = "{ \"email\" : \"test@test.com\" , \"password\" : \"12345\" }";
      //int httpCode = https.POST("hey="+data);
      //int httpCode = https.POST(data);
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) 
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        String payload = https.getString();
          Serial.println(payload);
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
        {
          String payload = https.getString();
          Serial.println(payload);
        }
      } 
      else 
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println("Wait 10s before next round...");
  delay(10000);
}
