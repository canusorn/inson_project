/*
 *    esp    ->    arduino
 *      3    ->     D3
 *      4    ->     D4
 *     GND   ->    GND
 */
 
 #include <SoftwareSerial.h>

SoftwareSerial mySerial(D4, D3); // RX, TX
float t, h, lux, voltage, pHValue;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.flush();
}

void loop() {

  while (mySerial.available() > 0) {

// read data from Arduino
     t = mySerial.parseFloat();
     h = mySerial.parseFloat();
     lux = mySerial.parseFloat();
     voltage = mySerial.parseFloat();
     pHValue = mySerial.parseFloat();

//print data
    if (mySerial.read() == '\n') {
    Serial.print("Temp:" + String(t));
    Serial.print(" Humid:" + String(h));
    Serial.print(" Light:" + String(lux));
    Serial.print(" Voltage:" + String(voltage,2));
    Serial.println(" PH:" + String(pHValue, 2));
    }
  }
}
