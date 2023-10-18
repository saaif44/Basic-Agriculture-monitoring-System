// Include Blynk libraries
#define BLYNK_TEMPLATE_ID "TMPL6EI7BOOm0"
#define BLYNK_TEMPLATE_NAME "soil testing"
#define BLYNK_AUTH_TOKEN "rC9VwZWVp3Ohnp7yJNFm2A_R8taOebNk"

#define BLYNK_PRINT Serial

// Include required libraries

#include <Wire.h> // I2C communication protocol library
#include <LiquidCrystal_I2C.h> // LCD library
#include <DHT.h> // DHT11 temperature and humidity sensor library
#include <SoftwareSerial.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sss";
char pass[] = "KiChaoBro**";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial

// Software Serial on Uno, Nano...
//for esp8266 wifi transmission
SoftwareSerial EspSerial(2,3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 38400

ESP8266 wifi(&EspSerial);

// BlynkTimer for reliability
BlynkTimer timer;

// Define DHT11 sensor digital pin and type
#define DHTPIN 11
#define DHTTYPE DHT11

// Initialize DHT11 and LCD modules
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define analog input pins for sensors and digital output pins for fan and buzzer
const int soilPin = A0;
const int buzzerPin = 8;
const int fanPin = 9;
const int LDR_PIN = A1;

enum DisplayMode {
  TMPHUMI, PERCENTMOISTURE, LIGHT
};

DisplayMode currentDisplay = TMPHUMI;
unsigned long displayTimer = 0;

void setup() {

  lcd.init();
  lcd.backlight();
   
  Serial.begin(115200); // Start serial communication
 EspSerial.begin(ESP8266_BAUD);
 
  delay(10);
    Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);

  dht.begin(); // Start DHT11 sensor

  // lcd.init(); // Initialize LCD module
  // lcd.backlight(); // Turn on LCD backlight

  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
  pinMode(fanPin, OUTPUT); // Set the fan pin as an output
  pinMode(LDR_PIN, INPUT); // Set the LDR pin as an input
}

void loop() {

    // Print Blynk connection status
  if (Blynk.connected()) {
    Serial.println("Connected to Blynk");
  } else {
    Serial.println("Not connected to Blynk");
  }

  Blynk.run();
  timer.run();



  float h = dht.readHumidity(); // Read humidity value from DHT11
  float t = dht.readTemperature(); // Read temperature value from DHT11
   // Turn on the fan if the temperature is over 30°C
  if (t > 30) {
    digitalWrite(fanPin, HIGH); // Turn on the fan
  } else {
    digitalWrite(fanPin, LOW); // Turn off the fan
  }

   // Read soil moisture sensor value
  int soilMoisture = analogRead(soilPin);
  // Convert analog value to percentage
  int percentMoisture = map(soilMoisture, 0, 1023, 0, 100);

  if (percentMoisture > 90) {
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
   Blynk.virtualWrite(V3, HIGH); 
  }
  else{
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    Blynk.virtualWrite(V3, LOW); 
  }

    // Read analog value from LDR pin
  int ldrValue = analogRead(LDR_PIN);

  // Convert analog reading of a photoresistor to a voltage reading and calculate Lux value
  float voltage = analogRead(A0) * (5.0 / 1023.0); // Read voltage from analog pin and convert to volts
  float lux = 500 / ((10000 * (5 - voltage)) / voltage); // Calculate the Lux value assuming a 10K Ohm resistor

   if (millis() - displayTimer >= 3000) {
    displayTimer = millis();
    currentDisplay = static_cast<DisplayMode>((static_cast<int>(currentDisplay) + 1) % 4);
  }


  

  // Check if the DHT11 sensor reading is valid
  if (isnan(h) || isnan(t)) {
    lcd.print("Failed DHT sensor!");
  }
  else{
    switch(currentDisplay){
      case TMPHUMI:     
      // Display temperature and humidity on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: "+ String(t)+("C"));
    lcd.setCursor(0, 1);
    lcd.print("Humidity: "+ String(h)+("%"));
    Blynk.virtualWrite(V0, t); 
    Blynk.virtualWrite(V1, h); 
      break;
      case PERCENTMOISTURE:
      // Display soil moisture and watering status on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil: " +String(percentMoisture)+ ("%"));
    Blynk.virtualWrite(V2, percentMoisture); 
    if(percentMoisture > 90){
      lcd.setCursor(0, 1);
    lcd.print("Need Water");
    }
    else{
    lcd.setCursor(0, 1);
    lcd.print("No Water Needed");
    }
      break;
      case LIGHT:
      // Set cursor position on first row, first column of LCD display and print light intensity value
    lcd.setCursor(0, 0);
    lcd.print("Light: "+String(ldrValue));
    // Set cursor position on second row, first column of LCD display and print Lux value
    lcd.setCursor(0, 1);
    lcd.print("Lux: "+ String(lux)+(" lm/m2")); // Print Lux value with units
    Blynk.virtualWrite(V4, ldrValue); 
    Blynk.virtualWrite(V5, lux); 
      break;
      
    }
  }

  //BLYNK PRINTING
  

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
  delay(1000);
}