/*
     arduino   ->    esp
        3      ->     D3
        4      ->     D4
       GND     ->    GND
*/

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {

 float t = random(20, 25) + 0.2;
 float h = random(60,80);
 float lux = random(1000,2000);
 float voltage = random(2,4);
 float pHValue = random(5,9);

  // sent data to esp8266
  mySerial.print("Temp:" + String(t));
  mySerial.print(" Humid:" + String(h));
  mySerial.print(" Light:" + String(lux));
  mySerial.print(" Voltage:" + String(voltage, 2));
  mySerial.print(" PH:" + String(pHValue, 2));
  mySerial.print("\n");

  delay(1000);
}
