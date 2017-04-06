#include <avr/wdt.h>
#include "sensor.h"

#include "Microduino_Matrix.h"
uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {  //2x2
  { 64, 63, 62},
  { 61, 60, 59}
};
Matrix display = Matrix(Addr, TYPE_COLOR);

void setup() {
  Serial.begin(115200); // See the connection status in Serial Monitor
  wdt_enable(WDTO_8S); //开启看门狗，并设置溢出时间为4秒

  Wire.begin();

  //display.clearFastMode();
  //display.setFastMode();
  display.clearDisplay();
  //setLedColor
  for (int y = 0; y < display.getHeight() * 8; y++) {
    for (int x = 0; x < display.getWidth() * 8; x++) {
      randomSeed(analogRead(A6));
      display.setLedColor(x, y, random(0, 255), random(0, 255), random(0, 255));   //x, y, r, g, b
      delay(5);
    }
  }
  delay(1000);
}


#define INTERVAL_SENSOR 4000
unsigned long sensor_time = 0;
int i = 0;
void loop() {
  if (sensor_time > millis()) {
    sensor_time = millis();
  }
  if (millis() - sensor_time > INTERVAL_SENSOR) {
    wdt_reset();
    i++;
    if (i > 3) {
      i = 0;
    }
    display.clearDisplay();
    switch (i) {
      case 0: {
          display.setColor(0, 255, 0);
          display.setCursor(1, 0);   //x, y
          display.print("PM");
          display.setCursor(1, 8); //x, y
          display.print(sensorPM25(), 0);
        }
        break;
      case 1: {
          display.setColor(255, 255, 0);
          display.setCursor(1, 0);   //x, y
          display.print("TEM");
          display.setCursor(1, 8); //x, y
          display.print(sensor_tem(), 0);
        }
        break;
      case 2: {
          display.setColor(255, 255, 0);
          display.setCursor(1, 0);   //x, y
          display.print("HUM");
          display.setCursor(1, 8); //x, y
          display.print(sensor_hum(), 0);
        }
        break;
      case 3: {
          display.setColor(0, 255, 255);
          display.setCursor(1, 0);   //x, y
          display.print("AIR");
          display.setCursor(1, 8); //x, y
          display.print(sensor_etoh(), 0);
        }
        break;
    }
    sensor_time = millis();
  }
}
