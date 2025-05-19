/*
  By Yaser Ali Husen
  GPIO Reference: https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
  Library: https://github.com/emelianov/modbus-esp8266
*/

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else //ESP32
#include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>

//for DHT sensor--------------------------------------
#include <DHT.h>
#define DHTPIN 2  //GPIO2 atau D4
// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT11     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
//for DHT sensor--------------------------------------

//ModbusIP object
ModbusIP mb;

//millis================================
//Set every  sec read DHT
unsigned long previousMillis = 0; // variable to store the last time the task was run
const long interval = 1000; // time interval in milliseconds (eg 1000ms = 1 second)
//======================================

void setup() {
  dht.begin();
  Serial.begin(9600);

  WiFi.begin("SSID", "Password");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mb.server();

  //configure holding register for temperature & humidity
  mb.addHreg(0);  //for temperatur integer value
  mb.addHreg(1);  //for temperature decimal value
  mb.addHreg(2);  //for humidity integer value
  mb.addHreg(3);  //for humidity decimal value

}

void loop() {
  delay(10);
  unsigned long currentMillis = millis(); // mendapatkan waktu sekarang
  // Checks whether it is time to run the task
  if (currentMillis - previousMillis >= interval) {
    // Save the last time the task was run
    previousMillis = currentMillis;
    //read DHT-11---------------------------------------
    t = dht.readTemperature();
    h = dht.readHumidity();
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.print("% ");
    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.println(" C ");
    //read DHT-11---------------------------------------

    //Write to Holding register temperature and humidity
    //temperature
    int t_int = int(t);
    int t_dec = int((t - t_int) * 100);
    mb.Hreg(0, t_int);
    mb.Hreg(1, t_dec);
    //humidity
    int h_int = int(h);
    int h_dec = int((h - h_int) * 100);
    mb.Hreg(2, h_int);
    mb.Hreg(3, h_dec);
  }
  //Call once inside loop()
  mb.task();

}
