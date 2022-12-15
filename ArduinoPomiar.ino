#include <Wire.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include <Catena-PM7003.h>
#include <TinyGPS.h>

#define DHTPIN 2
#define DHTTYPE DHT22

TinyGPS gps;
Adafruit_BMP280 bmp280;
BH1750 lightMeter;

SoftwareSerial espSerial(5,6);
//SoftwareSerial serialgps(3,4);

DHT dht(DHTPIN, DHTTYPE);

String str;
float latitude, longtitude;

void setup(){
  Serial.begin(115200);
  //Wire.begin();
  espSerial.begin(9600);
  //serialgps.begin(115200);
  dht.begin();
  lightMeter.begin();
  bmp280.begin(0x76);
  Serial.println("Setup loop working");
}

void loop(){
  /*while(serialgps.available()){
    int c = serialgps.read();
    Serial.print(serialgps.read());
    if(gps.encode(c)){
        
        gps.f_get_position(&latitude, &longtitude);
        Serial.print(latitude);
        Serial.print(longtitude);
      
      }
      
  }*/
 
  float h = round(dht.readHumidity());
  float t = round(dht.readTemperature());
  float p = round(bmp280.readPressure() / 100);
  float lux = lightMeter.readLightLevel();
  float l = round(lux/65535*10000);
  float lu = map(l, 0, 100, 100, 0);
  Serial.println(t);
  Serial.println(h);
  Serial.println(p);
  Serial.println(lu);
  str = String("temp=")+String(t)+String("&")+String("hum=")+String(h)+String("&")+String("pres=")+String(p)+String("&")+String("clo=")+String(lu)+String("&pm1=10")+String("&pm25=22")+String("&pm10=56");
  Serial.println(str);
  espSerial.println(str);
  delay(900000);
}
