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
const char* url_auth = "https://projsante.herokuapp.com/api/auth";
const char* url_relay = "https://projsante.herokuapp.com/api/relays";

/***DE PROCESS.INO***/
/***BUFFER***/
#define NB_BUFFER_HTTP 200
#define BLE_DATA_LONG 9
#define NB_STACK_BLE 30
#define NUMBER_DATA_SAVED 30
#define ID_LENGTH 25
char g_buffer_http[NB_BUFFER_HTTP] = {0};//ID = 2 bytes
char g_buffer_ble[BLE_DATA_LONG] = {0};//buffer de reception ble
short g_ble_stack_id[NB_STACK_BLE] = {0};//Stack BLE

/***DEVICES***/
typedef struct{
  char ID[ID_LENGTH] = {0};
  char O2 = 0;
  char GainO2 = 0;
  char pulse = 0;
  char GainPulse = 0;
  char _status = 0;//b0:working;b1:enable
  char dataSaved = 0;//To know what is the index of data in data[][index]
  char data[BLE_DATA_LONG][NUMBER_DATA_SAVED] = {{0},{0}};//Save of the 30 previous data
}struct_sensor;

#define NB_SENSORS 100
struct_sensor g_sensor[NB_SENSORS];
/***DE PROCESS.INO***/


/***PROTOTYPES***/
void get_token(String token_str);
void get_id();
int long_char(char* str);

#define TOKEN_LONG 184
const char* user = "";
const char* password = "";
char token[TOKEN_LONG];
String token_str;
long timer_relay = 0;
bool token_done = false;

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  get_id();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  //WiFiMulti.addAP("TP-LINK_0330", "14537486");//"Go Concept", "Grenoble2018"
  WiFiMulti.addAP("Go Concept", "Grenoble2018");//"Go Concept", "Grenoble2018"
  timer_relay = millis();
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;
    if(millis() - timer_relay > 10000)//request relays
    {
      timer_relay = millis();
      Serial.print("[HTTPS] Relay...\n");
      if (https.begin(*client, url_relay)) // HTTPS https://projsante.herokuapp.com/api/auth
      {  
        https.addHeader("x-auth-token",token);
        int httpCode = https.GET();
      
        if (httpCode > 0) 
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
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
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }
    if(!token_done)
    {
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, url_auth)) // HTTPS https://projsante.herokuapp.com/api/auth
      {  
        https.addHeader("Content-Type","application/json");
        String data = "{ \"email\" : \"test@test.com\" , \"password\" : \"12345\" }";
        int httpCode = https.POST(data);
      
        if (httpCode > 0) 
        {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
          //get_token(https.getString());
          //Serial.println(token);
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
          {
            token_done = true;
            get_token(https.getString());
            Serial.println(token);
          }
        }
        else 
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
      delay(1000);
    }
  }
}

void get_token(String token_str)
{
  for(int i=0;i<183;i++)
  {
    token[i] = token_str[i+10];
  }
}

void get_id()
{
  char *data = "[{\"_id\":\"5d9b138104f4a117aa9f6b09\"},{\"_id\":\"5d9b138104f4a117aa9f6b09\"}]";
  char id[26];
  int j=0;
  bool debut = false;
  for(int i=0;i<long_char(data);i++)
  {
    if(data[i] == ':')
    {
      debut = true;
      j=0;
    }
    if(j == 24)
    {
      id[j] = '\0';
      Serial.println();
      j=0;
      debut = false;
    }
    if(debut && data[i]!=':' && data[i]!='"')
    {
      id[j] = data[i];
      Serial.print(data[i]);
      j++; 
    }
  }
  Serial.println(id);
}

void set_id_sensor(char* _id)
{
  int a = 0;
  for(int i=0;i<NB_SENSORS;i++)
  {
    if(g_sensor[i].ID != 0)
    {
      for(int j=0;j<ID_LENGTH;j++)
      {
        if(g_sensor[i].ID[j] == _id[j])
        {
          a++;
        }
      }
      if(a == ID_LENGTH)
      {
        //Sensor already exist
      }
      else
      {
        //Sensor does not exist : add one
        for(int k=0;k<NB_SENSORS;k++)
        {
          if(g_sensor[k].ID == 0)
          {
            for(int l=0;l<ID_LENGTH;l++)
            {
              g_sensor[k].ID[l] = _id[l];
            }
            
            _id = {0};
          }
        }
      }
    }
  }
}

int long_char(char* str)
{
  int i=0;
  while(str[i] != 0)
  {
    i++;
  }
  return i;
}
