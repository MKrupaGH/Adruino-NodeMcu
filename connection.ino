#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(D2,D3);

const char *ssid = "ha";
const char *password = "12345678";

//const char *host = "https://express-weatherstation-production.up.railway.app:443";

//const char *fingerprint = "1E CB E3 98 65 D9 7C C3 CC 20 2D 49 15 06 76 B9 9F 66 89 48";
//const int port = 443;

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

String req="";
String reqPrev="";
char c;
bool isDone;
void setup() {
 delay(3000);
 Serial.begin(9600);
 espSerial.begin(9600);
 WiFi.begin(ssid,password);
 Serial.print("Connecting...");
 while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  while(espSerial.available()>0)
  {
    c = espSerial.read();
    if(c=='\n') {
      isDone=true;
      break;}
    else {req+=c;}
  }
    
  //if((millis() - lastTime) > timerDelay){
  if(isDone){
      if(WiFi.status()==WL_CONNECTED){
          
          Serial.println("Still connected");
          
          WiFiClient client;
          HTTPClient http;
          
          String serverName = "http://express-weatherstation-production.up.railway.app/catalog/sensor?" + req;
          Serial.println(serverName);
          
          http.begin(client, serverName);

          http.addHeader("Content-Type", "application/x-www-form-urlencoded");

          int httpResponseCode = http.GET();
          
          serverName="http://express-weatherstation-production.up.railway.app/catalog/sensor?";
          req="";
          
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          isDone=false;
          http.end();
        } else{
          req="";
          Serial.println("WiFi Disconnected");
        }

        lastTime = millis();
      }
  
    //}
}
