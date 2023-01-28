/*
 *   arduino   ->    esp
 *      3      ->     D3
 *      4      ->     D4
 *     GND     ->    GND
 */

#include <SoftwareSerial.h>
#include <dht.h>
#include <Wire.h>
#include <BH1750.h>

#define dataPin 8
#define SensorPin A0
#define Offset 0.00
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40

int pHArray[ArrayLenth];
int pHArrayIndex = 0;

dht DHT;
BH1750 lightMeter;

SoftwareSerial mySerial(3, 4); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(LED, OUTPUT);
  Serial.println("pH meter experiment!");
  
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  int sensorValue = analogRead(A3);
  int readData = DHT.read22(dataPin);

  float voltage = sensorValue * (5.0 / 1024.0);
  float t = DHT.temperature;
  float h = DHT.humidity;
  float pHValue, voltage1;
  float lux = lightMeter.readLightLevel();

  unsigned long samplingTime = millis();
  unsigned long printTime = millis();

  Serial.println ("Sensor Output (V):");
  Serial.println (voltage);
  Serial.println();
  delay(1000);

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print((char)176);
  Serial.print("C | ");
  Serial.print((t * 9.0) / 5.0 + 32.0);
  Serial.print((char)176);
  Serial.println("F ");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println(" % ");
  Serial.println("");
  delay(1000);

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
  }
  
  if (millis() - printTime > printInterval)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    digitalWrite(LED, digitalRead(LED) ^ 1);
    printTime = millis();

    // sent data to esp8266
    mySerial.print("Temp:" + String(t));
    mySerial.print(" Humid:" + String(h));
    mySerial.print(" Light:" + String(lux));
    mySerial.print(" Voltage:" + String(voltage,2));
    mySerial.print(" PH:" + String(pHValue, 2));
    mySerial.print("\n");
  }
}

double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;      //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;

  delay(1000);
}
