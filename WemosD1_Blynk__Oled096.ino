// SDA  Pin D2  Wemos D1 Pro
// SCL  Pin D1  Wemos D1 pro
#include "DHT.h"
 #include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "d057580798ec4c96acbXXXXXXXXXXXXX";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YOURWIFI";
char pass[] = "PASWORD";

#define DHTPIN 2          // Conectar a D4 del ESP8266 ARDUINO

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;
#define DHTPIN D4     // what pin we're connected to
 
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
 
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}
 
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(400000L);
Blynk.begin(auth, ssid, pass);
 timer.setInterval(1000L, sendSensor);
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
 
  dht.begin();
}
 
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
 
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    oled.println("Failed to read from DHT sensor!");
    return;
  }
 
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  oled.clear();
  oled.println("DHT 11   ");
  oled.println("Humidity & Temperatur");
 oled.println("------------------------");
 
  oled.print(h);
 oled.print(" %\t ");
  oled.print("   Temp ");
  oled.print(t);
  oled.print("  *C ");
  oled.println("------------------------");
 
 oled.print("Heat index: ");
 oled.print(hic);
 oled.println("                         ");
 oled.println("                         ");
 oled.println("                         ");
 oled.println("                         ");
  //oled.print(hif);
 // oled.print(" *F");
 Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
