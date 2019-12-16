/*****************************************************
* ANTHONY RTC
* DESC: The Anthony RTC is a clock designed to turn a light
* on at different times to indicate wakeup time and breakfast
* time.
* Author: Jonathan L Clark
* Date: 12/12/2019
*****************************************************/

#include "DS3231.h"
#include <Wire.h>

#define GREEN_LED 3
#define RED_LED 4
// On the Arduino Nano SDA=A4, SCL=A5
RTClib RTC;
DS3231 Clock;

void setup() {
  // Start the serial port
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Start the I2C interface
  Wire.begin();
  //SetClock();
}

void SetClock()
{
   Clock.setYear(19);
   Clock.setMonth(12);
   Clock.setDate(15);
   Clock.setDoW(1);
   Clock.setHour(19);
   Clock.setMinute(51);
   Clock.setSecond(0);
}

void SerialPrintTime(DateTime now)
{
   Serial.print(now.year(), DEC);
   Serial.print('/');
   Serial.print(now.month(), DEC);
   Serial.print('/');
   Serial.print(now.day(), DEC);
   Serial.print(' ');
   Serial.print(now.hour(), DEC);
   Serial.print(':');
   Serial.print(now.minute(), DEC);
   Serial.print(':');
   Serial.print(now.second(), DEC);
   Serial.println();
}

void loop() {
    delay(1000);
    DateTime now = RTC.now();
    //SerialPrintTime(now);
    if (now.hour() >= 3 && now.hour() < 7 || now.hour() == 7 && now.minute() < 30)
    {
        digitalWrite(RED_LED, HIGH);
    }
    else if (now.hour() == 7 && now.minute() >= 30 && now.minute() < 35)
    {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
    }
    else if (now.hour() >= 9 && now.minute() >= 35)
    {
       digitalWrite(RED_LED, LOW);
       digitalWrite(GREEN_LED, LOW);
    }
}
