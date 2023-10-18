
#define BLYNK_TEMPLATE_ID "TMPL6EI7BOOm0"
#define BLYNK_TEMPLATE_NAME "soil testing"
#define BLYNK_AUTH_TOKEN "rC9VwZWVp3Ohnp7yJNFm2A_R8taOebNk"
#define BLYNK_PRINT Serial
#include <DHT.h> 
#include <SoftwareSerial.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


char ssid[] = "Sss";
char pass[] = "KiChaoBro**";
SoftwareSerial EspSerial(2,3); // RX, TX
#define ESP8266_BAUD 38400
ESP8266 wifi(&EspSerial);
BlynkTimer timer;
#define DHTPIN 11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int soilPin = A0;
const int buzzerPin = 9;
const int fanPin = 10;
const int LDR_PIN = A1;

void setup() {
  Serial.begin(115200); 
  EspSerial.begin(ESP8266_BAUD);
 
  delay(10);
    Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
    dht.begin(); 
    pinMode(buzzerPin, OUTPUT); 
    pinMode(fanPin, OUTPUT); 
    pinMode(LDR_PIN, INPUT); 
}

void loop() {
  Blynk.run();
  timer.run();
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); 
  if (t > 20) {
    digitalWrite(fanPin, HIGH); 
  } else {
    digitalWrite(fanPin, LOW);
  }
  int soilMoisture = analogRead(soilPin);
  int percentMoisture = map(soilMoisture, 0, 1023, 0, 100);

  if (percentMoisture > 90) {
    digitalWrite(buzzerPin, HIGH); 
   delay(500);
   digitalWrite(buzzerPin, LOW); 
  }
  int ldrValue = analogRead(LDR_PIN);
  float voltage = analogRead(A0) * (5.0 / 1023.0); 
  float lux = 500 / ((10000 * (5 - voltage)) / voltage); 

    Blynk.virtualWrite(V0, t); 
    Blynk.virtualWrite(V1, h); 
    Blynk.virtualWrite(V2, percentMoisture);
    Blynk.virtualWrite(V4, ldrValue); 
    Blynk.virtualWrite(V5, lux);  

   // Print temperature and humidity values to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("% Temperature: ");
  Serial.print(t);
  Serial.println("°C");
    // Print soil moisture percentage value to Serial Monitor
  Serial.print("Soil : ");
  Serial.print(percentMoisture);
  Serial.println("%");
  Serial.print("LDRVALUE: "+String(ldrValue));
  Serial.print("Sun: "+ String(lux)+(" lm/m2"));
  // delay(1000);
}