#include <Arduino.h>
#include <display4_7.h>

int i_call = 0;
float i = 0;
int digit_pins[] = {10,11,12,13};
int segment_pins[] = {9,7,5,3,2,8,6,4};
Display7Sements4Digits display47(digit_pins, segment_pins);

void setup() {
    Serial.begin(9600);
    display47.init();
}

void loop() {
    float sensorValue = (float) analogRead(A0);
    //float temp = sensorValue * (5.0 / 1024.0) * 100.;
    Serial.print(i);
    Serial.print(',');
    Serial.println(sensorValue);
    if (i_call > 32) {
       char buffer[6];
       dtostrf(sensorValue, 5, 1, buffer);
       display47.set(buffer);
       i_call = 0;
   } else {
       i_call++;
   }
    display47.refresh();
    delay(100);
    i++;
}
